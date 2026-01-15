/**
 * @file MainWindow.h
 * @brief 主窗口类 - 组合所有 UI 组件并处理输入
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <memory>

#include "GameLogic.h"
#include "GameWidget.h"
#include "SceneGameView.h"
#include "RendererType.h"

namespace SnakeGame {

/**
 * @brief 主窗口类 - 应用程序主界面
 * 
 * 职责：
 * - 组合游戏渲染组件和分数显示
 * - 处理键盘输入并转发给后端
 * - 连接前后端信号槽
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param rendererType 渲染器类型
     * @param parent 父组件
     */
    explicit MainWindow(RendererType rendererType = RendererType::Widget,
                        QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MainWindow() override;

protected:
    /**
     * @brief 键盘按下事件
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    /**
     * @brief 分数变化处理
     * @param score 新分数
     */
    void onScoreChanged(int score);

    /**
     * @brief 游戏状态变化处理
     * @param state 新状态
     */
    void onGameStateChanged(GameState state);

    /**
     * @brief 游戏结束处理
     * @param finalScore 最终分数
     */
    void onGameOver(int finalScore);

private:
    std::unique_ptr<GameLogic> gameLogic_;  ///< 游戏逻辑（后端）
    RendererType rendererType_;              ///< 渲染器类型
    GameWidget* gameWidget_;                 ///< QPainter 渲染组件
    SceneGameView* sceneView_;               ///< QGraphicsScene 渲染组件
    QLabel* scoreLabel_;                     ///< 分数显示
    QLabel* statusLabel_;                    ///< 状态显示

    /**
     * @brief 初始化 UI 布局
     */
    void setupUI();

    /**
     * @brief 连接前后端信号槽
     */
    void connectSignals();
};

}  // namespace SnakeGame

#endif  // MAINWINDOW_H
