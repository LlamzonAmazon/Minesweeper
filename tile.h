#ifndef TILE_H
#define TILE_H

#include <QPushButton>

class MainWindow;


class Tile : public QPushButton {
    Q_OBJECT

public:
    explicit Tile(MainWindow *par, int r, int c);

    bool sweep(int row, int col); // Function to handle revealing surrounding tiles

    int getRow();
    int getCol();
    int getMineCount();
    bool isMine();
    bool isFlagged();
    bool isBelieved();
    bool isRevealed();

    void setRow(int r);
    void setCol(int c);
    void setMineCount(int count);
    void setMine(bool m);
    void setFlag(bool flag);
    void setBelieved(bool b);
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
    bool believed;
    bool revealed;

protected:
    void mousePressEvent(QMouseEvent *event) override; // Override to capture mouse clicks
};

#endif // TILE_H
