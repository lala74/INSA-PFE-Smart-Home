#include "dbmanager.h"

#include "constants.h"

DbManager::DbManager(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if(!m_db.open()) {
        qWarning() << "Database: Connection failed";
    } else {
        qDebug() << "Database: connection ok";
    }
}

QMap<QString, QVariant> DbManager::get_outdoor_data()
{
    return get_data_follow_by_sensor_id("sensorOutdoor");
}
QMap<QString, QVariant> DbManager::get_indoor_data()
{
    return get_data_follow_by_sensor_id("sensorIndoor");
}

QMap<QString, QVariant> DbManager::get_data_follow_by_sensor_id(const QString& sensor_id)
{
    QMap<QString, QVariant> returnVal;
    QSqlQuery query;
    QString cmd = "SELECT * FROM home_sensor WHERE sensor_id = '" + sensor_id + "' ORDER BY timestamp DESC LIMIT 1";
    query.exec(cmd);
    if(query.next()) {
        qDebug() << "Database: query ok";
        returnVal.insert(database::column::device, query.value(database::column::device));
        returnVal.insert(database::column::sensor_id, query.value(database::column::sensor_id));
        returnVal.insert(database::column::timestamp, query.value(database::column::timestamp));
        returnVal.insert(database::column::temperature, query.value(database::column::temperature));
        returnVal.insert(database::column::humidity, query.value(database::column::humidity));
        returnVal.insert(database::column::mouvement, query.value(database::column::mouvement));
        returnVal.insert(database::column::luminosity, query.value(database::column::luminosity));
    } else {
        qWarning() << "Database: Query failed";
    }
    return returnVal;
}
