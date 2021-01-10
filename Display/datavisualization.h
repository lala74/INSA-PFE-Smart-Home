#ifndef DATAVISUALIZATION_H
#define DATAVISUALIZATION_H

#include <QObject>
#include <QtCharts>

class DataVisualization : public QObject
{
    Q_OBJECT
public:
    DataVisualization();
    QChart* get_charts(const QString& sensorId, const QString& dataName);

private:
    QMap<QDateTime, QMap<QString, QVariant>> get_data_by_sensor_id(const QString& sensorId);
};

#endif  // DATAVISUALIZATION_H
