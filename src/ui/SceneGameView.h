/**
 * @file SceneGameView.h
 * @brief QGraphicsScene 游戏渲染视图
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef SCENEGAMEVIEW_H
#define SCENEGAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QVector>
#include <QPoint>
#include "Constants.h"
#include "GameState.h"

namespace SnakeGame {

/**
 * @brief QGraphicsScene 游戏渲染视图
 * 
 * 使用 QGraphicsScene/QGraphicsView 架构渲染游戏画面。
 * 与 GameWidget 实现相同的槽函数接口，可互换使用。
 * 
 * 职责：
 * - 接收后端信号更新渲染数据
 * - 使用 QGraphicsItem 绘制游戏元素
 * - 不包含任何游戏逻辑
 */
class SceneGameView : public QGraphicsView {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param boardWidth 游戏区域宽度（格数）
     * @param boardHeight 游戏区域高度（格数）
     * @param cellSize 单元格像素大小
     * @param parent 父组件
     */
    explicit SceneGameView(int boardWidth = Constants::kDefaultBoardWidth,
                           int boardHeight = Constants::kDefaultBoardHeight,
                           int cellSize = Constants::kCellSize,
                           QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~SceneGameView() override;

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

private:
    int boardWidth_;              ///< 游戏区域宽度（格数）
    int boardHeight_;             ///< 游戏区域高度（格数）
    int cellSize_;                ///< 单元格像素大小

    QGraphicsScene* scene_;       ///< 场景对象
    GameState gameState_;         ///< 当前游戏状态

    // 图形项
    QVector<QGraphicsRectItem*> snakeItems_;  ///< 蛇身矩形项
    QGraphicsEllipseItem* foodItem_;          ///< 食物椭圆项
    QGraphicsRectItem* overlayItem_;          ///< 状态覆盖层背景
    QGraphicsTextItem* overlayText_;          ///< 状态覆盖层文字

    /**
     * @brief 初始化场景
     */
    void setupScene();

    /**
     * @brief 绘制网格背景
     */
    void drawBackground();

    /**
     * @brief 更新蛇身图形项
     * @param body 蛇身坐标
     */
    void updateSnakeItems(const QVector<QPoint>& body);

    /**
     * @brief 更新覆盖层显示
     */
    void updateOverlay();

    /**
     * @brief 将网格坐标转换为场景坐标
     * @param gridPos 网格坐标
     * @return 场景坐标矩形
     */
    QRectF gridToScene(const QPoint& gridPos) const;
};

}  // namespace SnakeGame

#endif  // SCENEGAMEVIEW_H
