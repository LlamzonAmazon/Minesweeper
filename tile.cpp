#include "tile.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QMessageBox>

MainWindow *parent;
int row;
int col;
int surrMines;
bool mine;
bool flagged;
bool believed;
bool revealed;
QVector<QVector<Tile *>> tiles;

/**
 * @name Tile
 * @brief Tile::Tile is a constructor for Tile objects.
 * @param par - parent (MainWindow)
 * @param r - int row coordinate
 * @param c - int column coordinate
 */
Tile::Tile(MainWindow *par, int r, int c)
    : parent(par), row(r), col(c), surrMines(0), mine(false), flagged(false), revealed(false) {
    setFixedSize(40, 40);
    tiles = parent->getTiles();
}

/**
 * @name sweep
 * @brief Tile::sweep "sweeps" the minefield; disables tiles not touching mines and revealing those that are.
 * @param row - int row coordinate
 * @param col - int column coordinate
 */
bool Tile::sweep(int row, int col) {

    QVector<QVector<Tile *>> &tiles = parent->getTiles();

    // Ignore revealed tiles and out of bounds indices
    if (row < 0 || col < 0 || row >= parent->NUM_ROWS || col >= parent->NUM_COLS) return false;
    if (tiles[row][col]->isRevealed()) return false;

    Tile *tile = tiles[row][col];

    parent->setClearCount(parent->getClearCount()+1); // Increment clearCount for every tile swept

    if (tile->getMineCount() == 0) { // Tile not touching a mine

        tile->setRevealed(true);
        tile->setEnabled(false);

        for (int r = -1; r <= 1; ++r) {
            for (int c = -1; c <= 1; ++c) {
                if (r == 0 && c == 0) continue;

                sweep(row+r, col+c);
            }
        }

    } else { // Tile touching a mine
        tile->setText(QString::number(tile->getMineCount()));
        tile->setRevealed(true);
    }

    // Always check after sweeping if all the tiles have been cleared
    if (parent->getClearCount() >= ((parent->NUM_ROWS * parent->NUM_COLS) - parent->NUM_MINES))
        return true;
    return false;
}

/**
 * @name leftClick
 * @brief MainWindow::leftClick handles action upon left-clicking a tile.
 * @param row
 * @param col
 *
 * This function is included in tile.cpp because the left-click action is primarily related to the properties of tiles.
 */
void MainWindow::leftClick(int row, int col) {

    Tile *tile = tiles[row][col];
    bool gameWin = false;

    if (tile->isMine()) {
        if (tile->isFlagged()) return;

        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {

                tiles[i][j]->setEnabled(false);

                if (tiles[i][j]->isMine())
                    tiles[i][j]->setText("💣");
            }
        }

        QMessageBox::information(this, "GAME LOSE", "Oh no! You hit a mine! 💣");
        restartGame();

    } else gameWin = tile->sweep(row, col);

    if (gameWin) {
        QMessageBox::information(this, "GAME WIN", "Congratulations! You didn't die! 🎉");

        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {
                tiles[i][j]->setEnabled(false);
            }
        }

        restartGame();
    }
}

/**
 * @name rightClick
 * @brief MainWindow::rightClick handles action upon right-clicking a tile.
 * @param row
 * @param col
 *
 * This function is included in tile.cpp because the right-click action is primarily related to the properties of tiles.
 */
void MainWindow::rightClick(int row, int col) {

    if (tiles[row][col]->isRevealed()) return; // Do nothing if tile is already revealed

    Tile *tile = tiles[row][col];

    if (!tile->isFlagged() && !tile->isBelieved()) { // Nothing —> Flag
        tile->setFlag(true);

    } else if (tile->isFlagged() && !tile->isBelieved()) { // Flag —> Question mark
        tile->setFlag(false);
        tile->setBelieved(true);

    } else if (!tile->isFlagged() && tile->isBelieved()) { // Question mark —> Nothing
        tile->setBelieved(false);
    }
}

/**
 * @name Tile::mousePressEvent
 * @brief Overrides the mousePressEvent to handle left and right clicks
 */
void Tile::mousePressEvent(QMouseEvent *event) {
    emit tileClicked(row, col, event->button()); // Emit signal with row, col, and button info
    QPushButton::mousePressEvent(event); // Call default QPushButton behavior
}


// ================================= GETTERS & SETTERS ==================================================================

/**
 * @name getRow
 * @brief Tile::getRow returns the row coordinate of this tile.
 * @return int row coordinate
 */
int Tile::getRow() {
    return row;
}

/**
 * @name getCol
 * @brief Tile::getCol returns the column coordinate of this tile.
 * @return int columns coordinate
 */
int Tile::getCol() {
    return col;
}

/**
 * @name getMineCount
 * @brief Tile::getMineCount returns the number of mines touching this tile.
 * @return int number of mines touching this tile.
 */
int Tile::getMineCount() {
    return surrMines;
}

/**
 * @name isMine
 * @brief Tile::isMine returns whether or not this tile is a mine.
 * @return bool whether or not this tile is a mine.
 */
bool Tile::isMine() {
    return mine;
}

/**
 * @name isFlagged
 * @brief Tile::isFlagged returns the flag status of this tile.
 * @return bool flag status.
 */
bool Tile::isFlagged() {
    return flagged;
}

/**
 * @name isBelieved
 * @brief Tile::isBelieved
 * @return
 */
bool Tile::isBelieved() {
    return believed;
}

/**
 * @name isRevealed
 * @brief Tile::isRevealed returnes whether or not this tile is revealed.
 * @return revealed status.
 */
bool Tile::isRevealed() {
    return revealed;
}

/**
 * @name setRow
 * @brief Tile::setRow sets the row coordinate of this tile.
 * @param r - row coordinate.
 */
void Tile::setRow(int r) {
    row = r;
}

/**
 * @name setCol
 * @brief Tile::setCol sets the column coordinate of this tile.
 * @param c - column coordinate.
 */
void Tile::setCol(int c) {
    col = c;
}

/**
 * @name setMineCount
 * @brief Tile::setMineCount sets the number of mines touching this tile.
 * @param count - number of mines touching this tile.
 */
void Tile::setMineCount(int count) {
    surrMines = count;
}

/**
 * @name setMine
 * @brief Tile::setMine makes a certain tile a mine.
 * @param mine - true/false.
 */
void Tile::setMine(bool m) {
    mine = m;
}

/**
 * @name setFlag
 * @brief Tile::setFlag flags a tile.
 * @param flag - true/false.
 */
void Tile::setFlag(bool flag) {
    flagged = flag;
    if (flagged) this->setText("🚩");
    else this->setText("");
}

/**
 * @name setBelieved
 * @brief Tile::setBelieved allows the user to mark suspected mines.
 * @param b
 */
void Tile::setBelieved(bool b) {
    believed = b;
    if (believed) this->setText("❓");
    else this->setText("");
}

/**
 * @name setRevealed
 * @brief Tile::setRevealed tells the tile whether or not it is revealed
 * @param rev - true/false
 */
void Tile::setRevealed(bool rev) {
    revealed = rev;
}
