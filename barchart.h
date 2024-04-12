#ifndef BARCHART_H
#define BARCHART_H

#include <QObject>
#include "qcustomplot.h"

class Barchart : public QObject
{
    Q_OBJECT
public:
    explicit Barchart(QCustomPlot* cPlot, uint32_t numGraps, QObject *parent = nullptr);
    ~Barchart();
    void RecieveData(const QVector<double>& y, QCustomPlot* cPlot, uint32_t numBar);
    void ClearGraphAll(QCustomPlot* cPlot);
    void ClearGraph(QCustomPlot* cPlot, uint32_t numGraph);
private:
    QList<QCPBars*> barsList;
    QVector<double> ticks;
    QVector<QString> labels;

    void AddDataToGraph(const QVector<double>& y, uint32_t numGraph);
    void UpdateGraph(QCustomPlot* cPlot);
signals:

};

#endif // BARCHART_H
