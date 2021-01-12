#include "datavisualization.h"

#include "constants.h"
#include "dbmanager.h"

DataVisualization::DataVisualization() {}

QList<QChart*> DataVisualization::get_charts(const QString& sensorId)
{
    QChart* tempChart = new QChart();
    QChart* humChart = new QChart();
    QLineSeries* tempSeries = new QLineSeries();
    QLineSeries* humSeries = new QLineSeries();
    build_series_by_sensor_id(sensorId, tempSeries, humSeries);

    QRgb serieColor(0xFFFFFF);
    if(sensorId.compare(sensorID::indoor, Qt::CaseSensitive) == 0) {
        serieColor = QRgb(0x78C745);
    } else if(sensorId.compare(sensorID::outdoor, Qt::CaseSensitive) == 0) {
        serieColor = QRgb(0xFF8A27);
    }

    build_chart(tempChart, database::column::temperature);
    add_serie_to_chart(tempChart, tempSeries, serieColor, "%i C");
    build_chart(humChart, database::column::humidity);
    add_serie_to_chart(humChart, humSeries, serieColor, "%i %");

    return QList<QChart*>{tempChart, humChart};
}

void DataVisualization::build_series_by_sensor_id(const QString& sensorId,
                                                  QLineSeries* tempSeries,
                                                  QLineSeries* humSeries)
{
    QDateTime startTime, endTime;
    //    QDate startDate = QDate::currentDate();
    //    startDate = startDate.addDays(-3);

    startTime.setDate(QDate::currentDate());
    startTime.setTime(QTime::currentTime());
    startTime = startTime.addSecs(-12 * 60 * 60);

    endTime.setDate(QDate::currentDate());
    endTime.setTime(QTime::currentTime());

    DbManager::instance().build_series_by_sensor_id_and_time_interval(
        sensorId, startTime, endTime, tempSeries, humSeries);
}

void DataVisualization::build_chart(QChart* chart, const QString& title)
{
    // Set title font
    QFont font;
    font.setPixelSize(18);
    font.setBold(true);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setTitle(title);
    // Set background
    chart->setBackgroundBrush(QColor(0x26, 0x26, 0x26));

    chart->legend()->hide();
}

void DataVisualization::add_serie_to_chart(QChart* chart,
                                           QLineSeries* serie,
                                           QRgb serieColor,
                                           const QString& axisLabelFormat)
{
    QPen pen(serieColor);
    pen.setWidth(2);
    serie->setPen(pen);

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setLabelsColor(Qt::white);
    axisX->setFormat("dd-MM hh:mm");
    axisX->setGridLineColor(QColor(0x46, 0x46, 0x46));

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelsColor(Qt::white);

    axisY->setLabelFormat(axisLabelFormat);
    axisY->setGridLineColor(QColor("#464646"));

    chart->addSeries(serie);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    serie->attachAxis(axisX);
    serie->attachAxis(axisY);
}
