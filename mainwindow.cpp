#include "mainwindow.h"
#include "tile.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QGridLayout>
#include <QFrame>
#include <QTextLayout>
#include <QMessageBox>
#include <QRandomGenerator>

int flagCount;

/**
 * @brief MainWindow::MainWindow - constructor for the main window; initializes grid layout.
 * @param parent - widget containing the window.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), NUM_MINES(15), NUM_ROWS(16), NUM_COLS(30), flagCount(0), ui(new Ui::MainWindow) {
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
        this, "Restart Game", "You hit a mine! Do you want to restart?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        // Close the current window
        this->close();

        // Create a new MainWindow instance
        MainWindow *newGame = new MainWindow();
        newGame->show();
    }
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
 * @brief MainWindow::leftClick
 * @param row
 * @param col
 */
void MainWindow::leftClick(int row, int col) {

    Tile *tile = tiles[row][col];

    if (tile->isFlagged()) { // Flagged tile
        return;

    } else if (tile->isMine()) { // Tile is a mine

        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {

                tiles[i][j]->setEnabled(false);

                if (tiles[i][j]->isMine())
                    tiles[i][j]->setText("💣");
            }
        }

        restartGame();

    } else if (tile->getMineCount() > 0) { // Tile touching a mine
        tile->setText(QString::number(tile->getMineCount()));
        tile->setRevealed(true);

    } else { // Tile not touching a mine
        tile->sweep(row, col);
    }
}

/**
 * @brief MainWindow::rightClick
 * @param row
 * @param col
 */
void MainWindow::rightClick(int row, int col) {

    if (tiles[row][col]->isRevealed()) return; // Do nothing if tile is already revealed

    Tile *tile = tiles[row][col];
    tile->setFlag(!tile->isFlagged());

    if (tile->isFlagged()) {

        tile->setText("🚩");
        flagCount++;

        if (tile->isMine() && flagCount >= NUM_MINES) {
            QMessageBox::information(this, "GAME WIN", "Congratulations! You didn't die! 🎉");

            for (int i = 0; i < NUM_ROWS; ++i) {
                for (int j = 0; j < NUM_COLS; ++j) {
                    tiles[i][j]->setEnabled(false);
                }
            }
        }

    } else {
        tile->setText("");
        flagCount--;
    }
}

/**
 * @brief getTiles returns the tiles 2D Vector containing all of the tiles on the grid.
 * @return tiles
 */
QVector<QVector<Tile *>> &MainWindow::getTiles() {
    return tiles;
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


