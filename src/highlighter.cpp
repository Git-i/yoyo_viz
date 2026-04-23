#include "include/highlighter.hpp"
#include "qtextformat.h"
#include "tree_sitter/api.h"
#include "tree_sitter/tree-sitter-yoyo.h"
#include <exception>
#include <limits>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include "include/query.hpp"
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
    auto content = document()->toPlainText().toStdString();
    if (added > content.size()) added = content.size();
    TSInputEdit edit {
        .start_byte = static_cast<uint32_t>(position),
        .old_end_byte = static_cast<uint32_t>(position + removed),
        .new_end_byte = static_cast<uint32_t>(position + added),
        .start_point = TSPoint{},
        .old_end_point = TSPoint{},
        .new_end_point = TSPoint{}
    };
    auto old_content = content;
    old_content.erase(old_content.begin() + position, old_content.begin() + position + added);
    old_content.insert(static_cast<size_t>(position), static_cast<size_t>(removed), '$');
    std::println("{}", old_content);
    populate_edit_points(edit, old_content);
    ts_tree_edit(tree, &edit);
    auto old_tree = tree;
    tree = ts_parser_parse_string(parser, tree, content.c_str(), content.size());
    uint32_t num_ranges = 0;
    TSRange* ranges = ts_tree_get_changed_ranges(old_tree, tree, &num_ranges);
    uint32_t min_line = std::numeric_limits<uint32_t>::max(), max_line = 0;
    for (auto i : std::views::iota(0u, num_ranges)) {
        min_line = std::min(min_line, ranges[i].start_point.row);
        max_line = std::max(max_line, ranges[i].end_point.row);
    }
    highlights.clear();
    ts_query_cursor_set_point_range(queryCursor, TSPoint{.row = min_line, .column = 0}, TSPoint{.row = max_line + 1, .column = 0});
    runQueries(ts_tree_root_node(tree));
    free(static_cast<void*>(ranges));
    auto doc = document();
    std::set<int> done_blocks;
    for (auto& hl : highlights) {
        auto current_block = doc->findBlock(hl.start_byte);
        auto end_block = doc->findBlock(hl.end_byte);
        while (current_block != end_block) {
            if(done_blocks.contains(current_block.blockNumber())) continue;
            rehighlightBlock(current_block);
            done_blocks.insert(current_block.blockNumber());
            current_block = current_block.next();
        }
        if (!done_blocks.contains(end_block.blockNumber())) {
            rehighlightBlock(end_block);
            done_blocks.insert(end_block.blockNumber());
        }
    }
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
    initCatppuccin();
    buildQueries();
    queryCursor = ts_query_cursor_new();
}
YoyoHighlighter::~YoyoHighlighter() {
    if (tree) ts_tree_delete(tree);
    if (parser) ts_parser_delete(parser);
}
void YoyoHighlighter::initialHighlight() {
    auto content = document()->toPlainText().toStdString();
    std::println("{}", content);
    tree = ts_parser_parse_string(parser, nullptr, content.c_str(), content.size()); 
    auto root = ts_tree_root_node(tree);
    std::println("{}", ts_node_string(root));
    runQueries(root);
}
void YoyoHighlighter::runQueries(const TSNode& node) {
    for (auto query : highlightQueries) {
        ts_query_cursor_exec(queryCursor, query, node);
        TSQueryMatch match;
        while (ts_query_cursor_next_match(queryCursor, &match)) {
            for (auto i : std::views::iota(0u, match.capture_count)) {
                auto& capture = match.captures[i];
                uint32_t name_len = 0;
                auto name_ptr = ts_query_capture_name_for_id(query, capture.index, &name_len);
                std::string_view name {name_ptr, name_len};
                highlights.push_back(HighlightObj{
                    .start_byte = ts_node_start_byte(capture.node),
                    .end_byte =  ts_node_end_byte(capture.node),
                    .capture_name = std::string(name)
                });
            }
        }
    }

}
void YoyoHighlighter::initCatppuccin() {
    QTextCharFormat fmt;
    fmt.setForeground(QColor::fromString("#CBA6F7"));
    std::unordered_map<std::string, QTextCharFormat> colors = {};
    colors["mauve"] = fmt;
    fmt.setForeground(QColor::fromString("#fab387"));
    colors["peach"] = fmt;
    fmt.setForeground(QColor::fromString("#f9e2af"));
    colors["yellow"] = fmt;
    fmt.setForeground(QColor::fromString("#9399b2"));
    colors["overlay2"] = fmt;
    fmt.setForeground(QColor::fromString("#f2cdcd"));
    colors["flamingo"] = fmt;
    fmt.setForeground(QColor::fromString("#89b4fa"));
    colors["blue"] = fmt;
    fmt.setForeground(QColor::fromString("#a6e3a1"));
    colors["green"] = fmt;
    hl_theme["keyword.function"] = colors["mauve"];
    hl_theme["keyword.return"] = colors["mauve"];
    hl_theme["keyword.repeat"] = colors["mauve"];
    hl_theme["keyword.type"] = colors["yellow"];
    hl_theme["comment"] = colors["overlay2"];
    hl_theme["type"] = colors["yellow"];
    hl_theme["variable.member"] = colors["flamingo"];
    hl_theme["function"] = colors["blue"];
    hl_theme["string"] = colors["green"];
    hl_theme["number"] = colors["peach"];
}
void YoyoHighlighter::highlightBlock(const QString& text) {
    auto blk = currentBlock();
    auto blk_pos = static_cast<uint32_t>(blk.position());
    auto blk_size = static_cast<uint32_t>(blk.length());
    for (auto& hl : highlights) {
        if (blk_pos <= hl.start_byte && hl.start_byte <= (blk_pos + blk_size)) {
            auto num_chars = hl.end_byte - hl.start_byte;
            auto count = std::min(num_chars, blk_size - (hl.start_byte - blk_pos));
            std::println("blk_pos {} blk_size {} hl: start {} end {}", blk_pos, blk_size, hl.start_byte, hl.end_byte);
            setFormat(hl.start_byte - blk_pos, count, hl_theme[hl.capture_name]);
        }
    }
}
void YoyoHighlighter::buildQueries() {
    auto lang = tree_sitter_yoyo();
    auto query = [lang](std::string_view source) {
        uint32_t err_off; TSQueryError err = TSQueryErrorNone;
        auto query = ts_query_new(lang, source.data(), source.size(), &err_off, &err);
        if (err != TSQueryErrorNone) {
            std::println(stderr, "Error in query {}", source);
        }
        return query;
    };
    highlightQueries.push_back(query(hl_query));
}
