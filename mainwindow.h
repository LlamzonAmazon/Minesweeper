#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#include <QFrame>
#include <QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QVector<QVector<QPushButton*>> buttons;
    QGridLayout *layout;
    void createGrid();
    void setMines();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void handleLeftClick(int row, int col);
    void sweep(int row, int col);
    void handleRightClick(int row, int col);
};

#endif // MAINWINDOW_H
