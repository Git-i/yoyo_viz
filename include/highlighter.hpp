#pragma once
#include "qtextdocument.h"
#include <print>
#include <QQuickTextDocument>
#include <QSyntaxHighlighter>
extern "C" {
struct TSTree;
struct TSParser;
}
class YoyoHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    TSTree* tree = nullptr;
    TSParser* parser = nullptr;
    explicit YoyoHighlighter(QObject* parent);
    void highlightBlock(const QString& text) override {}
    void refreshHighlights(int position, int removed, int added);
    void changeDocument(QTextDocument* doc);
    void initialHighlight();
    ~YoyoHighlighter();
};
class Highlighter : public QObject {
    Q_OBJECT
    YoyoHighlighter hl;
public:
    explicit Highlighter(QObject* parent = nullptr) : QObject(parent), hl(this) {}
    Q_INVOKABLE void setDocument(QQuickTextDocument* doc) {
        auto internal_doc = doc->textDocument();
        hl.changeDocument(internal_doc);
        QObject::connect(internal_doc, &QTextDocument::contentsChange, &hl, &YoyoHighlighter::refreshHighlights);
        std::println("stuff");
    }
};
