/**
 * @file SceneGameView.cpp
 * @brief QGraphicsScene 游戏渲染视图实现
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "SceneGameView.h"
#include <QBrush>
#include <QPen>
#include <QFont>

namespace SnakeGame {

SceneGameView::SceneGameView(int boardWidth, int boardHeight, int cellSize, QWidget* parent)
    : QGraphicsView(parent)
    , boardWidth_(boardWidth)
    , boardHeight_(boardHeight)
    , cellSize_(cellSize)
    , scene_(nullptr)
    , gameState_(GameState::Ready)
    , foodItem_(nullptr)
    , overlayItem_(nullptr)
    , overlayText_(nullptr)
{
    setupScene();
}

SceneGameView::~SceneGameView()
{
    // scene_ 和其中的 items 会通过 Qt 父子机制自动销毁
}

void SceneGameView::setupScene()
{
    // 创建场景
    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, boardWidth_ * cellSize_, boardHeight_ * cellSize_);
    setScene(scene_);

    // 设置视图属性
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(boardWidth_ * cellSize_ + 2, boardHeight_ * cellSize_ + 2);
    setStyleSheet("border: 1px solid #3d3d50;");

    // 绘制背景
    drawBackground();

    // 创建食物项（初始隐藏）
    foodItem_ = new QGraphicsEllipseItem();
    foodItem_->setBrush(QBrush(QColor("#FF5722")));
    foodItem_->setPen(Qt::NoPen);
    foodItem_->setVisible(false);
    scene_->addItem(foodItem_);

    // 创建覆盖层
    overlayItem_ = new QGraphicsRectItem(0, 0, boardWidth_ * cellSize_, boardHeight_ * cellSize_);
    overlayItem_->setBrush(QBrush(QColor(0, 0, 0, 180)));
    overlayItem_->setPen(Qt::NoPen);
    overlayItem_->setZValue(100);  // 确保在最上层
    overlayItem_->setVisible(false);
    scene_->addItem(overlayItem_);

    // 创建覆盖层文字
    overlayText_ = new QGraphicsTextItem();
    overlayText_->setDefaultTextColor(Qt::white);
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    overlayText_->setFont(font);
    overlayText_->setZValue(101);
    overlayText_->setVisible(false);
    scene_->addItem(overlayText_);
}

void SceneGameView::drawBackground()
{
    // 填充背景色
    scene_->setBackgroundBrush(QBrush(QColor("#1e1e28")));

    // 绘制网格线
    QPen gridPen(QColor("#2d2d3a"));
    gridPen.setWidth(1);

    // 垂直线
    for (int x = 0; x <= boardWidth_; ++x) {
        scene_->addLine(x * cellSize_, 0, x * cellSize_, boardHeight_ * cellSize_, gridPen);
    }

    // 水平线
    for (int y = 0; y <= boardHeight_; ++y) {
        scene_->addLine(0, y * cellSize_, boardWidth_ * cellSize_, y * cellSize_, gridPen);
    }
}

void SceneGameView::onSnakeMoved(const QVector<QPoint>& body)
{
    updateSnakeItems(body);
}

void SceneGameView::onFoodSpawned(const QPoint& position)
{
    if (!foodItem_) {
        return;
    }

    QRectF rect = gridToScene(position);
    // 食物使用圆形，稍微缩小以区分
    qreal margin = cellSize_ * 0.15;
    foodItem_->setRect(rect.adjusted(margin, margin, -margin, -margin));
    foodItem_->setVisible(true);
}

void SceneGameView::onGameStateChanged(GameState state)
{
    gameState_ = state;
    updateOverlay();
}

void SceneGameView::updateSnakeItems(const QVector<QPoint>& body)
{
    // 移除多余的蛇身项
    while (snakeItems_.size() > body.size()) {
        QGraphicsRectItem* item = snakeItems_.takeLast();
        scene_->removeItem(item);
        delete item;
    }

    // 添加缺少的蛇身项
    while (snakeItems_.size() < body.size()) {
        QGraphicsRectItem* item = new QGraphicsRectItem();
        item->setPen(Qt::NoPen);
        item->setZValue(10);  // 蛇在网格之上
        scene_->addItem(item);
        snakeItems_.append(item);
    }

    // 更新所有蛇身项的位置和颜色
    for (int i = 0; i < body.size(); ++i) {
        QRectF rect = gridToScene(body[i]);
        snakeItems_[i]->setRect(rect);

        // 蛇头使用亮绿色，身体使用深绿色
        if (i == 0) {
            snakeItems_[i]->setBrush(QBrush(QColor("#4CAF50")));
        } else {
            snakeItems_[i]->setBrush(QBrush(QColor("#388E3C")));
        }
    }
}

void SceneGameView::updateOverlay()
{
    QString text;
    bool showOverlay = false;

    switch (gameState_) {
        case GameState::Ready:
            text = tr("按空格键开始游戏");
            showOverlay = true;
            break;

        case GameState::Paused:
            text = tr("游戏暂停\n按 P 继续");
            showOverlay = true;
            break;

        case GameState::GameOver:
            text = tr("游戏结束\n按空格键重新开始");
            showOverlay = true;
            break;

        case GameState::Running:
            showOverlay = false;
            break;
    }

    if (overlayItem_ && overlayText_) {
        overlayItem_->setVisible(showOverlay);
        overlayText_->setVisible(showOverlay);

        if (showOverlay) {
            overlayText_->setPlainText(text);
            // 居中显示文字
            QRectF textRect = overlayText_->boundingRect();
            qreal x = (boardWidth_ * cellSize_ - textRect.width()) / 2;
            qreal y = (boardHeight_ * cellSize_ - textRect.height()) / 2;
            overlayText_->setPos(x, y);
        }
    }
}

QRectF SceneGameView::gridToScene(const QPoint& gridPos) const
{
    return QRectF(gridPos.x() * cellSize_, 
                  gridPos.y() * cellSize_, 
                  cellSize_, 
                  cellSize_);
}

}  // namespace SnakeGame
