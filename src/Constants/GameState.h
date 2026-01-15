/**
 * @file GameState.h
 * @brief 游戏状态枚举
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace SnakeGame {

/**
 * @brief 游戏运行状态
 */
enum class GameState {
    Ready,      ///< 等待开始
    Running,    ///< 游戏进行中
    Paused,     ///< 游戏暂停
    GameOver    ///< 游戏结束
};

}  // namespace SnakeGame

#endif  // GAMESTATE_H
