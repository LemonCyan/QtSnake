/**
 * @file Food.h
 * @brief 食物类头文件 - 管理食物位置和生成逻辑
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef FOOD_H
#define FOOD_H

#include <QPoint>
#include <QVector>
#include <functional>

namespace SnakeGame {

/**
 * @brief 食物类 - 管理食物的位置和重新生成
 * 
 * 职责：
 * - 存储食物当前位置
 * - 在空白区域随机生成新食物
 */
class Food {
public:
    /**
     * @brief 随机数生成器类型
     * @param min 最小值（含）
     * @param max 最大值（含）
     * @return 生成的随机数
     */
    using RandomGenerator = std::function<int(int min, int max)>;

    /**
     * @brief 构造函数
     * @param boardWidth 游戏区域宽度（格数）
     * @param boardHeight 游戏区域高度（格数）
     */
    Food(int boardWidth = 20, int boardHeight = 15);

    /**
     * @brief 获取食物当前位置
     * @return 食物坐标
     */
    QPoint getPosition() const;

    /**
     * @brief 在排除指定位置后重新生成食物
     * @param excludePositions 需要排除的位置（如蛇身）
     * @return true 生成成功，false 表示没有可用位置
     */
    bool respawn(const QVector<QPoint>& excludePositions);

    /**
     * @brief 设置随机数生成器（用于测试）
     * @param generator 自定义随机数生成器
     */
    void setRandomGenerator(RandomGenerator generator);

    /**
     * @brief 重置食物状态
     * @param boardWidth 新的游戏区域宽度
     * @param boardHeight 新的游戏区域高度
     */
    void reset(int boardWidth, int boardHeight);

private:
    QPoint position_;           ///< 食物当前位置
    int boardWidth_;            ///< 游戏区域宽度
    int boardHeight_;           ///< 游戏区域高度
    RandomGenerator randomGenerator_;   ///< 随机数生成器

    /**
     * @brief 获取所有可用位置
     * @param excludePositions 需要排除的位置
     * @return 可用位置列表
     */
    QVector<QPoint> getAvailablePositions(const QVector<QPoint>& excludePositions) const;
};

}  // namespace SnakeGame

#endif  // FOOD_H
