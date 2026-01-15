/**
 * @file Constants.h
 * @brief 游戏常量定义
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace SnakeGame {

/**
 * @brief 游戏常量命名空间
 */
namespace Constants {

    /** @brief 默认游戏区域宽度（格数） */
    constexpr int kDefaultBoardWidth = 20;

    /** @brief 默认游戏区域高度（格数） */
    constexpr int kDefaultBoardHeight = 15;

    /** @brief 初始蛇长度 */
    constexpr int kInitialSnakeLength = 3;

    /** @brief 游戏刷新间隔（毫秒） */
    constexpr int kGameTickInterval = 200;

    /** @brief 每个食物得分 */
    constexpr int kScorePerFood = 10;

    /** @brief 单元格像素大小（用于渲染，后端可选） */
    constexpr int kCellSize = 30;

}  // namespace Constants

}  // namespace SnakeGame

#endif  // CONSTANTS_H
