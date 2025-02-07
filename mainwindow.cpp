#include "mainwindow.h"
#include "tile.h"
#include "./ui_mainwindow.h"

/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class.
 *
 * This file contains the definitions of the methods in the MainWindow class,
 * including grid creation, mine placement, and handling user interactions.
 *
 * @author Thomas Llamzon | 251316111 | TLLAMZON
 * @date Feb 1, 2025
 */

/**
 * @name MainWindow
 * @brief Constructor for the MainWindow class.
 * Initializes the grid layout and sets up the UI.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    NUM_MINES(99),
    NUM_ROWS(16),
    NUM_COLS(30),
    ui(new Ui::MainWindow),
    clearCount(0) {

    ui->setupUi(this);

    // Resize the tiles 2D vector.
    tiles.resize(NUM_ROWS);
    for (int i = 0; i < NUM_ROWS; ++i)
        tiles[i].resize(NUM_COLS);

    createGrid();
}

/**
 * @name ~MainWindow
 * @brief Destructor for the MainWindow class.
 */
MainWindow::~MainWindow() {
    delete ui;
}

/**
 * @name createGrid
 * @brief Creates the game grid and initializes the tiles (calls setMines).
 */
void MainWindow::createGrid() {
    QFrame *frame = new QFrame(this);
    QGridLayout *layout = new QGridLayout(frame);
    layout->setSpacing(2);

    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < NUM_COLS; ++j) {

            Tile *tile = new Tile(this, i, j);

            // Lock tiles in their position in the layout.
            QSizePolicy lock = tile->sizePolicy();
            lock.setRetainSizeWhenHidden(true);
            tile->setSizePolicy(lock);

            layout->addWidget(tile, i, j); // Add Tile widget to layout.
            tiles[i][j] = tile;

            // Connect the tileClicked signal to the handleTileClick slot.
            connect(tile, &Tile::tileClicked, this, &MainWindow::handleTileClick);
        }
    }

    setMines();
    setCentralWidget(frame);
}

/**
 * @name setMines
 * @brief Sets mines randomly across the grid and updates neighboring mine counts.
 */
void MainWindow::setMines() {
    for (int i = 0; i < NUM_MINES; ++i) {
        int rRow = QRandomGenerator::global()->bounded(0, NUM_ROWS);
        int rCol = QRandomGenerator::global()->bounded(0, NUM_COLS);

        while (tiles[rRow][rCol]->isMine()) {
            rRow = QRandomGenerator::global()->bounded(0, NUM_ROWS);
            rCol = QRandomGenerator::global()->bounded(0, NUM_COLS);
        }

        tiles[rRow][rCol]->setMine(true);

        // Update neighboring tiles' mine counts.
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                int nRow = rRow + j;
                int nCol = rCol + k;

                if (j == 0 && k == 0) continue; // Skip the tile itself.
                if (nRow < 0 || nCol < 0 || nRow >= NUM_ROWS || nCol >= NUM_COLS) continue;

                if (!tiles[nRow][nCol]->isMine()) {
                    tiles[nRow][nCol]->setMineCount(tiles[nRow][nCol]->getMineCount() + 1);
                }
            }
        }
    }
}

/**
 * @name handleTileClick
 * @brief Handles tile click events and calls left/right click action.
 * @param row The row of the clicked tile.
 * @param col The column of the clicked tile.
 * @param button The mouse button used for the click.
 */
void MainWindow::handleTileClick(int row, int col, Qt::MouseButton button) {
    if (tiles[row][col]->isRevealed()) return;

    if (button == Qt::LeftButton) {
        leftClick(row, col);
    } else if (button == Qt::RightButton) {
        rightClick(row, col);
    }
}

/**
 * @name restartGame
 * @brief Restarts the game by closing the current window creating a new one.
 */
void MainWindow::restartGame() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Restart Game", "Do you want to restart?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->close();
        MainWindow *newGame = new MainWindow();
        newGame->show();
    } else {
        this->close();
    }
}

/**
 * @name getTiles
 * @brief Returns the grid of tiles.
 * @return Reference to the 2D vector of tiles.
 */
QVector<QVector<Tile *>> &MainWindow::getTiles() {
    return tiles;
}

/**
 * @name getClearCount
 * @brief Gets the current count of cleared tiles.
 * @return The cleared tile count.
 */
int MainWindow::getClearCount() const {
    return clearCount;
}

/**
 * @name getMINES
 * @brief Gets the number of mines on the grid.
 * @return The number of mines on the grid.
 */
int MainWindow::getMINES() const {
    return NUM_MINES;
}

/**
 * @name getROW
 * @brief Gets the number of rows on the grid.
 * @return The number of rows on the grid.
*/
int MainWindow::getROW() const {
    return NUM_ROWS;
}

/**
 * @name getCOL
 * @brief Gets the number of columns on the grid.
 * @return The number of columns on the grid.
 */
int MainWindow::getCOL() const {
    return NUM_COLS;
}

/**
 * @name setClearCount
 * @brief Sets the current count of cleared tiles.
 * @param c The new cleared tile count.
 */
void MainWindow::setClearCount(int c) {
    clearCount = c;
}
