#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum reversi_status {black=1, white=-1, empty=0};
    int xy_value[8][8]={
        {100, -60, 10, 10, 10, 10, -60, 100},
        {-60, -80, -5, -5, -5, -5, -80, -60},
        {10, -5, -5, -5, -5, -5, -5, 10},
        {10, -5, -5, -5, -5, -5, -5, 10},
        {10, -5, -5, -5, -5, -5, -5, 10},
        {10, -5, -5, -5, -5, -5, -5, 10},
        {-60, -80, -5, -5, -5, -5, -80, -60},
        {100, -60, 10, 10, 10, 10, -60, 100}
    };

    int black_count, white_count;

    void initial_chessboard();

    void paintEvent(QPaintEvent *);

    int reverse_amount(const int , const int );//看能翻多少个子

    void reverse(const int &, const int &);//把子给翻了

    int CanPlay_count(const int &, const int &);//数落在该处后对方有几个地方可以下

    //AI_play()

    void show_count();

    void mousePressEvent(QMouseEvent *);

    bool end_of_game();

    bool can_play();

    void game_start();

    void game_over();

    //void mouseMoveEvent(QMouseEvent *);

private slots:
    void on_actionHelp_triggered();

    void on_actionExit_triggered();

    void on_actionAbout_us_triggered();

    void AI_play();

    void change_role();

    void on_actionStart_triggered();

    void on_actionRegret_triggered();

private:
    Ui::MainWindow *ui;

    int width, length;

    int reversi[8][8];

    int temp_black[8][8], temp_white[8][8];

    QPoint leftUp, rightDown, p1, p2;

    reversi_status role;//判断下棋角色

    QRect rec;

    QTimer *mtime;
};

#endif // MAINWINDOW_H
