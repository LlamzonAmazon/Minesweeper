#include "tile.h"
#include "mainwindow.h"
#include <QMouseEvent>

MainWindow *parent;
int row;
int col;
int surrMines;
bool mine;
bool flagged;
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
 * @brief Sweeps the minefield; disables tiles not touching mines and revealing those that are.
 * @param row - int row coordinate
 * @param col - int column coordinate
 */
void Tile::sweep(int row, int col) {

    if (row < 0 || col < 0 || row >= parent->NUM_ROWS || col >= parent->NUM_COLS) return; // Ignore out of bounds indices

    QVector<QVector<Tile *>> &tiles = parent->getTiles();
    Tile *tile = tiles[row][col];
    if (!tile->isEnabled()) return; // Ignore already cleared tiles.

    if (tile->getMineCount() == 0) {

        tile->setEnabled(false);
        tile->setRevealed(true);

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {

                if (dr == 0 && dc == 0) continue;

                sweep(row + dr, col + dc);
            }
        }

    } else {
        tile->setText(QString::number(tile->getMineCount()));
        tile->setRevealed(true);
    }
}

/**
 * @name mousePressEvent
 * @brief Overrides the mousePressEvent to handle left and right clicks
 */
void Tile::mousePressEvent(QMouseEvent *event) {
    emit tileClicked(row, col, event->button()); // Emit signal with row, col, and button info
    QPushButton::mousePressEvent(event); // Call default QPushButton behavior
}


// ================================= GETTERS & SETTERS =================================

/**
 * @name getRow
 * @brief getRow returns the row coordinate of this tile.
 * @return int row coordinate
 */
int Tile::getRow() {
    return row;
}

/**
 * @name getCol
 * @brief getCol returns the column coordinate of this tile.
 * @return int columns coordinate
 */
int Tile::getCol() {
    return col;
}

/**
 * @name getMineCount
 * @brief getMineCount returns the number of mines touching this tile.
 * @return int number of mines touching this tile.
 */
int Tile::getMineCount() {
    return surrMines;
}

/**
 * @name isMine
 * @brief isMine returns whether or not this tile is a mine.
 * @return bool whether or not this tile is a mine.
 */
bool Tile::isMine() {
    return mine;
}

/**
 * @name isFlagged
 * @brief isFlagged returns the flag status of this tile.
 * @return bool flag status.
 */
bool Tile::isFlagged() {
    return flagged;
}

/**
 * @name isRevealed
 * @brief isRevealed returnes whether or not this tile is revealed.
 * @return revealed status.
 */
bool Tile::isRevealed() {
    return revealed;
}

/**
 * @name setRow
 * @brief setRow sets the row coordinate of this tile.
 * @param r - row coordinate.
 */
void Tile::setRow(int r) {
    row = r;
}

/**
 * @name setCol
 * @brief setCol sets the column coordinate of this tile.
 * @param c - column coordinate.
 */
void Tile::setCol(int c) {
    col = c;
}

/**
 * @name setMineCount
 * @brief setMineCount sets the number of mines touching this tile.
 * @param count - number of mines touching this tile.
 */
void Tile::setMineCount(int count) {
    surrMines = count;
}

/**
 * @name setMine
 * @brief setMine makes a certain tile a mine.
 * @param mine - true/false.
 */
void Tile::setMine(bool m) {
    mine = m;
}

/**
 * @name setFlag
 * @brief setFlag flags a tile.
 * @param flag - true/false.
 */
void Tile::setFlag(bool flag) {
    flagged = flag;
}

/**
 * @name setRevealed
 * @brief setRevealed tells the tile whether or not it is revealed
 * @param rev - true/false
 */
void Tile::setRevealed(bool rev) {
    revealed = rev;
}
