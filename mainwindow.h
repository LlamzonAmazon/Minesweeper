#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Tile;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    const int NUM_MINES;
    const int NUM_ROWS;
    const int NUM_COLS;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createGrid();
    void restartGame();
    void setMines();
    void leftClick(int row, int col);
    void rightClick(int row, int col);
    QVector<QVector<Tile *>> &getTiles();

    int getClearCount();
    void setClearCount(int c);

public slots:
    void handleTileClick(int row, int col, Qt::MouseButton mb);

private:
    Ui::MainWindow *ui;

    static int clearedTiles;
    QVector<QVector<Tile *>> tiles;
    int clearCount;
};

#endif // MAINWINDOW_H
