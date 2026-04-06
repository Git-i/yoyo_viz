#include "qqml.h"
#include "include/highlighter.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
int main(int argc, char **argv) {
    QGuiApplication::setApplicationName("Yoyo visualizer");
    QGuiApplication app(argc, argv);
    
    QQmlApplicationEngine engine;
    qmlRegisterType<Highlighter>("com.yoyoviz.cpp", 1, 0, "Highlighter");
    engine.load(QUrl("qrc:/qt/qml/YoyoViz/Main.qml"));

    return app.exec();
}

