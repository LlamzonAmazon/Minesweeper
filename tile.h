#ifndef TILE_H
#define TILE_H

#include <QPushButton>

#include <QMouseEvent>
#include <QMessageBox>
#include <QIcon>

class MainWindow;

class Tile : public QPushButton {
    Q_OBJECT

public:
    /**
     * @name Tile
     * @brief Constructor for Tile objects.
     * @param par - Parent MainWindow object.
     * @param r - Row coordinate of the tile.
     * @param c - Column coordinate of the tile.
     */
    explicit Tile(MainWindow *par, int r, int c);

    /**
     * @name sweep
     * @brief Reveals the current tile and recursively reveals surrounding tiles if they are safe.
     * @param row - Row coordinate of the tile.
     * @param col - Column coordinate of the tile.
     * @return true if the game is won after this operation, false otherwise.
     */
    bool sweep(int row, int col);

    /**
     * @name getRow
     * @brief Gets the row coordinate of the tile.
     * @return int Row coordinate.
     */
    int getRow();

    /**
     * @name getCol
     * @brief Gets the column coordinate of the tile.
     * @return int Column coordinate.
     */
    int getCol();

    /**
     * @name getMineCount
     * @brief Gets the number of mines surrounding the tile.
     * @return int Number of surrounding mines.
     */
    int getMineCount();

    /**
     * @name isMine
     * @brief Checks if the tile is a mine.
     * @return true if the tile is a mine, false otherwise.
     */
    bool isMine();

    /**
     * @name isFlagged
     * @brief Checks if the tile is flagged.
     * @return true if the tile is flagged, false otherwise.
     */
    bool isFlagged();

    /**
     * @name isBelieved
     * @brief Checks if the tile is marked with a question mark.
     * @return true if the tile is marked, false otherwise.
     */
    bool isBelieved();

    /**
     * @name isRevealed
     * @brief Checks if the tile is revealed.
     * @return true if the tile is revealed, false otherwise.
     */
    bool isRevealed();

    /**
     * @name setRow
     * @brief Sets the row coordinate of the tile.
     * @param r - Row coordinate.
     */
    void setRow(int r);

    /**
     * @name setCol
     * @brief Sets the column coordinate of the tile.
     * @param c - Column coordinate.
     */
    void setCol(int c);

    /**
     * @name setMineCount
     * @brief Sets the number of mines surrounding the tile.
     * @param count - Number of surrounding mines.
     */
    void setMineCount(int count);

    /**
     * @name setMine
     * @brief Marks the tile as a mine.
     * @param m - true if the tile is a mine, false otherwise.
     */
    void setMine(bool m);

    /**
     * @name setFlag
     * @brief Flags or unflags the tile.
     * @param flag - true to flag the tile, false to unflag it.
     */
    void setFlag(bool flag);

    /**
     * @name setBelieved
     * @brief Marks or unmarks the tile with a question mark.
     * @param b - true to mark the tile, false to unmark it.
     */
    void setBelieved(bool b);

    /**
     * @name setRevealed
     * @brief Sets the revealed status of the tile.
     * @param rev - true to mark the tile as revealed, false otherwise.
     */
    void setRevealed(bool rev);

signals:
    /**
     * @name tileClicked
     * @brief Signal emitted when a tile is clicked.
     * @param row - Row coordinate of the clicked tile.
     * @param col - Column coordinate of the clicked tile.
     * @param button - Mouse button used to click the tile.
     */
    void tileClicked(int row, int col, Qt::MouseButton button);

private:
    MainWindow *parent; ///< Pointer to the parent MainWindow.
    int row; ///< Row coordinate of the tile.
    int col; ///< Column coordinate of the tile.
    int surrMines; ///< Number of mines surrounding the tile.
    bool mine; ///< Whether the tile is a mine.
    bool flagged; ///< Whether the tile is flagged.
    bool believed; ///< Whether the tile is marked with a question mark.
    bool revealed; ///< Whether the tile is revealed.

protected:
    /**
     * @name mousePressEvent
     * @brief Captures mouse click events on the tile.
     * @param event - Mouse event data.
     */
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // TILE_H
