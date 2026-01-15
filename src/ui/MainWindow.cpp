/**
 * @file MainWindow.cpp
 * @brief 主窗口类实现
 * @author Snake Game Team
 * @date 2026-01-15
 */

#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QFont>
#include <QFrame>

namespace SnakeGame {

MainWindow::MainWindow(RendererType rendererType, QWidget* parent)
    : QMainWindow(parent)
    , gameLogic_(std::make_unique<GameLogic>())
    , rendererType_(rendererType)
    , gameWidget_(nullptr)
    , sceneView_(nullptr)
    , scoreLabel_(nullptr)
    , statusLabel_(nullptr)
{
    setupUI();
    connectSignals();

    // 初始化显示
    gameLogic_->resetGame();
}

MainWindow::~MainWindow()
{
    // gameLogic_ 通过 unique_ptr 自动销毁
    // gameWidget_, scoreLabel_, statusLabel_ 通过 Qt 父子对象机制销毁
}

void MainWindow::setupUI()
{
    // 设置窗口标题和属性
    setWindowTitle(tr("贪吃蛇游戏 - Snake Game"));
    setStyleSheet("background-color: #1e1e28;");

    // 创建中心部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // ==================== 顶部信息栏 ====================
    QHBoxLayout* infoLayout = new QHBoxLayout();

    // 分数显示
    scoreLabel_ = new QLabel(tr("分数: 0"), this);
    scoreLabel_->setStyleSheet(
        "QLabel {"
        "   color: #4CAF50;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "   padding: 10px 20px;"
        "   background-color: #2d2d3a;"
        "   border-radius: 8px;"
        "}"
    );

    // 状态显示
    statusLabel_ = new QLabel(tr("状态: 准备开始"), this);
    statusLabel_->setStyleSheet(
        "QLabel {"
        "   color: #FFD700;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   padding: 10px 20px;"
        "   background-color: #2d2d3a;"
        "   border-radius: 8px;"
        "}"
    );

    infoLayout->addWidget(scoreLabel_);
    infoLayout->addStretch();
    infoLayout->addWidget(statusLabel_);

    mainLayout->addLayout(infoLayout);

    // ==================== 游戏区域 ====================
    // 根据渲染器类型创建不同的前端组件
    QWidget* gameComponent = nullptr;
    
    if (rendererType_ == RendererType::Scene) {
        // 使用 QGraphicsScene 渲染器
        sceneView_ = new SceneGameView(
            gameLogic_->getBoardWidth(),
            gameLogic_->getBoardHeight(),
            Constants::kCellSize,
            this
        );
        gameComponent = sceneView_;
    } else {
        // 使用 QPainter 渲染器（默认）
        gameWidget_ = new GameWidget(
            gameLogic_->getBoardWidth(),
            gameLogic_->getBoardHeight(),
            Constants::kCellSize,
            this
        );
        gameComponent = gameWidget_;
    }

    // 将游戏区域居中
    QHBoxLayout* gameLayout = new QHBoxLayout();
    gameLayout->addStretch();
    gameLayout->addWidget(gameComponent);
    gameLayout->addStretch();

    mainLayout->addLayout(gameLayout);

    // ==================== 底部操作提示 ====================
    QLabel* helpLabel = new QLabel(
        tr("操作说明: ↑↓←→ 或 WASD 控制方向 | 空格 开始/重新开始 | P 暂停"),
        this
    );
    helpLabel->setStyleSheet(
        "QLabel {"
        "   color: #888;"
        "   font-size: 14px;"
        "   padding: 10px;"
        "}"
    );
    helpLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(helpLabel);

    // 设置窗口大小
    adjustSize();
    setFixedSize(size());
}

void MainWindow::connectSignals()
{
    // 后端 → 前端渲染
    if (rendererType_ == RendererType::Scene && sceneView_) {
        connect(gameLogic_.get(), &GameLogic::snakeMoved,
                sceneView_, &SceneGameView::onSnakeMoved);

        connect(gameLogic_.get(), &GameLogic::foodSpawned,
                sceneView_, &SceneGameView::onFoodSpawned);

        connect(gameLogic_.get(), &GameLogic::gameStateChanged,
                sceneView_, &SceneGameView::onGameStateChanged);
    } else if (gameWidget_) {
        connect(gameLogic_.get(), &GameLogic::snakeMoved,
                gameWidget_, &GameWidget::onSnakeMoved);

        connect(gameLogic_.get(), &GameLogic::foodSpawned,
                gameWidget_, &GameWidget::onFoodSpawned);

        connect(gameLogic_.get(), &GameLogic::gameStateChanged,
                gameWidget_, &GameWidget::onGameStateChanged);
    }

    // 后端 → 主窗口
    connect(gameLogic_.get(), &GameLogic::scoreChanged,
            this, &MainWindow::onScoreChanged);

    connect(gameLogic_.get(), &GameLogic::gameStateChanged,
            this, &MainWindow::onGameStateChanged);

    connect(gameLogic_.get(), &GameLogic::gameOver,
            this, &MainWindow::onGameOver);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        // 方向键
        case Qt::Key_Up:
        case Qt::Key_W:
            gameLogic_->setDirection(Direction::Up);
            break;

        case Qt::Key_Down:
        case Qt::Key_S:
            gameLogic_->setDirection(Direction::Down);
            break;

        case Qt::Key_Left:
        case Qt::Key_A:
            gameLogic_->setDirection(Direction::Left);
            break;

        case Qt::Key_Right:
        case Qt::Key_D:
            gameLogic_->setDirection(Direction::Right);
            break;

        // 开始/重新开始
        case Qt::Key_Space:
        case Qt::Key_Return:
            if (gameLogic_->getState() == GameState::Ready ||
                gameLogic_->getState() == GameState::GameOver) {
                gameLogic_->startGame();
            }
            break;

        // 暂停/继续
        case Qt::Key_P:
        case Qt::Key_Escape:
            if (gameLogic_->getState() == GameState::Running) {
                gameLogic_->pauseGame();
            } else if (gameLogic_->getState() == GameState::Paused) {
                gameLogic_->resumeGame();
            }
            break;

        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onScoreChanged(int score)
{
    scoreLabel_->setText(tr("分数: %1").arg(score));
}

void MainWindow::onGameStateChanged(GameState state)
{
    QString statusText;

    switch (state) {
        case GameState::Ready:
            statusText = tr("状态: 准备开始");
            statusLabel_->setStyleSheet(
                "QLabel { color: #FFD700; font-size: 18px; font-weight: bold; "
                "padding: 10px 20px; background-color: #2d2d3a; border-radius: 8px; }"
            );
            break;

        case GameState::Running:
            statusText = tr("状态: 游戏中");
            statusLabel_->setStyleSheet(
                "QLabel { color: #4CAF50; font-size: 18px; font-weight: bold; "
                "padding: 10px 20px; background-color: #2d2d3a; border-radius: 8px; }"
            );
            break;

        case GameState::Paused:
            statusText = tr("状态: 已暂停");
            statusLabel_->setStyleSheet(
                "QLabel { color: #FF9800; font-size: 18px; font-weight: bold; "
                "padding: 10px 20px; background-color: #2d2d3a; border-radius: 8px; }"
            );
            break;

        case GameState::GameOver:
            statusText = tr("状态: 游戏结束");
            statusLabel_->setStyleSheet(
                "QLabel { color: #F44336; font-size: 18px; font-weight: bold; "
                "padding: 10px 20px; background-color: #2d2d3a; border-radius: 8px; }"
            );
            break;
    }

    statusLabel_->setText(statusText);
}

void MainWindow::onGameOver(int finalScore)
{
    Q_UNUSED(finalScore);
    // 可以在这里添加游戏结束的额外处理，如显示对话框
}

}  // namespace SnakeGame
