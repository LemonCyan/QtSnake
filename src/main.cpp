/**
 * @file main.cpp
 * @brief 贪吃蛇游戏程序入口
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include <QApplication>
#include <QDebug>
#include "MainWindow.h"
#include "RendererType.h"

using namespace SnakeGame;

/**
 * @brief 解析命令行参数中的渲染器类型
 * @param args 命令行参数列表
 * @return 渲染器类型
 */
RendererType parseRendererType(const QStringList& args)
{
    for (const QString& arg : args) {
        if (arg.startsWith("--renderer=")) {
            QString value = arg.mid(11).toLower();
            if (value == "scene") {
                qInfo() << "Using QGraphicsScene renderer";
                return RendererType::Scene;
            } else if (value == "widget") {
                qInfo() << "Using QPainter (Widget) renderer";
                return RendererType::Widget;
            } else {
                qWarning() << "Unknown renderer type:" << value << ", falling back to Widget";
            }
        }
    }
    // 默认使用 Widget 渲染器
    return RendererType::Widget;
}

/**
 * @brief 程序入口
 * @param argc 命令行参数数量
 * @param argv 命令行参数
 * @return 应用程序退出码
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    QApplication::setApplicationName("Snake Game");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("SnakeGame Team");

    // 解析渲染器类型
    RendererType rendererType = parseRendererType(QCoreApplication::arguments());

    // 创建并显示主窗口
    MainWindow mainWindow(rendererType);
    mainWindow.show();

    return app.exec();
}
