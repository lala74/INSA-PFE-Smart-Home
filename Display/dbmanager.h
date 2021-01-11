#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtCharts>
#include <QtSql>

#include "constants.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
    static DbManager& instance()
    {
        static DbManager* _instance = 0;
        if(_instance == 0) {
            _instance = new DbManager(database::path);
        }
        return *_instance;
    }
    QMap<QString, QVariant> get_outdoor_data();
    QMap<QString, QVariant> get_indoor_data();
    qint8 get_data_follow_by_sensor_id_and_time_interval(const QString& sensor_id,
                                                         QDateTime startTime,
                                                         QDateTime endTime,
                                                         QLineSeries* tempSeries,
                                                         QLineSeries* humSeries);

private:
    explicit DbManager(const QString& path);
    explicit DbManager(const DbManager&) {}

private:
    QMap<QString, QVariant> get_last_data_follow_by_sensor_id(const QString& sensor_id);

private:
    QSqlDatabase m_db;
};

#endif  // DBMANAGER_H
