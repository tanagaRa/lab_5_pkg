#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    LayoutSettings();
    this->pbAverage->setEnabled(false);
    this->pbMnogoyg->setEnabled(false);
    connect (this->pbOpenFile,SIGNAL(clicked()),SLOT(readFile()));
    connect (this->pbAverage,SIGNAL(clicked()),SLOT(av()));
    connect (this->pbMnogoyg,SIGNAL(clicked()),SLOT(liangaBarski()));
    connect (this->pbClear,SIGNAL(clicked()),SLOT(clearPlots()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::LayoutSettings()
{
    QWidget* wgt = new QWidget;
    plot = new QCustomPlot;
    this->plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->plot->setInteraction(QCP::iRangeDrag,true);
    this->plot->setInteraction(QCP::iRangeZoom,true);
    this->plot->xAxis->setRange(-100, 100);
    this->plot->yAxis->setRange(-100, 100);
    this->plot->xAxis->setLabel("x");
    this->plot->yAxis->setLabel("y");
    this->box->addWidget(this->plot);
    this->boxbuttons->addWidget(this->pbAverage);
    this->boxbuttons->addWidget(this->pbMnogoyg);
    this->boxbuttons->addWidget(this->pbClear);
    this->boxbuttons->addWidget(this->pbOpenFile);
    this->box->addLayout(this->boxbuttons);
    wgt->setLayout(this->box);
    this->setCentralWidget(wgt);
    this->setMinimumSize(700,500);
}

void MainWindow::readFile()
{
    this->currentGraph = 0;
    this->x_min=0;
    this->y_min=0;
    this->x_max=0;
    this->y_max=0;
    this->n=0;
    this->m = -1;
    this->vector_X_1.clear();
    this->vector_Y_1.clear();
    this->vector_X_2.clear();
    this->vector_Y_2.clear();
    this->X_Mn.clear();
    this->Y_Mn.clear();
    this->check.clear();
    this->edges.clear();
    this->q = 0;
    this->id=0;
    this->ind=0;
    this->f_x=0;
    this->f_y=0;
    this->plot->clearGraphs();
    this->plot->replot();
  QFileDialog* dial = new QFileDialog(this);
  QString path = dial->getOpenFileName(this,tr("Выберите файл"),"/",tr("txt(*.txt)"));
  QFile file(path);
  if (file.open(QIODevice::ReadOnly))
  {
      QTextStream writeStream(&file);
      double x1,x2,y1,y2;
      writeStream >>this->n;
      for (int i = 0 ; i < n ; i++)
      {
           writeStream >> x1 >> y1 >> x2 >> y2;
           this->vector_X_1.push_back(x1);
           this->vector_Y_1.push_back(y1);
           this->vector_X_2.push_back(x2);
           this->vector_Y_2.push_back(y2);
      }
      writeStream >> this->m;
      if (m == 1)
      {
          this->pbMnogoyg->setEnabled(false);
          this->pbAverage->setEnabled(true);
          writeStream >> this-> x_min >> this-> y_min >> this-> x_max >> this-> y_max;
      }
      if (m == 2)
      {
          this->pbMnogoyg->setEnabled(true);
          this->pbAverage->setEnabled(false);
          int k;
          writeStream >> k;
          int x,y;
          for (int i = 0 ; i < k; i++)
          {
              writeStream >> x >> y;
              this->X_Mn.push_back(x);
              this->Y_Mn.push_back(y);
              this->check.push_back(false);
          }

      }

      file.close();
      draw();
  }
}

void MainWindow::clearPlots()
{
    this->currentGraph = 0;
    this->x_min=0;
    this->y_min=0;
    this->x_max=0;
    this->y_max=0;
    this->n=0;
    this->m = -1;
    this->vector_X_1.clear();
    this->vector_Y_1.clear();
    this->vector_X_2.clear();
    this->vector_Y_2.clear();
    this->X_Mn.clear();
    this->Y_Mn.clear();
    this->check.clear();
    this->edges.clear();
    this->q = 0;
    this->id=0;
    this->ind=0;
    this->f_x=0;
    this->f_y=0;
    this->plot->clearGraphs();
    this->plot->replot();
    draw();
}

void MainWindow::draw()
{
    if (m == 1)
        drawRectangle();
    else if (m == 2)
        drawMnogoug();
    for (int i = 0 ; i < this->n; i ++)
    {
        currentGraph++;
        this->plot->addGraph();
        this->plot->graph(currentGraph)->addData(this->vector_X_1[i], this->vector_Y_1[i]);
        this->plot->graph(currentGraph)->addData(this->vector_X_2[i], this->vector_Y_2[i]);
        this->plot->replot();
    }
}

void MainWindow::drawRectangle()
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    this->plot->addGraph();
    this->plot->graph(currentGraph)->addData(this->x_min, this->y_min);
    this->plot->graph(currentGraph)->addData(this->x_max, this->y_min);
    this->plot->graph(currentGraph)->setPen(pen);
    currentGraph++;
    this->plot->addGraph();
    this->plot->graph(currentGraph)->addData(this->x_max, this->y_min);
    this->plot->graph(currentGraph)->addData(this->x_max, this->y_max);
    this->plot->graph(currentGraph)->setPen(pen);
    currentGraph++;
    this->plot->addGraph();
    this->plot->graph(currentGraph)->addData(this->x_max, this->y_max);
    this->plot->graph(currentGraph)->addData(this->x_min, this->y_max);
    this->plot->graph(currentGraph)->setPen(pen);
    currentGraph++;
    this->plot->addGraph();
    this->plot->graph(currentGraph)->addData(this->x_min, this->y_max);
    this->plot->graph(currentGraph)->addData(this->x_min, this->y_min);
    this->plot->graph(currentGraph)->setPen(pen);
    this->plot->replot();
}

void MainWindow::av()
{
    for (int i = 0; i < this->n; i ++)
    {
        cutLine1(vector_X_1[i],vector_Y_1[i], vector_X_2[i], vector_Y_2[i]);
    }

}

void MainWindow::cutLine1(double x1,double y1,double x2,double y2)
{
    if(sqrt(pow(x1-x2,2)+pow(y1-y2,2)) <= 1)
    {
        return;
    }
    bool t1=false,b1=false,l1=false,r1=false;
    bool t2=false,b2=false,l2=false,r2=false;
    where(this->x_min,l1,x1,r1,this->x_max);
    where(this->x_min,b1,y1,t1,this->x_max);
    where(this->x_min,l2,x2,r2,this->x_max);
    where(this->x_min,b2,y2,t2,this->x_max);
    if(!t1 && !t2 && !r1 && !r2 && !l1 && !l2 && !b1 && !b2)
    {
        currentGraph++;
        QPen pen;
        pen.setWidth(3);
        pen.setColor(Qt::darkRed);
        this->plot->addGraph();
        this->plot->graph(currentGraph)->addData(x1, y1);
        this->plot->graph(currentGraph)->addData(x2, y2);
        this->plot->graph(currentGraph)->setPen(pen);
        this->plot->replot();
        return;
    }
    if((t1 == t2 && t1 != false) || (r1 == r2 && r1 != false) || (l1 == l2 && l1 != false) || (b1 == b2 && b1 != false))
    {
        return;
    }
    cutLine1(x1,y1,(x2+x1)/2,(y2+y1)/2);
    cutLine1((x2+x1)/2,(y2+y1)/2,x2,y2);
}

void MainWindow::where(double l,bool& p1,double t,bool& p2,double r)
{
    if(t > r)
    {
        p1 = false;
        p2 = true;
    }
    else if (l > t)
    {
        p2 = false;
        p1 = true;
    }
    else
    {
        p1 = false;
        p2 = false;
    }
}

void MainWindow::liangaBarski()
{
    for (int i = 0; i < this->n; i ++)
    {
        int yes = 0,no = 0;
        for (int j = 0 ; j < edges.size();j++)
        {
            double u = (edges[j].x1 - vector_X_1[i])/(vector_X_2[i]-vector_X_1[i])-(edges[j].y1-vector_Y_1[i])/(vector_Y_2[i]-vector_Y_1[i]);
            if (u >= 0)
                yes = 1;
            if (u <= 0)
                no = 1;
            if (yes == 1 && no == 1)
                break;
        }
        if (yes ==1 && no ==1 )
        cutLine2(vector_X_1[i],vector_Y_1[i], vector_X_2[i], vector_Y_2[i]);
    }
}

void MainWindow::cutLine2(double x1,double y1,double x2,double y2)
{
    QVector <double> t_exit;
    QVector <double> t_enter;
    double max = 0,min = 1;
    for (int i = 0; i < this->edges.size(); i++)
    {
        double A,B,S;
        double t;
        A = -(edges[i].y2 - edges[i].y1);
        B = edges[i].x2 - edges[i].x1;
        S = A*(x2-x1) + B*(y2-y1);
        if (edges[i].y2-edges[i].y1 != 0 && edges[i].x2-edges[i].x1 != 0)
            t = ((y1-edges[i].y1)*(edges[i].x2-edges[i].x1)/(edges[i].y2-edges[i].y1) + edges[i].x1 - x1) / (x2 - x1 - (y2 - y1)*(edges[i].x2 - edges[i].x1)/(edges[i].y2-edges[i].y1));
        else if (edges[i].y2-edges[i].y1 == 0)
            t =(edges[i].y1 -y1) / (y2 - y1);
        else
             t =(edges[i].x1 -x1) / (x2 - x1);
        if (S>0)
        {
            if (t <=1 && t >=0)
                t_enter.push_back(t);
        }else if (S<0)
        {
            if (t <=1 && t >=0)
                t_exit.push_back(t);

        }

    }
    if (t_enter.size()>0)
        max = t_enter[0];
    for (int i = 0; i < t_enter.size();i++)
    {
        if (t_enter[i] > max)
            max =t_enter[i];
    }
    if (t_exit.size()>0)
        min = t_exit[0];
    for (int i = 0; i < t_exit.size();i++)
    {
        if (t_exit[i] < min)
            min =t_exit[i];
    }
            currentGraph++;
            QPen pen;
            pen.setWidth(3);
            pen.setColor(Qt::darkRed);
            this->plot->addGraph();
            this->plot->graph(currentGraph)->addData(x1+max*(x2-x1), y1+max*(y2-y1));
            this->plot->graph(currentGraph)->addData(x1+min*(x2-x1), y1+min*(y2-y1));
            this->plot->graph(currentGraph)->setPen(pen);
            this->plot->replot();

}

void MainWindow::drawMnogoug()
{
    if (q >= 2)
    {
        check[id]=false;
    }
    int k = this->Y_Mn.size();
    if (q == 0)
    {
    int min = this->Y_Mn[0];
    int minx = this->X_Mn[0];

    for (int i = 0 ; i < k; i++)
    {
        if ((min > Y_Mn[i] || (min == Y_Mn[i] && minx < X_Mn[i])) && this->check[i] == false)
        {
            min =Y_Mn[i];
            minx=X_Mn[i];
            ind = i;
        }
    }
        check[ind]=true;

            id = ind;
            f_x = X_Mn[id];
            f_y = Y_Mn[id];
    }


    QVector <vect> vectors;
    vect tmp_v;
    for (int i = 0 ; i < k; i++)
    {
        if (!check[i])
        {
            tmp_v.x=X_Mn[i] - X_Mn[ind];
            tmp_v.y=Y_Mn[i] - Y_Mn[ind];
            tmp_v.x1=X_Mn[ind];
            tmp_v.y1=Y_Mn[ind];
            tmp_v.x2=X_Mn[i];
            tmp_v.y2=Y_Mn[i];
            tmp_v.end = i;
            vectors.push_back(tmp_v);
        }
    }
    tmp_v = vectors[0];
    for (int i = 1; i < vectors.size(); i++)
    {
        if (tmp_v.x * vectors[i].y - tmp_v.y * vectors[i].x < 0 || (tmp_v.x * vectors[i].y - tmp_v.y * vectors[i].x == 0 && (pow(tmp_v.x,2)+pow(tmp_v.y,2) <= pow(vectors[i].x,2)+pow(vectors[i].y,2))))
        {
            tmp_v = vectors[i];
        }
    }
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    this->plot->addGraph();
    this->plot->graph(currentGraph)->addData(tmp_v.x1, tmp_v.y1);
    this->plot->graph(currentGraph)->addData(tmp_v.x2, tmp_v.y2);
    this->plot->graph(currentGraph)->setPen(pen);
    currentGraph++;
    this->plot->replot();
    q++;
    ind = tmp_v.end;
    this->edges.push_back(tmp_v);
    if (tmp_v.x2 == f_x && tmp_v.y2 == f_y)
    {
        currentGraph--;
        return;
    }
    drawMnogoug();
}
