#include "graphic.h"

Graphic::Graphic(QCustomPlot* cPlot, uint32_t numGraps, QObject *parent)
{
   ptrGraph.resize(numGraps);

   for(int i = 0; i<ptrGraph.size(); i++)
   {
       ptrGraph[i] = new QCPGraph(cPlot->xAxis, cPlot->yAxis);
   }
   ptrGraph[0]->setName("Количество рейсов");
   cPlot->legend->setVisible(true);
   cPlot->legend->setFont(QFont("Helvetica",9));
}

void Graphic::AddDataToGraph(const QVector<double>& x, const QVector<double>& y, uint32_t numGraph)
{
    ptrGraph[numGraph]->setData(x,y);
}

void Graphic::ClearGraphAll(QCustomPlot* cPlot)
{
    for(int i = 0; i<ptrGraph.size(); ++i)
    {
       ptrGraph[i]->data().clear();
    }

    for(int i = 0; i< cPlot->graphCount(); ++i)
    {
        cPlot->graph(i)->data()->clear();
    }
    cPlot->replot();
}

void Graphic::ClearGraph(QCustomPlot* cPlot, uint32_t numGraph)
{
    ptrGraph[numGraph]->data().clear();
    cPlot->graph(numGraph)->data()->clear();
    cPlot->replot();
}

void Graphic::UpdateGraph(QCustomPlot* cPlot)
{
    cPlot->rescaleAxes();
    cPlot->replot();
    cPlot->show();
}

void Graphic::RecieveData(const QVector<double>& x, const QVector<double>& y, QCustomPlot* cPlot, uint32_t numGraph)
{
    ClearGraph(cPlot, numGraph);
    AddDataToGraph(x, y, numGraph);
    UpdateGraph(cPlot);
}

Graphic::~Graphic()
{
}
