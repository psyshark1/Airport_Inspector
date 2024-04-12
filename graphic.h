#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QObject>
#include "barchart.h"

class Graphic : public QObject
{
    Q_OBJECT   

public:
    Graphic(QCustomPlot* cPlot, uint32_t numGraps, QObject *parent = nullptr);
    ~Graphic();
    void RecieveData(const QVector<double>& x, const QVector<double>& y, QCustomPlot* cPlot, uint32_t numGraph);
    void ClearGraphAll(QCustomPlot* cPlot);
    void ClearGraph(QCustomPlot* cPlot, uint32_t numGraph);

private:
    QList<QCPGraph*> ptrGraph;
    void AddDataToGraph(const QVector<double>& x, const QVector<double>& y, uint32_t numGraph);
    void UpdateGraph(QCustomPlot* cPlot);

};

#endif // GRAPHIC_H
