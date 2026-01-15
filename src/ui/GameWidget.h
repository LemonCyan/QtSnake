/**
 * @file GameWidget.h
 * @brief 游戏渲染组件 - 负责绘制游戏画面
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include "Constants.h"
#include "GameState.h"

namespace SnakeGame {

/**
 * @brief 游戏渲染组件 - 负责绘制蛇、食物和游戏区域
 * 
 * 职责：
 * - 接收后端信号更新渲染数据
 * - 绘制游戏画面
 * - 不包含任何游戏逻辑
 */
class GameWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param boardWidth 游戏区域宽度（格数）
     * @param boardHeight 游戏区域高度（格数）
     * @param cellSize 单元格像素大小
     * @param parent 父组件
     */
    explicit GameWidget(int boardWidth = Constants::kDefaultBoardWidth,
                        int boardHeight = Constants::kDefaultBoardHeight,
                        int cellSize = Constants::kCellSize,
                        QWidget* parent = nullptr);

public slots:
    /**
     * @brief 更新蛇身数据
     * @param body 蛇身坐标列表
     */
    void onSnakeMoved(const QVector<QPoint>& body);

    /**
     * @brief 更新食物位置
     * @param position 食物坐标
     */
    void onFoodSpawned(const QPoint& position);

    /**
     * @brief 更新游戏状态
     * @param state 游戏状态
     */
    void onGameStateChanged(GameState state);

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent* event) override;

private:
    int boardWidth_;            ///< 游戏区域宽度（格数）
    int boardHeight_;           ///< 游戏区域高度（格数）
    int cellSize_;              ///< 单元格像素大小

    QVector<QPoint> snakeBody_; ///< 蛇身坐标
    QPoint foodPosition_;       ///< 食物坐标
    GameState gameState_;       ///< 当前游戏状态

    /**
     * @brief 绘制网格背景
     * @param painter 画笔
     */
    void drawBackground(QPainter& painter);

    /**
     * @brief 绘制蛇
     * @param painter 画笔
     */
    void drawSnake(QPainter& painter);

    /**
     * @brief 绘制食物
     * @param painter 画笔
     */
    void drawFood(QPainter& painter);

    /**
     * @brief 绘制游戏状态覆盖层
     * @param painter 画笔
     */
    void drawOverlay(QPainter& painter);

    /**
     * @brief 将网格坐标转换为像素坐标
     * @param gridPos 网格坐标
     * @return 像素坐标矩形
     */
    QRect gridToPixel(const QPoint& gridPos) const;
};

}  // namespace SnakeGame

#endif  // GAMEWIDGET_H
