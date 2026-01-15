/**
 * @file Snake.cpp
 * @brief 蛇类实现文件
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "Snake.h"
#include <QDebug>

namespace SnakeGame {

Snake::Snake(const QPoint& startPos, int initialLength, Direction initialDirection)
    : currentDirection_(initialDirection)
{
    reset(startPos, initialLength, initialDirection);
}

void Snake::move()
{
    if (body_.isEmpty()) {
        qWarning() << "Snake::move() called on empty snake";
        return;
    }

    // 计算新蛇头位置
    QPoint newHead = calculateNextHead();

    // 在头部插入新位置
    body_.prepend(newHead);

    // 移除尾部
    body_.removeLast();
}

void Snake::grow()
{
    if (body_.isEmpty()) {
        qWarning() << "Snake::grow() called on empty snake";
        return;
    }

    // 计算新蛇头位置
    QPoint newHead = calculateNextHead();

    // 在头部插入新位置，不移除尾部
    body_.prepend(newHead);
}

bool Snake::setDirection(Direction newDirection)
{
    // 防御性校验：禁止反向移动
    if (DirectionHelper::isOpposite(currentDirection_, newDirection)) {
        qWarning() << "Attempted reverse direction, ignoring";
        return false;
    }

    currentDirection_ = newDirection;
    return true;
}

QPoint Snake::getHead() const
{
    if (body_.isEmpty()) {
        qWarning() << "Snake::getHead() called on empty snake";
        return QPoint(-1, -1);
    }
    return body_.first();
}

const QVector<QPoint>& Snake::getBody() const
{
    return body_;
}

Direction Snake::getDirection() const
{
    return currentDirection_;
}

int Snake::getLength() const
{
    return body_.size();
}

void Snake::reset(const QPoint& startPos, int initialLength, Direction initialDirection)
{
    body_.clear();
    currentDirection_ = initialDirection;

    // 根据初始方向生成蛇身
    // 蛇头在 startPos，身体向相反方向延伸
    QPoint offset = DirectionHelper::toOffset(initialDirection);
    QPoint reverseOffset(-offset.x(), -offset.y());

    for (int i = 0; i < initialLength; ++i) {
        body_.append(startPos + reverseOffset * i);
    }
}

QPoint Snake::calculateNextHead() const
{
    QPoint offset = DirectionHelper::toOffset(currentDirection_);
    return getHead() + offset;
}

}  // namespace SnakeGame
