/**
 * @file RendererType.h
 * @brief 渲染器类型枚举定义
 * @author Snake Game Team
 * @date 2026-01-15
 */

#ifndef RENDERERTYPE_H
#define RENDERERTYPE_H

namespace SnakeGame {

/**
 * @brief 渲染器类型枚举
 * 
 * 用于在程序启动时选择前端渲染方式
 */
enum class RendererType {
    Widget,  ///< QPainter 方式（默认）
    Scene    ///< QGraphicsScene 方式
};

}  // namespace SnakeGame

#endif  // RENDERERTYPE_H
