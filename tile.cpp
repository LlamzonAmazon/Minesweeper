#include "tile.h"
#include "mainwindow.h"

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
 * @file tile.cpp
 * @brief This class represents every tile on the grid.
 *
 * This class handles behaviours pertaining to how buttons/tiles perform,
 * such as left & right click behaviour and recursive grid tile clearing (sweeping).
 *
 * @author Thomas Llamzon | 251316111 | TLLAMZON
 * @date Feb 1, 2025
 */

/**
 * @name Tile
 * @brief Constructs a Tile object.
 * @param par Pointer to the parent MainWindow.
 * @param r Row index of the tile.
 * @param c Column index of the tile.
 */
Tile::Tile(MainWindow *par, int r, int c)
    : parent(par), row(r), col(c), surrMines(0), mine(false), flagged(false), revealed(false) {
    setFixedSize(40, 40);
    tiles = parent->getTiles();
}

/**
 * @name sweep
 * @brief Reveals tiles recursively based on their distance from mines on the grid.
 * @param row Row index of the tile.
 * @param col Column index of the tile.
 * @return True if all non-mine tiles are cleared; otherwise, false.
 */
bool Tile::sweep(int row, int col) {
    QVector<QVector<Tile *>> &tiles = parent->getTiles();

    // Ignore out-of-bounds indices or already revealed tiles.
    if (row < 0 || col < 0 || row >= parent->getROW() || col >= parent->getCOL()) return false;
    if (tiles[row][col]->isRevealed()) return false;

    Tile *tile = tiles[row][col];
    parent->setClearCount(parent->getClearCount() + 1); // Increment cleared tile count.

    if (tile->getMineCount() == 0) { // If the tile is not touching any mines.

        tile->setRevealed(true);

        if (tile->isBelieved()) tile->setText("");

        tile->setVisible(false);

        // Recursively process surrounding tiles.
        for (int r = -1; r <= 1; ++r) {
            for (int c = -1; c <= 1; ++c) {

                if (r == 0 && c == 0) continue; // Skip the current tile.

                sweep(row + r, col + c);
            }
        }
    } else { // Tile is touching one or more mines.
        tile->setIcon(QIcon());
        tile->setText(QString::number(tile->getMineCount()));
        tile->setRevealed(true);
    }

    // Check if all non-mine tiles have been cleared.
    if (parent->getClearCount() >= ((parent->getROW() * parent->getCOL()) - parent->getMINES()))
        return true;
    return false;
}

/**
 * @name leftClick
 * @brief Handles actions when a tile is left-clicked.
 * @param row Row index of the clicked tile.
 * @param col Column index of the clicked tile.
 */
void MainWindow::leftClick(int row, int col) {
    if (tiles[row][col]->isFlagged()) return; // Ignore flagged tiles.

    Tile *tile = tiles[row][col];
    bool gameWin = false;

    if (tile->isMine()) { // If the clicked tile is a mine, end the game.

        QIcon bomb(":/tileIcons/icons/bomb.png");

        for (int i = 0; i < NUM_ROWS; ++i) {
            for (int j = 0; j < NUM_COLS; ++j) {

                if (tiles[i][j]->isMine())
                    tiles[i][j]->setIcon(bomb);

                tiles[i][j]->setEnabled(false);
            }
        }

        QMessageBox::information(this, "GAME LOSE", "Oh no! You hit a mine!");

        restartGame();
    } else gameWin = tile->sweep(row, col);

    if (gameWin) { // If the player wins the game.

        QMessageBox::information(this, "GAME WIN", "Congratulations! You didn't die!");

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
 * @brief Handles actions when a tile is right-clicked.
 * @param row Row index of the clicked tile.
 * @param col Column index of the clicked tile.
 */
void MainWindow::rightClick(int row, int col) {
    if (tiles[row][col]->isRevealed()) return; // Ignore revealed tiles.

    Tile *tile = tiles[row][col];

    if (!tile->isFlagged() && !tile->isBelieved()) { // No mark -> Flag.
        tile->setFlag(true);

    } else if (tile->isFlagged() && !tile->isBelieved()) { // Flag -> Question mark.
        tile->setFlag(false);
        tile->setBelieved(true);

    } else if (!tile->isFlagged() && tile->isBelieved()) { // Question mark -> No mark.
        tile->setBelieved(false);
    }
}

/**
 * @name mousePressEvent
 * @brief Captures mouse click events on the tile and emits the appropriate signal.
 * @param event Mouse event data.
 */
void Tile::mousePressEvent(QMouseEvent *event) {
    emit tileClicked(row, col, event->button()); // Emit signal with row, col, and button info.
    QPushButton::mousePressEvent(event); // Call base class behavior.
}


// ================================= GETTERS & SETTERS ==================================

/**
 *
 * @name getRow
 * @brief Gets the row index of the tile.
 * @return Row index of the tile.
 */
int Tile::getRow() { return row; }

/**
 * @name getCol
 * @brief Gets the column index of the tile.
 * @return Column index of the tile.
 */
int Tile::getCol() { return col; }

/**
 * @name getMineCount
 * @brief Gets the number of surrounding mines.
 * @return Number of surrounding mines.
 */
int Tile::getMineCount() { return surrMines; }

/**
 * @name isMine
 * @brief Checks if the tile is a mine.
 * @return True if the tile is a mine; otherwise, false.
 */
bool Tile::isMine() { return mine; }

/**
 * @name isFlagged
 * @brief Checks if the tile is flagged.
 * @return True if the tile is flagged; otherwise, false.
 */
bool Tile::isFlagged() { return flagged; }

/**
 * @name isBelieved
 * @brief Checks if the tile is marked with a question mark.
 * @return True if the tile is marked as believed; otherwise, false.
 */
bool Tile::isBelieved() { return believed; }

/**
 * @name isRevealed
 * @brief Checks if the tile is revealed.
 * @return True if the tile is revealed; otherwise, false.
 */
bool Tile::isRevealed() { return revealed; }

/**
 * @name setRow
 * @brief Sets the row index of the tile.
 * @param r Row index to set.
 */
void Tile::setRow(int r) { row = r; }

/**
 * @name setCol
 * @brief Sets the column index of the tile.
 * @param c Column index to set.
 */
void Tile::setCol(int c) { col = c; }

/**
 * @name setMineCount
 * @brief Sets the number of surrounding mines.
 * @param count Number of mines to set.
 */
void Tile::setMineCount(int count) { surrMines = count; }

/**
 * @name setMine
 * @brief Marks the tile as a mine.
 * @param m True if the tile is a mine; otherwise, false.
 */
void Tile::setMine(bool m) { mine = m; }

/**
 * @name setFlag
 * @brief Flags or unflags the tile.
 * @param flag True to flag the tile; false to unflag it.
 */
void Tile::setFlag(bool flag) {
    flagged = flag;
    if (flagged) {
        this->setIcon(QIcon(":/tileIcons/icons/flag.png"));
    }
    else this->setIcon(QIcon());
}

/**
 * @name setBelieved
 * @brief Marks or unmarks the tile with a question mark.
 * @param b True to mark the tile; false to unmark it.
 */
void Tile::setBelieved(bool b) {
    believed = b;
    if (believed) {
        this->setIcon(QIcon(":/tileIcons/icons/qmark.png"));
    }
    else this->setIcon(QIcon());
}

/**
 * @name setRevealed
 * @brief Sets the revealed state of the tile.
 * @param rev True to reveal the tile; false to hide it.
 */
void Tile::setRevealed(bool rev) { revealed = rev; }
