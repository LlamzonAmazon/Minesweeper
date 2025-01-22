#ifndef TILE_H
#define TILE_H

#include <QPushButton>

class MainWindow;


class Tile : public QPushButton {
    Q_OBJECT

public:
    explicit Tile(MainWindow *par, int r, int c);

    void sweep(int row, int col); // Function to handle revealing surrounding tiles
    int getRow();
    int getCol();
    int getMineCount();
    bool isMine();
    bool isFlagged();
    bool isRevealed();

    void setRow(int r);
    void setCol(int c);
    void setMineCount(int count);
    void setMine(bool m);
    void setFlag(bool flag);
    void setRevealed(bool rev);

signals:
    void tileClicked(int row, int col, Qt::MouseButton button); // Signal for tile click events

private:
    MainWindow *parent;
    int row;
    int col;
    int surrMines;
    bool mine;
    bool flagged;
    bool revealed;

protected:
    void mousePressEvent(QMouseEvent *event) override; // Override to capture mouse clicks
};

#endif // TILE_H
