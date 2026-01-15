/**
 * @file Food.cpp
 * @brief 食物类实现文件
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "Food.h"
#include <QRandomGenerator>
#include <QDebug>

namespace SnakeGame {

Food::Food(int boardWidth, int boardHeight)
    : position_(-1, -1)
    , boardWidth_(boardWidth)
    , boardHeight_(boardHeight)
{
    // 默认随机数生成器使用 Qt 的全局随机数生成器
    randomGenerator_ = [](int min, int max) {
        return QRandomGenerator::global()->bounded(min, max + 1);
    };
}

QPoint Food::getPosition() const
{
    return position_;
}

bool Food::respawn(const QVector<QPoint>& excludePositions)
{
    QVector<QPoint> availablePositions = getAvailablePositions(excludePositions);

    if (availablePositions.isEmpty()) {
        qWarning() << "Food::respawn() - No available positions";
        return false;
    }

    // 随机选择一个可用位置
    int index = randomGenerator_(0, availablePositions.size() - 1);
    position_ = availablePositions[index];

    return true;
}

void Food::setRandomGenerator(RandomGenerator generator)
{
    if (generator) {
        randomGenerator_ = std::move(generator);
    }
}

void Food::reset(int boardWidth, int boardHeight)
{
    boardWidth_ = boardWidth;
    boardHeight_ = boardHeight;
    position_ = QPoint(-1, -1);
}

QVector<QPoint> Food::getAvailablePositions(const QVector<QPoint>& excludePositions) const
{
    QVector<QPoint> available;
    available.reserve(boardWidth_ * boardHeight_ - excludePositions.size());

    for (int x = 0; x < boardWidth_; ++x) {
        for (int y = 0; y < boardHeight_; ++y) {
            QPoint pos(x, y);
            if (!excludePositions.contains(pos)) {
                available.append(pos);
            }
        }
    }

    return available;
}

}  // namespace SnakeGame
