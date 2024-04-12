#include "barchart.h"

Barchart::Barchart(QCustomPlot* cPlot, uint32_t numGraps, QObject *parent)
    : QObject{parent}
{
    barsList.resize(numGraps);

    for(int i = 0; i<barsList.size(); ++i)
    {
        barsList[i] = new QCPBars(cPlot->xAxis, cPlot->yAxis);
    }
    barsList[0]->setName("Количество рейсов");
    barsList[0]->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    barsList[0]->setBrush(QColor(111, 9, 176));
    barsList[0]->setWidth(0.5);
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    labels << "Сентябрь 2016" << "Октябрь 2016" << "Ноябрь 2016" << "Декабрь 2016" <<
              "Январь 2017" << "Февраль 2017" << "Март 2017" << "Апрель 2017" << "Май 2017" << "Июнь 2017" <<
              "Июль 2017" << "Август 2017";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    cPlot->xAxis->setTicker(textTicker);
    cPlot->xAxis->setSubTicks(false);
    cPlot->xAxis->setTickLength(0, 4);
    cPlot->xAxis->setRange(0, 13);
    cPlot->xAxis->setTickLabelRotation(60);
    cPlot->legend->setVisible(true);
    cPlot->legend->setFont(QFont("Helvetica",9));

    //cPlot->setInteraction(QCP::iRangeZoom, true);
    //cPlot->setInteraction(QCP::iRangeDrag, true);

}

void Barchart::RecieveData(const QVector<double>& y, QCustomPlot* cPlot, uint32_t numBar)
{
    ClearGraph(cPlot, numBar);
    AddDataToGraph(y, numBar);
    UpdateGraph(cPlot);
}

void Barchart::ClearGraphAll(QCustomPlot* cPlot)
{
    for(int i = 0; i<barsList.size(); ++i)
    {
        barsList[i]->data().clear();
    }
    cPlot->replot();
}

void Barchart::ClearGraph(QCustomPlot* cPlot, uint32_t numGraph)
{
    barsList[numGraph]->data().clear();
    cPlot->replot();
}

void Barchart::AddDataToGraph(const QVector<double>& y, uint32_t numGraph)
{
    barsList[numGraph]->setData(ticks,y);
}

void Barchart::UpdateGraph(QCustomPlot* cPlot)
{
    cPlot->rescaleAxes();
    cPlot->replot();
    cPlot->show();
}

Barchart::~Barchart()
{}
