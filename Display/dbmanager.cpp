#include "dbmanager.h"

DbManager::DbManager(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if(!m_db.open()) {
        qDebug() << "Error: connection with database failed";
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
        returnVal.insert("device", query.value("device"));
        returnVal.insert("sensor_id", query.value("sensor_id"));
        returnVal.insert("timestamp", query.value("timestamp"));
        returnVal.insert("temperature", query.value("temperature"));
        returnVal.insert("humidiy", query.value("humidiy"));
        returnVal.insert("mouvement", query.value("mouvement"));
        returnVal.insert("luminosity", query.value("luminosity"));
    }
    return returnVal;
}
