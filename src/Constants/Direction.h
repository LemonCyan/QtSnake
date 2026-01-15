/**
 * @file Direction.h
 * @brief 移动方向枚举及工具函数
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef DIRECTION_H
#define DIRECTION_H

#include <QPoint>

namespace SnakeGame {

/**
 * @brief 蛇的移动方向
 */
enum class Direction {
    Up,     ///< 向上移动
    Down,   ///< 向下移动
    Left,   ///< 向左移动
    Right   ///< 向右移动
};

/**
 * @brief 方向工具类
 */
class DirectionHelper {
public:
    /**
     * @brief 检查两个方向是否相反
     * @param dir1 第一个方向
     * @param dir2 第二个方向
     * @return true 表示方向相反，false 表示不相反
     */
    static bool isOpposite(Direction dir1, Direction dir2) {
        return (dir1 == Direction::Up && dir2 == Direction::Down) ||
               (dir1 == Direction::Down && dir2 == Direction::Up) ||
               (dir1 == Direction::Left && dir2 == Direction::Right) ||
               (dir1 == Direction::Right && dir2 == Direction::Left);
    }

    /**
     * @brief 获取方向对应的位移向量
     * @param dir 移动方向
     * @return 对应的坐标偏移量
     */
    static QPoint toOffset(Direction dir) {
        switch (dir) {
            case Direction::Up:    return QPoint(0, -1);
            case Direction::Down:  return QPoint(0, 1);
            case Direction::Left:  return QPoint(-1, 0);
            case Direction::Right: return QPoint(1, 0);
            default:               return QPoint(0, 0);
        }
    }
};

}  // namespace SnakeGame

#endif  // DIRECTION_H
