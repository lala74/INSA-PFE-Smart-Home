#include "datavisualization.h"

#include "constants.h"
#include "dbmanager.h"

DataVisualization::DataVisualization() {}

QChart* DataVisualization::get_charts()
{
    QLineSeries* series = new QLineSeries();
    QFile sunSpots(":sun");
    if(!sunSpots.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Open chart faileddddddddddddddddddddd";
    }

    QTextStream stream(&sunSpots);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if(line.startsWith("#") || line.startsWith(":")) continue;
        QStringList values = line.split(" ", QString::SkipEmptyParts);
        QDateTime momentInTime;
        momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt(), 15));
        series->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
    }
    sunSpots.close();
    //![2]

    //![3]
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Sunspots count (by Space Weather Prediction Center)");
    //![3]

    //![4]
    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Sunspots count LAAAAAAAAAAAA");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return chart;
}

void DataVisualization::update_outdoor_datas()
{
    outdoorDatas = get_data_by_sensor_id(sensorID::outdoor);
}

void DataVisualization::update_indoor_datas()
{
    outdoorDatas = get_data_by_sensor_id(sensorID::indoor);
}

QMap<QDateTime, QMap<QString, QVariant>> DataVisualization::get_data_by_sensor_id(const QString& sensorId)
{
    DbManager database(database::path);
    QDateTime startTime, endTime;
    QDate startDate = QDate::currentDate();
    startDate = startDate.addDays(-3);
    startTime.setDate(startDate);
    startTime.setTime(QTime::currentTime());
    endTime.setDate(QDate::currentDate());
    endTime.setTime(QTime::currentTime());
    return database.get_data_follow_by_sensor_id_and_time_interval(sensorId, startTime, endTime);
}
