#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <QDebug>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QRandomGenerator>

/**
 * @file mainwindow.h
 * @brief Declaration of the MainWindow class.
 *
 * This file defines the MainWindow class, which handles the game's UI and logic,
 * including grid creation, mine placement, and user interaction.
 *
 * @author Thomas Llamzon
 * @date Jan 28, 2025
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Tile;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    /**
     * @brief Constructor for MainWindow.
     * @param parent The parent widget for the main window.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     */
    ~MainWindow();

    /**
     * @brief Restarts the game by recreating the MainWindow.
     */
    void restartGame();

    /**
     * @brief Retrieves the grid of tiles.
     * @return A reference to the 2D vector of tiles.
     */
    QVector<QVector<Tile *>> &getTiles();

    /**
     * @brief Gets the count of cleared tiles.
     * @return The count of cleared tiles.
     */
    int getClearCount() const;

    /**
     * @brief Gets the number of mines on the grid.
     * @return The number of mines on the grid.
     */
    int getMINES() const;

    /**
     * @brief Gets the number of rows on the grid.
     * @return The number of rows on the grid.
     */
    int getROW() const;

    /**
     * @brief Gets the number of columns on the grid.
     * @return The number of columns on the grid.
     */
    int getCOL() const;

    /**
     * @brief Sets the count of cleared tiles.
     * @param c The new cleared tile count.
     */
    void setClearCount(int c);

public slots:
    /**
     * @brief Handles tile clicks and determines the appropriate action.
     * @param row The row of the clicked tile.
     * @param col The column of the clicked tile.
     * @param mb The mouse button used for the click.
     */
    void handleTileClick(int row, int col, Qt::MouseButton mb);

private:
    /**
     * @brief Creates the game grid and initializes tiles.
     */
    void createGrid();

    /**
     * @brief Randomly places mines on the grid and updates neighboring tile counts.
     */
    void setMines();

    /**
     * @brief Handles left-click behavior on a tile.
     * @param row The row of the clicked tile.
     * @param col The column of the clicked tile.
     */
    void leftClick(int row, int col);

    /**
     * @brief Handles right-click behavior on a tile (e.g., flagging).
     * @param row The row of the clicked tile.
     * @param col The column of the clicked tile.
     */
    void rightClick(int row, int col);


    const int NUM_MINES;
    const int NUM_ROWS;
    const int NUM_COLS;

    Ui::MainWindow *ui;
    QVector<QVector<Tile *>> tiles;
    int clearCount;

    Q_DISABLE_COPY(MainWindow)
};

#endif // MAINWINDOW_H
