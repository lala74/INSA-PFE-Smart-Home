#include "datavisualization.h"

#include "constants.h"
#include "dbmanager.h"

DataVisualization::DataVisualization() {}

QChart* DataVisualization::get_charts(const QString& sensorId, const QString& dataName)
{
    QMap<QDateTime, QMap<QString, QVariant>> data = get_data_by_sensor_id(sensorId);
    QChart* chart = new QChart();
    QLineSeries* series = new QLineSeries();
    QString axisLabelFormat{"%i"};

    for(auto const& key : data.keys()) {
        series->append(key.toMSecsSinceEpoch(), data.value(key).value(dataName).toDouble());
    }
    // Setting series line
    QRgb seriesColor(0xFFFFFF);
    if(sensorId.compare(sensorID::indoor, Qt::CaseSensitive) == 0) {
        seriesColor = QRgb(0x78C745);
    } else if(sensorId.compare(sensorID::outdoor, Qt::CaseSensitive) == 0) {
        seriesColor = QRgb(0xFF8A27);
    }
    QPen pen(seriesColor);
    pen.setWidth(2);
    series->setPen(pen);
    // Set title font
    QFont font;
    font.setPixelSize(18);
    font.setBold(true);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setTitle(QString(dataName));
    // Set background
    chart->setBackgroundBrush(QColor(0x26, 0x26, 0x26));

    chart->legend()->hide();

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setLabelsColor(Qt::white);
    axisX->setFormat("dd-MM hh:mm");
    axisX->setGridLineColor(QColor(0x46, 0x46, 0x46));

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelsColor(Qt::white);
    if(dataName.compare(database::column::temperature, Qt::CaseSensitive) == 0) {
        axisLabelFormat = "%i C";
    } else if(dataName.compare(database::column::humidity, Qt::CaseSensitive) == 0) {
        axisLabelFormat = "%i %";
    }
    axisY->setLabelFormat(axisLabelFormat);
    axisY->setGridLineColor(QColor("#464646"));

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    return chart;
}

QMap<QDateTime, QMap<QString, QVariant>> DataVisualization::get_data_by_sensor_id(const QString& sensorId)
{
    QDateTime startTime, endTime;
    QDate startDate = QDate::currentDate();
    startDate = startDate.addDays(-3);

    startTime.setDate(startDate);
    startTime.setTime(QTime::currentTime());

    endTime.setDate(QDate::currentDate());
    endTime.setTime(QTime::currentTime());

    return DbManager::instance().get_data_follow_by_sensor_id_and_time_interval(sensorId, startTime, endTime);
}
