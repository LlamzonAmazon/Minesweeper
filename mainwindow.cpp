#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator64>

/* Constructor */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , buttons(16) {
    ui->setupUi(this);
    createGrid();
}

/* Destructor */
MainWindow::~MainWindow() {
    delete ui;
}

/* Create grid layout */
QGridLayout *layout;
QVector<QVector<QPushButton*>> buttons;
QVector<QVector<QPushButton*>> mines;
void MainWindow::createGrid() {

    QFrame *frame = new QFrame(this);
    layout = new QGridLayout(frame);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    buttons.resize(16);
    for (int i = 0; i < 16; ++i) {
        buttons[i].resize(30);
    }

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 30; ++j) {
            QPushButton *button = new QPushButton();
            button->setMinimumSize(40, 40);
            button->setMaximumSize(40, 40);

            layout->addWidget(button, i, j);
            buttons[i][j] = button;
            buttons[i][j]->setProperty("adjMines", 0);
            buttons[i][j]->setProperty("isMine", false);
            buttons[i][j]->setProperty("flagged", false);

            // Listening for specific mouse click event
            button->installEventFilter(this);
        }
    }

    setMines();

    setCentralWidget(frame);
}

/* Randomly set mines on the grid */
const int NUM_MINES = 75;
void MainWindow::setMines() {

    for (int i = 0; i < NUM_MINES; ++i) {

        int randRow;
        int randCol;

        while (buttons[randRow][randCol]->property("isMine").toBool()) {
            randRow = QRandomGenerator::global()->bounded(16);
            randCol = QRandomGenerator::global()->bounded(30);
        }


        buttons[randRow][randCol]->setProperty("isMine", true);
        // buttons[randRow][randCol]->setText("💣"); // DEBUG

        // Increment adjMine values for all tiles surrounding the mine
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                if (j == 0 && k == 0) continue; // Ignore this tile
                if (randRow+j < 0 || randCol+k < 0 || randRow+j >= buttons.size() || randCol+k >= buttons[0].size()) continue; // Ignore out of bounds indices

                if (buttons[randRow+j][randCol+k]->property("isMine") == false)
                    buttons[randRow+j][randCol+k]->setProperty("adjMines", buttons[randRow+j][randCol+k]->property("adjMines").toInt()+1);
            }
        }
    }
}

/* Check left click or right click */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPushButton *button = qobject_cast<QPushButton *>(obj);

        if (button) {
            for (int i = 0; i < 16; ++i) {
                for (int j = 0; j < 30; ++j) {

                    if (buttons[i][j] == button) {

                        if (mouseEvent->button() == Qt::LeftButton) handleLeftClick(i, j);
                        else if (mouseEvent->button() == Qt::RightButton) handleRightClick(i, j);

                        return false;
                    }
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::handleLeftClick(int row, int col) {
    QPushButton *button = buttons[row][col]; // Create pointer to point to clicked button using the buttons 2D vector

    if (button->property("flagged").toBool()) return; // Do nothing on left click if tile is flagged

    // Check if this button is a mine
    if (button->property("isMine") == true) {

        // Disable all buttons and reveal mines
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 30; ++j) {

                buttons[i][j]->setEnabled(false);

                if (buttons[i][j]->property("flagged").toBool()) // Tile flagged --> leave flag
                    continue;
                if (buttons[i][j]->property("isMine").toBool()) // Tile is mine --> Reveal mine
                    buttons[i][j]->setText("💣");
            }
        }

        QMessageBox::information(this, "Game Over", "You hit a mine! I'll give you another life though.");

        // Reset minefield
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 30; ++j) {

                buttons[i][j]->setEnabled(true);

                if (buttons[i][j]->property("isMine").toBool() && !buttons[i][j]->property("flagged").toBool())
                    buttons[i][j]->setText("");
            }
        }

    } else {
        sweep(row, col);
    }
}

/* Clear safe tiles */
void MainWindow::sweep(int row, int col) {

    if (row < 0 || col < 0 || row > 15 || col > 29) return; // Do not perform this function on out of bounds indices.
    if (!buttons[row][col]->isEnabled()) return; // Do not perform this function on cleared tiles.

    if (buttons[row][col]->property("adjMines").toInt() == 0) {

        buttons[row][col]->setEnabled(false);

        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                if (r == 0 && c == 0) continue;

                sweep(row+r, col+c);
            }
        }
    }
    else {
        buttons[row][col]->setText(buttons[row][col]->property("adjMines").toString());
    }
}

/* Set/Remove flag */
int flags = NUM_MINES;
void MainWindow::handleRightClick(int row, int col) {

    if (!buttons[row][col]->property("flagged").toBool()) {
        // Not flagged --> Set flag
        buttons[row][col]->setProperty("flagged", true);
        buttons[row][col]->setText("🚩");
        flags++;
    }
    else {
        // Flagged --> Remove flag
        buttons[row][col]->setProperty("flagged", false);
        buttons[row][col]->setText("");
        flags--;
    }
}
