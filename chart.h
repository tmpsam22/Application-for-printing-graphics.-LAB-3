#ifndef CHART_H
#define CHART_H

#include "common_container.h"

// forward declaration
class QChart;

class Chart
{
public:

    Chart();

    virtual ~Chart();

    QChart* getChart();

    void drawChart(const QString& title, const container& data);

    void reDrawChart() const;

    void cleanSeries();

    void switchColor();

private:

    QChart* chart_;

    bool isColorized_;

    container data_;
};


#endif // CHART_H
