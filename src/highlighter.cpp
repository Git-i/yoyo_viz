#include "include/highlighter.hpp"
#include "tree_sitter/api.h"
#include "tree_sitter/tree-sitter-yoyo.h"
#include <exception>
#include <print>
#include <ranges>
static void populate_edit_points(TSInputEdit& edit, const std::string& text) {
    uint32_t bytes_off = 0;
    uint32_t line_no = 0;
    bool done_start = false, done_new_end = false, done_old_end = false;
    for (auto line : text | std::views::split('\n')) {
        auto new_byte_off = bytes_off + 1 + line.size();
        if (edit.start_byte <= new_byte_off && !done_start) {
            done_start = true;
            edit.start_point.row = line_no;
            edit.start_point.column = (edit.start_byte - bytes_off);
            std::println("start point: {} {}", edit.start_point.row, edit.start_point.column);
        }
        if (edit.new_end_byte <= new_byte_off && !done_new_end) {
            done_new_end = true;
            edit.new_end_point.row = line_no;
            edit.new_end_point.column = (edit.new_end_byte - bytes_off);
        }
        if (edit.old_end_byte <= new_byte_off && !done_old_end) {
            done_old_end = true;
            edit.old_end_point.row = line_no;
            edit.old_end_point.column = (edit.old_end_byte - bytes_off);
        }
        if (done_start && done_new_end && done_old_end) break;
        line_no++;
        bytes_off = new_byte_off;
    }
}
void YoyoHighlighter::refreshHighlights(int position, int removed, int added) {
    std::println("pos: {}, removed: {}, added: {}", position, removed, added);
    TSInputEdit edit {
        .start_byte = static_cast<uint32_t>(position),
        .old_end_byte = static_cast<uint32_t>(position + removed),
        .new_end_byte = static_cast<uint32_t>(position + added),
        .start_point = TSPoint{},
        .old_end_point = TSPoint{},
        .new_end_point = TSPoint{}
    };
    auto content = document()->toPlainText().toStdString();
    auto old_content = content;
    old_content.erase(old_content.begin() + position, old_content.begin() + position + added);
    old_content.insert(static_cast<size_t>(position), static_cast<size_t>(removed), '$');
    std::println("{}", old_content);
    populate_edit_points(edit, old_content);
    ts_tree_edit(tree, &edit);
    tree = ts_parser_parse_string(parser, tree, content.c_str(), content.size());
    std::println("{}", ts_node_string(ts_tree_root_node(tree)));
}
void YoyoHighlighter::changeDocument(QTextDocument* doc) {
    setDocument(doc);
    initialHighlight();
}
YoyoHighlighter::YoyoHighlighter(QObject* parent) : QSyntaxHighlighter(parent) {
    parser = ts_parser_new();
    if (ts_parser_set_language(parser, tree_sitter_yoyo()) != true) {
        std::println(stderr, "Tree sitter version mismatch");
        std::terminate();
    }
}
YoyoHighlighter::~YoyoHighlighter() {
    if (tree) ts_tree_delete(tree);
    if (parser) ts_parser_delete(parser);
}
void YoyoHighlighter::initialHighlight() {
    auto content = document()->toRawText().toStdString();
    tree = ts_parser_parse_string(parser, nullptr, content.c_str(), content.size()); 
    std::println("{}", ts_node_string(ts_tree_root_node(tree)));
}
