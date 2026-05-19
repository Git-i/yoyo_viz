#include "error_parser.hpp"
#include <QRegularExpression>
#include "compiler_state.hpp"
#include "qobject.h"
#include "qregularexpression.h"
void ErrorViewer::setCompiler(CompilerState* state) {
    stt = state;
}
CompilerState* ErrorViewer::nullCompilerState() const {
    return nullptr;
}

int ErrorViewer::numErrors() {
    if (stt == nullptr) return 0;
    return static_cast<int>(stt->output.errors.size());
}
static QString ansiToHtml(const std::string& str) {
    QString final(str.c_str());
    final.replace(
        QRegularExpression("\\033\\[1;31m"), 
        "<font color=\"#d20f39\">")
    .replace(
        QRegularExpression("\\033\\[1;34m"),
        "<font color=\"#1e66f5\">")
    .replace(
        QRegularExpression("\\033\\[0m"),
        "</font>")
    .replace("\n", "<br>");
    return "<span style='white-space: pre-wrap'>" + final + "</span>";
}
QString ErrorViewer::errorAt(int index) {
    if (index == -1) return "";
    return ansiToHtml(stt->output.errors[static_cast<size_t>(index)].second);
}
