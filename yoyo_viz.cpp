#include "qqml.h"
#include "include/highlighter.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "include/compiler_state.hpp"
int main(int argc, char **argv) {
    QGuiApplication::setApplicationName("Yoyo visualizer");
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    // qmlRegisterType<Highlighter>("com.yoyoviz.cpp", 1, 0, "Highlighter");
    // qmlRegisterType<CompilerState>("com.yoyoviz.cpp", 1, 0, "CompilerState");
    engine.load(QUrl("qrc:/qt/qml/YoyoViz/Main.qml"));

    return app.exec();
}

