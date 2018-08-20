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

    role = black;

    black_count = 2;
    white_count = 2;

    leftUp = QPoint(0,0);
    rightDown = QPoint(600,600);
    rec = QRect(leftUp, rightDown);

    width = 75;
    length = 75;

    mtime = new QTimer(this);

    initial_chessboard();
    update();

    //connect(mtime, SIGNAL(timeout()), this, SLOT(AI_play()));
    //mtime->start(1000);

    if(end_of_game())
    {
        if(black_count>white_count)
            qDebug()<<"black wins!";
        else if(black_count<white_count)
            qDebug()<<"white wins!";
    }


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
             reversi[i][j]= empty;

     reversi[3][3] = black;
     reversi[3][4] = white;
     reversi[4][3] = white;
     reversi[4][4] = black;

     ui->black->setGeometry(650,180,120,120);
     ui->white->setGeometry(790,180,120,120);
     ui->blackCount->setGeometry(650,330,120,50);
     ui->whiteCount->setGeometry(790,330,120,50);

     ui->black->show();
     ui->white->show();
     ui->blackCount->display(2);
     ui->whiteCount->display(2);    
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
                p.drawPixmap(leftUp.x()+j*length, leftUp.y()+i*width, length, width, QPixmap(":/new/prefix1/black.jpg"));
            else if(reversi[i][j]==white)
                p.drawPixmap(leftUp.x()+j*length, leftUp.y()+i*width, length, width, QPixmap(":/new/prefix1/white.jpg"));
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
          //AI_play();

          connect(mtime, SIGNAL(timeout()), this, SLOT(AI_play()));
          mtime->start(1000);
      }
      else
      {
          role=black;

      }

  }

void MainWindow::AI_play()
  {
      if(role == white)
      {
          int weight=0, max=-1000, m=0, n=0;
          int i=0, j=0;
      for(i=0; i<8; i++)
          for(j=0; j<8; j++)
          {
              if( reverse_amount(i,j)>0 )
              {
                  weight = xy_value[i][j]+reverse_amount(i,j)*4+CanPlay_count(i,j)*10;


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
      reverse(m,n);
      update();
      change_role();
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
    //cout << "qme = " << qme->pos();
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

            if(x >= leftUp.x() && x <= (leftUp.x() + 8 * width) && y >= leftUp.y() && y <= (leftUp.y() + 8 * length))
            {
                i = (qme->y() - leftUp.y()) / length;
                j = (qme->x() - leftUp.x()) / width;

                //reversi[i][j] = role;

                if(reverse_amount(i,j) > 0)
                {
                    reverse(i,j);
                    qDebug()<<"can drop!";
                    //qDebug()<<"reverse_amount = "<<reverse_amount(i, j);

                    //connect(mtime, SIGNAL(timeout()), this, SLOT(change_role()));
                    //mtime->start(1000);

                  this->change_role();
                  update();
                }
            }
            else
                return;

            //qDebug()<<i<<","<<j;
            //qDebug()<<"reverse_amount = "<<reverse_amount(i, j);
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
