#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "help.h"
#include "info.h"
#include <QString>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0,0,950,640);

    leftUp = QPoint(0,0);
    rightDown = QPoint(600,600);
    rec = QRect(leftUp, rightDown);

    p1 = QPoint(35, 35);
    p2 = QPoint(565,565);

    width = 66;
    length = 66;

    mtime = new QTimer(this);

    initial_chessboard();
    update();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHelp_triggered()
{
    help mhelp;
    mhelp.setModal(true);
    mhelp.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_us_triggered()
{
    info minfo;
    minfo.setModal(true);
    minfo.exec();
}

void MainWindow::initial_chessboard()
 {
     QPainter p(this);
     p.drawPixmap(rec, QPixmap(":/new/prefix1/chessboard1.png"));

     for(int i=0; i<8; i++)
         for(int j=0; j<8; j++)
         {
             reversi[i][j]= empty;
             temp_black[i][j] = empty;
             temp_white[i][j] = empty;
         }

     reversi[3][3] = black;
     reversi[3][4] = white;
     reversi[4][3] = white;
     reversi[4][4] = black;

     temp_black[3][3] = black;
     temp_black[3][4] = white;
     temp_black[4][3] = white;
     temp_black[4][4] = black;

     temp_white[3][3] = black;
     temp_white[3][4] = white;
     temp_white[4][3] = white;
     temp_white[4][4] = black;

     role = black;

     black_count = 2;
     white_count = 2;

     ui->black->setGeometry(650,180,120,120);
     ui->white->setGeometry(790,180,120,120);
     ui->blackCount->setGeometry(650,330,120,50);
     ui->whiteCount->setGeometry(790,330,120,50);

     ui->black->show();
     ui->white->show();
     ui->blackCount->display(2);
     ui->whiteCount->display(2);

     QFont font;
     font.setPointSize(28);

     ui->black_drop->setFont(font);
     ui->black_drop->setGeometry(700,400,150,100);
     ui->black_drop->hide();

     ui->white_drop->setFont(font);
     ui->white_drop->setGeometry(700,400,150,100);
     ui->white_drop->hide();

     ui->black_continue->setFont(font);
     ui->black_continue->setGeometry(670,400,250,100);
     ui->black_continue->setWordWrap(true);
     ui->black_continue->setAlignment(Qt::AlignTop);
     ui->black_continue->hide();

     ui->white_continue->setFont(font);
     ui->white_continue->setGeometry(670,400,250,100);
     ui->white_continue->setWordWrap(true);
     ui->white_continue->setAlignment(Qt::AlignTop);
     ui->white_continue->hide();

     ui->black_win->setFont(font);
     ui->black_win->setGeometry(700,400,250,100);
     ui->black_win->hide();

     ui->white_win->setFont(font);
     ui->white_win->setGeometry(700,400,250,100);
     ui->white_win->hide();
 }

void MainWindow::paintEvent(QPaintEvent *qpe)
{
    Q_UNUSED(qpe);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(rec, QPixmap(":/new/prefix1/chessboard1.png"));

    QPainter p(this);
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(reversi[i][j]==black)
                p.drawPixmap(p1.x()+j*length, p1.y()+i*width, length, width, QPixmap(":/new/prefix1/black.jpg"));
            else if(reversi[i][j]==white)
                p.drawPixmap(p1.x()+j*length, p1.y()+i*width, length, width, QPixmap(":/new/prefix1/white.jpg"));
        }
    }
    show_count();

    //qDebug()<<"updated!";
}

