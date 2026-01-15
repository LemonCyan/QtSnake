/**
 * @file GameWidget.cpp
 * @brief 游戏渲染组件实现
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "GameWidget.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QFont>

namespace SnakeGame {

GameWidget::GameWidget(int boardWidth, int boardHeight, int cellSize, QWidget* parent)
    : QWidget(parent)
    , boardWidth_(boardWidth)
    , boardHeight_(boardHeight)
    , cellSize_(cellSize)
    , foodPosition_(-1, -1)
    , gameState_(GameState::Ready)
{
    // 设置固定大小
    setFixedSize(boardWidth_ * cellSize_, boardHeight_ * cellSize_);

    // 设置背景色
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(30, 30, 40));
    setPalette(pal);
}

void GameWidget::onSnakeMoved(const QVector<QPoint>& body)
{
    snakeBody_ = body;
    update();  // 触发重绘
}

void GameWidget::onFoodSpawned(const QPoint& position)
{
    foodPosition_ = position;
    update();  // 触发重绘
}

void GameWidget::onGameStateChanged(GameState state)
{
    gameState_ = state;
    update();  // 触发重绘
}

void GameWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制层次：背景 → 食物 → 蛇 → 覆盖层
    drawBackground(painter);
    drawFood(painter);
    drawSnake(painter);
    drawOverlay(painter);
}

void GameWidget::drawBackground(QPainter& painter)
{
    // 绘制网格线
    painter.setPen(QPen(QColor(50, 50, 60), 1));

    // 垂直线
    for (int x = 0; x <= boardWidth_; ++x) {
        painter.drawLine(x * cellSize_, 0, x * cellSize_, boardHeight_ * cellSize_);
    }

    // 水平线
    for (int y = 0; y <= boardHeight_; ++y) {
        painter.drawLine(0, y * cellSize_, boardWidth_ * cellSize_, y * cellSize_);
    }
}

void GameWidget::drawSnake(QPainter& painter)
{
    if (snakeBody_.isEmpty()) {
        return;
    }

    // 蛇身颜色渐变
    QColor headColor(76, 175, 80);      // 鲜艳绿色 - 蛇头
    QColor bodyColor(56, 142, 60);      // 深绿色 - 蛇身
    QColor tailColor(46, 125, 50);      // 更深绿色 - 蛇尾

    for (int i = 0; i < snakeBody_.size(); ++i) {
        QRect rect = gridToPixel(snakeBody_[i]);
        
        // 缩小一点，留出间隙
        rect.adjust(2, 2, -2, -2);

        QColor color;
        if (i == 0) {
            // 蛇头
            color = headColor;
        } else if (i == snakeBody_.size() - 1) {
            // 蛇尾
            color = tailColor;
        } else {
            // 蛇身 - 渐变
            double ratio = static_cast<double>(i) / snakeBody_.size();
            color = QColor(
                bodyColor.red() + static_cast<int>((tailColor.red() - bodyColor.red()) * ratio),
                bodyColor.green() + static_cast<int>((tailColor.green() - bodyColor.green()) * ratio),
                bodyColor.blue() + static_cast<int>((tailColor.blue() - bodyColor.blue()) * ratio)
            );
        }

        painter.setBrush(QBrush(color));
        painter.setPen(Qt::NoPen);

        if (i == 0) {
            // 蛇头绘制为圆角矩形
            painter.drawRoundedRect(rect, 8, 8);

            // 绘制眼睛
            painter.setBrush(QBrush(Qt::white));
            int eyeSize = cellSize_ / 6;
            int eyeOffset = cellSize_ / 4;
            painter.drawEllipse(rect.center() + QPoint(-eyeOffset/2, -eyeOffset/2), eyeSize, eyeSize);
            painter.drawEllipse(rect.center() + QPoint(eyeOffset/2, -eyeOffset/2), eyeSize, eyeSize);

            // 瞳孔
            painter.setBrush(QBrush(Qt::black));
            int pupilSize = eyeSize / 2;
            painter.drawEllipse(rect.center() + QPoint(-eyeOffset/2, -eyeOffset/2), pupilSize, pupilSize);
            painter.drawEllipse(rect.center() + QPoint(eyeOffset/2, -eyeOffset/2), pupilSize, pupilSize);
        } else {
            // 蛇身绘制为圆角矩形
            painter.drawRoundedRect(rect, 6, 6);
        }
    }
}

void GameWidget::drawFood(QPainter& painter)
{
    if (foodPosition_.x() < 0 || foodPosition_.y() < 0) {
        return;
    }

    QRect rect = gridToPixel(foodPosition_);
    rect.adjust(4, 4, -4, -4);

    // 食物绘制为红色圆形
    painter.setBrush(QBrush(QColor(244, 67, 54)));  // 红色
    painter.setPen(QPen(QColor(211, 47, 47), 2));   // 深红边框

    painter.drawEllipse(rect);

    // 添加高光效果
    painter.setBrush(QBrush(QColor(255, 255, 255, 100)));
    painter.setPen(Qt::NoPen);
    QRect highlight(rect.left() + rect.width() / 4, 
                    rect.top() + rect.height() / 4,
                    rect.width() / 3, 
                    rect.height() / 3);
    painter.drawEllipse(highlight);
}

void GameWidget::drawOverlay(QPainter& painter)
{
    QString text;
    QColor overlayColor(0, 0, 0, 150);

    switch (gameState_) {
        case GameState::Ready:
            text = tr("按 空格键 开始游戏");
            break;
        case GameState::Paused:
            text = tr("游戏暂停\n按 P 继续");
            overlayColor = QColor(0, 0, 0, 180);
            break;
        case GameState::GameOver:
            text = tr("游戏结束\n按 空格键 重新开始");
            overlayColor = QColor(0, 0, 0, 200);
            break;
        case GameState::Running:
            return;  // 运行中不显示覆盖层
    }

    // 半透明覆盖层
    painter.fillRect(rect(), overlayColor);

    // 文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);

    painter.drawText(rect(), Qt::AlignCenter, text);
}

QRect GameWidget::gridToPixel(const QPoint& gridPos) const
{
    return QRect(gridPos.x() * cellSize_,
                 gridPos.y() * cellSize_,
                 cellSize_,
                 cellSize_);
}

}  // namespace SnakeGame
