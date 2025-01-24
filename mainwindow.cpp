#include "mainwindow.h"
#include "tile.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QRandomGenerator>

/**
 * @brief MainWindow::MainWindow - constructor for the main window; initializes grid layout.
 * @param parent - widget containing the window.
 */
MainWindow::MainWindow(QWidget *parent) // Adjust NUM_MINES here to adjust game difficulty.
    : QMainWindow(parent), NUM_MINES(99), NUM_ROWS(16), NUM_COLS(30), clearCount(0), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    tiles.resize(NUM_ROWS);
    for (int i = 0; i < NUM_ROWS; ++i)
        tiles[i].resize(NUM_COLS);

    createGrid();
}

/**
 * @brief MainWindow::~MainWindow - destructor.
 */
MainWindow::~MainWindow() {
    delete ui;
}

/**
 * @brief MainWindow::createGrid - initializes the minefield, creating tiles, filling the tiles vector, and adding listeners to buttons.
 *
 */
void MainWindow::createGrid() {

    QFrame *frame = new QFrame(this);
    QGridLayout *layout = new QGridLayout(frame);
    layout->setSpacing(2);

    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < NUM_COLS; ++j) {

            Tile *tile = new Tile(this, i, j);
            layout->addWidget(tile, i, j); // Add the Tile directly (no button member)

            tiles[i][j] = tile;

            // Connect the tileClicked signal to MainWindow handleTileClick slot
            connect(tile, &Tile::tileClicked, this, &MainWindow::handleTileClick);
        }
    }

    setMines();
    setCentralWidget(frame);
}

/**
 * @name restartGame
 * @brief Restarts the game by recreating the MainWindow.
 */
void MainWindow::restartGame() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Restart Game", "Do you want to restart?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Close the current window
        this->close();

        // Create a new MainWindow instance
        MainWindow *newGame = new MainWindow();
        newGame->show();
    } else
        this->close();
}

/**
 * @brief MainWindow::setMines - randomly sets mines across the grid.
 */
void MainWindow::setMines() {

    for (int i = 0; i < NUM_MINES; ++i) {
        int rRow = QRandomGenerator::global()->bounded(0, NUM_ROWS-1);
        int rCol = QRandomGenerator::global()->bounded(0, NUM_COLS-1);

        while (tiles[rRow][rCol]->isMine()) {
            rRow = QRandomGenerator::global()->bounded(0, NUM_ROWS-1);
            rCol = QRandomGenerator::global()->bounded(0, NUM_COLS-1);
        }

        tiles[rRow][rCol]->setMine(true);
        // tiles[rRow][rCol]->setText("💣"); // FOR DEBUGGING

        for (int j = -1; j <= 1; ++ j) {
            for (int k = -1; k <= 1; ++k) {

                if (j == 0 && k == 0)
                    continue;

                else if (rRow+j < 0 || rCol+k < 0 || rRow+j >= NUM_ROWS || rCol+k >= NUM_COLS)
                    continue;

                else {
                    if (!tiles[rRow+j][rCol+k]->isMine())
                        tiles[rRow+j][rCol+k]->setMineCount(tiles[rRow+j][rCol+k]->getMineCount()+1);
                }
            }
        }

    }
}

/**
 * @brief MainWindow::handleTileClick - performs a set of actions based off whether the click was a left or right click.
 * @param row - row of click.
 * @param col - column of click.
 * @param button - button that was clicked.
 */
void MainWindow::handleTileClick(int row, int col, Qt::MouseButton button) {

    if (tiles[row][col]->isRevealed()) return;
    else if (button == Qt::LeftButton) leftClick(row, col);
    else if (button == Qt::RightButton) rightClick(row, col);
}

/**
 * @brief getTiles returns the tiles 2D Vector containing all of the tiles on the grid.
 * @return tiles
 */
QVector<QVector<Tile *>> &MainWindow::getTiles() {
    return tiles;
}

/**
 * @name getClearCount
 * @brief MainWindow::getClearCount
 * @return
 */
int MainWindow::getClearCount() {
    return this->clearCount;
}

/**
 * @name setClearCount
 * @brief MainWindow::setClearCount
 */
void MainWindow::setClearCount(int c) {
    this->clearCount = c;
}