int MainWindow::reverse_amount(const int i, const int j)
  {
      int dir[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
      int tempX = i, tempY = j;
      int k = 0, dir_count = 0, count = 0, step_count = 0;
      //qDebug()<<"reversi["<<i<<"]["<<j<<"] = "<<reversi[i][j];
      if(reversi[tempX][tempY] != empty) //只有空的才可以落子         
          return 0;
      for(k=0; k<8; k++)
      {
          tempX+=dir[k][0];
          tempY+=dir[k][1];
          step_count ++;
          bool first_meet = 0;
          while(tempX>=0 && tempX<8 && tempY>=0 && tempY<8)
          {
              if(reversi[tempX][tempY] == empty) //遇到空的说明不能吃子
                  break;
              else
              {
                  if(reversi[tempX][tempY] == role)
                  {
                      tempX+=dir[k][0];
                      tempY+=dir[k][1];                     
                      if(dir_count!=0 && first_meet == 1) //之前已有对方子，且第一次回到自己的子
                      {
                          count+=dir_count; //可以吃子
                          break;
                      }
                      step_count ++;
                  }
                  else
                  {
                      if(step_count == 1)
                          first_meet = 1;
                          dir_count++;
                          tempX+=dir[k][0];
                          tempY+=dir[k][1];
                          step_count++;
                  }
              }
          }
          //qDebug()<<"dir_count="<<dir_count;
          dir_count=0;
          step_count=0;
          tempX=i;
          tempY=j;
      }
      return count;
  }

void MainWindow::reverse(const int &i, const int &j)
{
    for(int m=0; m<8; m++)
        for(int n=0; n<8; n++)
        {
            if(role == black)
                temp_black[m][n] = reversi[m][n];
            else
                temp_white[m][n] = reversi[m][n];
        }

    reversi[i][j] = role;
      int dir[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
      int tempX = i, tempY = j;
      int k = 0, dir_count = 0, count = 0, step_count = 0;
      bool first_meet = 0;
      for(k=0; k<8; k++)
      {
          tempX+=dir[k][0];
          tempY+=dir[k][1];
          step_count ++;
          while(tempX>=0 && tempX<8 && tempY>=0 && tempY<8)
          {
              if(reversi[tempX][tempY] == empty)
                  break;
              else
              {
                  if(reversi[tempX][tempY] == role)
                  {
                      tempX+=dir[k][0];
                      tempY+=dir[k][1];
                      if(dir_count!=0 && first_meet == 1) //之前已有对方子，且第一次回到自己的子
                      {
                          count += dir_count; //可以吃子，接下来进行翻转操作
                          tempX = i+dir[k][0];
                          tempY = j+dir[k][1];
                          while(tempX>=0 && tempX<8 && tempY>=0 && tempY<8)
                          {
                              if(reversi[tempX][tempY] == role)
                                  break;
                              else
                              {
                                  reversi[tempX][tempY] = role;
                                  tempX+=dir[k][0];
                                  tempY+=dir[k][1];
                              }
                          }
                          break;
                      }
                      step_count ++;
                  }
                  else
                  {
                      if(step_count == 1)
                          first_meet = 1;
                      step_count++;
                      dir_count++;
                      tempX+=dir[k][0];
                      tempY+=dir[k][1];
                  }
              }
          }
          dir_count=0;
          step_count=0;
          tempX=i;
          tempY=j;
          first_meet = 0;
      }

      show_count();
  }

int MainWindow::CanPlay_count(const int &i, const int &j)
  {
      int reversi_temp[8][8];
      int m, n;
      reversi_status role_temp;
      if(role == black)
          role_temp=white;
      else
          role_temp=black;

      for(m=0; m<8; m++)
          for(n=0; n<8; n++)
              reversi_temp[m][n]=reversi[m][n];

      int dir[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
      int tempX = i, tempY = j;
      int k = 0, dir_count = 0, count = 0, step_count = 0;
      bool first_meet = 0;
      for(k=0; k<8; k++)
      {
          tempX+=dir[k][0];
          tempY+=dir[k][1];
          step_count ++;
          while(tempX>=0 && tempX<8 && tempY>=0 && tempY<8)
          {
              if(reversi_temp[tempX][tempY]==empty)
                  break;
              else
              {
                  if(reversi_temp[tempX][tempY] == role_temp)
                  {
                      tempX+=dir[k][0];
                      tempY+=dir[k][1];
                      if(dir_count!=0 && first_meet == 1) //之前已有对方子，且第一次回到自己的子
                      {
                          count += dir_count; //可以吃子，接下来进行翻转操作
                          tempX = i+dir[k][0];
                          tempY = j+dir[k][1];
                          while(tempX>=0 && tempX<8 && tempY>=0 && tempY<8)
                          {
                              if(reversi_temp[tempX][tempY] == role_temp)
                                  break;
                              else
                              {
                                  reversi_temp[tempX][tempY] = role_temp;
                                  tempX+=dir[k][0];
                                  tempY+=dir[k][1];
                              }
                          }
                          break;
                      }
                  }
                  else
                  {
                      if(step_count == 1)
                          first_meet = 1;
                          dir_count++;
                          tempX+=dir[k][0];
                          tempY+=dir[k][1];
                          step_count++;
                  }
              }
          }
          dir_count=0;
          step_count=0;
          tempX=i;
          tempY=j;
          first_meet = 0;
      }
      int canPlayCount=0;
      for(m=0; m<8; m++)
          for(n=0; n<8; n++)
              if(reverse_amount(m,n)>0)
                  canPlayCount++;
      return canPlayCount;
  }

void MainWindow::change_role()
  {
      if(role==black)
      {
          role=white;

          ui->black_drop->hide();
          ui->white_drop->show();
          ui->black_continue->hide();
          ui->white_continue->hide();

          if(can_play() && !end_of_game())
          {
              connect(mtime, SIGNAL(timeout()), this, SLOT(AI_play()));
              mtime->start(1000);
          }
          else if(!can_play() && !end_of_game())
          {
              ui->black_drop->hide();
              ui->white_drop->hide();
              ui->black_continue->show();
              ui->white_continue->hide();

              connect(mtime, SIGNAL(timeout()), this, SLOT(change_role()));
              mtime->start(1000);
          }
      }
      else
      {
          role=black;

          if(can_play() && !end_of_game())
          {
              ui->black_drop->show();
              ui->white_drop->hide();
              ui->black_continue->hide();
              ui->white_continue->hide();

              connect(mtime, SIGNAL(timeout()), this, SLOT(update()));
              mtime->start(500);
          }
          else if(!can_play() && !end_of_game())
          {
              ui->black_drop->hide();
              ui->white_drop->hide();
              ui->black_continue->hide();
              ui->white_continue->show();

              connect(mtime, SIGNAL(timeout()), this, SLOT(change_role()));
              mtime->start(1000);
          }
      }

  }

void MainWindow::AI_play()
  {
    if(end_of_game())
    {
        ui->black_drop->hide();
        ui->white_drop->hide();
        ui->black_continue->hide();
        ui->white_continue->hide();
    }
      if(role == white)
      {
          //bool flag = 0;
          int weight=0, max=-1000, m=0, n=0;
          int i=0, j=0;
       for(i=0; i<8; i++)
          for(j=0; j<8; j++)
          {
              if( reverse_amount(i,j)>0 )
              {
                  weight = xy_value[i][j]+reverse_amount(i,j)*4+CanPlay_count(i,j)*10;
                  //flag = 1;

                      if(weight>max)
                      {
                          max=weight;
                          m=i;
                          n=j;
                      }
              }
          }

      qDebug()<<"**********************";
      qDebug()<<"xy_value["<<m<<"]["<<n<<"] = "<<xy_value[m][n];
      qDebug()<<"reverse_amount = "<<reverse_amount(m,n);
      qDebug()<<"canPaly_count = "<<CanPlay_count(m,n);
      qDebug()<<"max = "<<max;

      //if(flag == 1){ui->black_drop->show();ui->white_drop->hide();ui->black_continue->hide();ui->white_continue->hide();}
      //else{ui->black_drop->hide();ui->white_drop->hide();ui->black_continue->show(); ui->white_continue->hide();}

      reverse(m,n);
      update();

      change_role();
      game_over();
      }
      else
          return;
  }

void MainWindow::show_count()
  {
      black_count=0;
      white_count=0;

      for(int i=0; i<8; i++)
          for(int j=0; j<8; j++)
          {
              if(reversi[i][j]==black)
                  black_count++;
              if(reversi[i][j]==white)
                  white_count++;
          }

      ui->blackCount->display(black_count);
      ui->whiteCount->display(white_count);
  }

void MainWindow::mousePressEvent(QMouseEvent *qme)
{
    if(end_of_game())
    {
        ui->black_drop->hide();
        ui->white_drop->hide();
        ui->black_continue->hide();
        ui->white_continue->hide();

    }
    if(role == black)
    {
        setMouseTracking(true);
        if(qme->buttons() & Qt::LeftButton)
        {
            //qDebug()<<"qme called!";
            int x = qme->x();
            int y = qme->y();
            int i = 0;
            int j = 0;

            if(x >= p1.x() && x <= (p1.x() + 8 * width) && y >= p1.y() && y <= (p1.y() + 8 * length))
            {
                i = (qme->y() - p1.y()) / length;
                j = (qme->x() - p1.x()) / width;

                //reversi[i][j] = role;

                if(reverse_amount(i,j) > 0)
                {
                    reverse(i,j);
                    qDebug()<<"can drop!";
                    //connect(mtime, SIGNAL(timeout()), this, SLOT(change_role()));
                    //mtime->start(1000);

                  this->change_role();
                  update();
                }
                game_over();
            }
            else
                return;
      }
   }
   else
       return;
}

bool MainWindow::end_of_game()
  {
      int i, j;
      bool flag=1;
      for(i=0; i<8; i++)
          for(j=0; j<8; j++)
              if(reverse_amount(i, j)>0)
                  flag=0;
      return flag;
}

bool MainWindow::can_play()
{
    bool flag = 0;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(reverse_amount(i,j) > 0)
                flag = 1;
    return flag;
}

void MainWindow::game_over()
{
    if(end_of_game())
    {
        if(black_count > white_count)
        {
            ui->black_drop->hide();
            ui->white_drop->hide();
            ui->black_continue->hide();
            ui->white_continue->hide();
            ui->black_win->show();
            ui->white_win->hide();
        }
        else if(black_count < white_count)
        {
            ui->black_drop->hide();
            ui->white_drop->hide();
            ui->black_continue->hide();
            ui->white_continue->hide();
            ui->black_win->hide();
            ui->white_win->show();
        }
    }
}

void MainWindow::on_actionStart_triggered()
{
    initial_chessboard();
    update();
}

void MainWindow::on_actionRegret_triggered()
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            reversi[i][j] = temp_black[i][j];
    update();
}
