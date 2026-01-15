/**
 * @file GameLogic.h
 * @brief 游戏逻辑类头文件 - 核心游戏控制器
 * @author Snake Game Team
 * @date 2026-01-15
 * 
 * 本类是游戏后端的核心，通过 Qt 信号槽机制与前端 UI 解耦。
 * 前端只需连接信号即可接收游戏状态更新。
 */

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPoint>
#include <memory>

#include "Snake.h"
#include "Food.h"
#include "Direction.h"
#include "GameState.h"
#include "Constants.h"

namespace SnakeGame {

/**
 * @brief 游戏逻辑类 - 管理整个游戏流程
 * 
 * 职责：
 * - 管理游戏状态（开始、暂停、结束）
 * - 驱动游戏循环（通过 QTimer）
 * - 处理碰撞检测
 * - 管理分数
 * - 通过信号通知前端状态变化
 */
class GameLogic : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param boardWidth 游戏区域宽度（格数）
     * @param boardHeight 游戏区域高度（格数）
     * @param parent 父对象
     */
    explicit GameLogic(int boardWidth = Constants::kDefaultBoardWidth,
                       int boardHeight = Constants::kDefaultBoardHeight,
                       QObject* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~GameLogic() override;

    // ==================== 游戏控制 ====================

    /**
     * @brief 开始游戏
     */
    void startGame();

    /**
     * @brief 暂停游戏
     */
    void pauseGame();

    /**
     * @brief 恢复游戏
     */
    void resumeGame();

    /**
     * @brief 重置游戏到初始状态
     */
    void resetGame();

    // ==================== 输入处理 ====================

    /**
     * @brief 设置蛇的移动方向
     * @param direction 新方向
     */
    void setDirection(Direction direction);

    // ==================== 状态查询 ====================

    /**
     * @brief 获取当前游戏状态
     * @return 游戏状态
     */
    GameState getState() const;

    /**
     * @brief 获取当前分数
     * @return 分数
     */
    int getScore() const;

    /**
     * @brief 获取蛇身坐标
     * @return 蛇身坐标列表
     */
    QVector<QPoint> getSnakeBody() const;

    /**
     * @brief 获取食物位置
     * @return 食物坐标
     */
    QPoint getFoodPosition() const;

    /**
     * @brief 获取游戏区域宽度
     * @return 宽度（格数）
     */
    int getBoardWidth() const;

    /**
     * @brief 获取游戏区域高度
     * @return 高度（格数）
     */
    int getBoardHeight() const;

signals:
    // ==================== 信号（后端 → 前端） ====================

    /**
     * @brief 蛇移动后发出
     * @param body 更新后的蛇身坐标
     */
    void snakeMoved(const QVector<QPoint>& body);

    /**
     * @brief 食物生成后发出
     * @param position 食物位置
     */
    void foodSpawned(const QPoint& position);

    /**
     * @brief 分数变化后发出
     * @param score 新分数
     */
    void scoreChanged(int score);

    /**
     * @brief 游戏状态变化后发出
     * @param state 新状态
     */
    void gameStateChanged(GameState state);

    /**
     * @brief 游戏结束时发出
     * @param finalScore 最终分数
     */
    void gameOver(int finalScore);

private slots:
    /**
     * @brief 游戏主循环回调
     */
    void onGameTick();

private:
    // ==================== 成员变量 ====================

    std::unique_ptr<Snake> snake_;      ///< 蛇对象
    std::unique_ptr<Food> food_;        ///< 食物对象
    QTimer* gameTimer_;                 ///< 游戏循环定时器

    GameState state_;                   ///< 当前游戏状态
    int score_;                         ///< 当前分数
    int boardWidth_;                    ///< 游戏区域宽度
    int boardHeight_;                   ///< 游戏区域高度

    // ==================== 内部方法 ====================

    /**
     * @brief 检查蛇头是否撞墙
     * @param head 蛇头坐标
     * @return true 表示撞墙
     */
    bool checkWallCollision(const QPoint& head) const;

    /**
     * @brief 检查蛇头是否撞到自身
     * @param head 蛇头坐标
     * @return true 表示撞到自身
     */
    bool checkSelfCollision(const QPoint& head) const;

    /**
     * @brief 检查蛇头是否吃到食物
     * @param head 蛇头坐标
     * @return true 表示吃到食物
     */
    bool checkFoodCollision(const QPoint& head) const;

    /**
     * @brief 处理游戏结束
     */
    void handleGameOver();

    /**
     * @brief 生成新食物
     */
    void spawnFood();

    /**
     * @brief 设置游戏状态并发出信号
     * @param newState 新状态
     */
    void setState(GameState newState);
};

}  // namespace SnakeGame

#endif  // GAMELOGIC_H
