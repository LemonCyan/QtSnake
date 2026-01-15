/**
 * @file GameLogic.cpp
 * @brief 游戏逻辑类实现文件
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "GameLogic.h"
#include <QDebug>

namespace SnakeGame {

GameLogic::GameLogic(int boardWidth, int boardHeight, QObject* parent)
    : QObject(parent)
    , snake_(std::make_unique<Snake>())
    , food_(std::make_unique<Food>(boardWidth, boardHeight))
    , gameTimer_(new QTimer(this))  // 使用 Qt 父子对象机制管理内存
    , state_(GameState::Ready)
    , score_(0)
    , boardWidth_(boardWidth)
    , boardHeight_(boardHeight)
{
    // 连接定时器信号到游戏循环槽函数
    connect(gameTimer_, &QTimer::timeout, this, &GameLogic::onGameTick);

    // 设置定时器间隔
    gameTimer_->setInterval(Constants::kGameTickInterval);
}

GameLogic::~GameLogic()
{
    // QTimer 通过 Qt 父子对象机制自动销毁
    // snake_ 和 food_ 通过 unique_ptr 自动销毁
}

// ==================== 游戏控制 ====================

void GameLogic::startGame()
{
    if (state_ == GameState::Ready || state_ == GameState::GameOver) {
        // 重置游戏状态
        resetGame();
    }

    if (state_ == GameState::Ready) {
        setState(GameState::Running);
        gameTimer_->start();

        // 发送初始状态
        emit snakeMoved(snake_->getBody());
        emit foodSpawned(food_->getPosition());
        emit scoreChanged(score_);
    }
}

void GameLogic::pauseGame()
{
    if (state_ == GameState::Running) {
        gameTimer_->stop();
        setState(GameState::Paused);
    }
}

void GameLogic::resumeGame()
{
    if (state_ == GameState::Paused) {
        setState(GameState::Running);
        gameTimer_->start();
    }
}

void GameLogic::resetGame()
{
    // 停止定时器
    gameTimer_->stop();

    // 重置蛇
    QPoint startPos(boardWidth_ / 2, boardHeight_ / 2);
    snake_->reset(startPos, Constants::kInitialSnakeLength, Direction::Right);

    // 重置食物
    food_->reset(boardWidth_, boardHeight_);
    spawnFood();

    // 重置分数
    score_ = 0;

    // 重置状态
    setState(GameState::Ready);

    // 发送重置后的状态
    emit snakeMoved(snake_->getBody());
    emit foodSpawned(food_->getPosition());
    emit scoreChanged(score_);
}

// ==================== 输入处理 ====================

void GameLogic::setDirection(Direction direction)
{
    if (state_ == GameState::Running) {
        snake_->setDirection(direction);
    }
}

// ==================== 状态查询 ====================

GameState GameLogic::getState() const
{
    return state_;
}

int GameLogic::getScore() const
{
    return score_;
}

QVector<QPoint> GameLogic::getSnakeBody() const
{
    return snake_->getBody();
}

QPoint GameLogic::getFoodPosition() const
{
    return food_->getPosition();
}

int GameLogic::getBoardWidth() const
{
    return boardWidth_;
}

int GameLogic::getBoardHeight() const
{
    return boardHeight_;
}

// ==================== 私有槽函数 ====================

void GameLogic::onGameTick()
{
    if (state_ != GameState::Running) {
        return;
    }

    // 获取蛇头当前位置
    QPoint currentHead = snake_->getHead();

    // 计算下一个蛇头位置
    QPoint offset = DirectionHelper::toOffset(snake_->getDirection());
    QPoint nextHead = currentHead + offset;

    // 检查墙壁碰撞
    if (checkWallCollision(nextHead)) {
        handleGameOver();
        return;
    }

    // 检查自身碰撞（需要在移动后检查，但这里先预判）
    // 注意：移动后蛇尾会消失，所以如果下一步是当前蛇尾位置，不算碰撞
    // 为简化逻辑，先移动再检查
    bool ateFood = checkFoodCollision(nextHead);

    if (ateFood) {
        // 吃到食物，蛇增长
        snake_->grow();
        
        // 增加分数
        score_ += Constants::kScorePerFood;
        emit scoreChanged(score_);

        // 生成新食物
        spawnFood();
    } else {
        // 正常移动
        snake_->move();
    }

    // 检查自身碰撞（移动后检查）
    if (checkSelfCollision(snake_->getHead())) {
        handleGameOver();
        return;
    }

    // 发送蛇移动信号
    emit snakeMoved(snake_->getBody());
}

// ==================== 私有方法 ====================

bool GameLogic::checkWallCollision(const QPoint& head) const
{
    return head.x() < 0 || head.x() >= boardWidth_ ||
           head.y() < 0 || head.y() >= boardHeight_;
}

bool GameLogic::checkSelfCollision(const QPoint& head) const
{
    const QVector<QPoint>& body = snake_->getBody();
    
    // 从索引 1 开始检查，跳过蛇头本身，轮询检查复杂度高，可优化
    for (int i = 1; i < body.size(); ++i) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

bool GameLogic::checkFoodCollision(const QPoint& head) const
{
    return head == food_->getPosition();
}

void GameLogic::handleGameOver()
{
    gameTimer_->stop();
    setState(GameState::GameOver);
    emit gameOver(score_);
}

void GameLogic::spawnFood()
{
    bool success = food_->respawn(snake_->getBody());
    
    if (success) {
        emit foodSpawned(food_->getPosition());
    } else {
        // 没有可用位置，玩家获胜（蛇填满整个游戏区域）
        qDebug() << "Player wins! Snake filled the entire board.";
        handleGameOver();
    }
}

void GameLogic::setState(GameState newState)
{
    if (state_ != newState) {
        state_ = newState;
        emit gameStateChanged(state_);
    }
}

}  // namespace SnakeGame
