#pragma once
#include "tree_sitter/api.h"
#include <yoyo_highlighter.hpp>
#include <print>
#include <QQuickTextDocument>
#include <QSyntaxHighlighter>
#include <QtQml/qqmlregistration.h>
#include <unordered_map>
class Highlighter : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Highlighter(QObject* parent = nullptr) : QObject(parent), hl(this) {}
    Q_INVOKABLE void setDocument(QQuickTextDocument* doc) {
        auto internal_doc = doc->textDocument();
        hl.changeDocument(internal_doc);
        QObject::connect(internal_doc, &QTextDocument::contentsChange, &hl, &YoyoHighlighter::refreshHighlights);
        std::println("stuff");
    }
private:
    YoyoHighlighter hl;
};
