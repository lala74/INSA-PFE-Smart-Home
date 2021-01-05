#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>

class DbManager : public QObject
{
    Q_OBJECT
public:
    DbManager(const QString& path);
    QMap<QString, QVariant> get_outdoor_data();
    QMap<QString, QVariant> get_indoor_data();

private:
    QMap<QString, QVariant> get_data_follow_by_sensor_id(const QString& sensor_id);

private:
    QSqlDatabase m_db;
};

#endif  // DBMANAGER_H
