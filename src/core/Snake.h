/**
 * @file Snake.h
 * @brief 蛇类头文件 - 管理蛇的身体和移动逻辑
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <QVector>
#include <QPoint>
#include "Direction.h"

namespace SnakeGame {

/**
 * @brief 蛇类 - 管理蛇的身体坐标和移动行为
 * 
 * 职责：
 * - 存储蛇身坐标
 * - 处理蛇的移动和生长
 * - 管理移动方向（含反向校验）
 */
class Snake {
public:
    /**
     * @brief 构造函数
     * @param startPos 蛇头初始位置
     * @param initialLength 初始长度
     * @param initialDirection 初始移动方向
     */
    Snake(const QPoint& startPos = QPoint(10, 7),
          int initialLength = 3,
          Direction initialDirection = Direction::Right);

    /**
     * @brief 移动蛇（不增长）
     * 蛇头向当前方向移动一格，蛇尾移除
     */
    void move();

    /**
     * @brief 移动蛇并增长一节
     * 蛇头向当前方向移动一格，蛇尾保留
     */
    void grow();

    /**
     * @brief 设置移动方向
     * @param newDirection 新方向
     * @return true 设置成功，false 表示试图反向移动被拒绝
     */
    bool setDirection(Direction newDirection);

    /**
     * @brief 获取蛇头位置
     * @return 蛇头坐标
     */
    QPoint getHead() const;

    /**
     * @brief 获取蛇身坐标列表
     * @return 蛇身坐标（body_[0] 为蛇头）
     */
    const QVector<QPoint>& getBody() const;

    /**
     * @brief 获取当前移动方向
     * @return 当前方向
     */
    Direction getDirection() const;

    /**
     * @brief 获取蛇的长度
     * @return 蛇身节数
     */
    int getLength() const;

    /**
     * @brief 重置蛇到初始状态
     * @param startPos 蛇头初始位置
     * @param initialLength 初始长度
     * @param initialDirection 初始方向
     */
    void reset(const QPoint& startPos = QPoint(10, 7),
               int initialLength = 3,
               Direction initialDirection = Direction::Right);

private:
    QVector<QPoint> body_;          ///< 蛇身坐标，body_[0] 为蛇头
    Direction currentDirection_;    ///< 当前移动方向

    /**
     * @brief 计算下一个蛇头位置
     * @return 新蛇头坐标
     */
    QPoint calculateNextHead() const;
};

}  // namespace SnakeGame

#endif  // SNAKE_H
