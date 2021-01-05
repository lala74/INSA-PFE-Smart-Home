#include "display.h"

#include "dbmanager.h"
#include "ui_display.h"

Display::Display(QWidget* parent) : QMainWindow(parent), ui(new Ui::Display)
{
    ui->setupUi(this);
    startTimer(1000);  // 1-second timer

    ui->IndoorTemp->setText("0 °C");
    ui->IndoorHum->setText("80 %");
    ui->IndoorMov->setText("True");
    ui->IndoorLum->setText("1");
    ui->OutdoorTemp->setText("0 °C");
    ui->OutdoorHum->setText("80 %");
    ui->OutdoorMov->setText("True");
    ui->OutdoorLum->setText("1");

    QMap<QString, QVariant> mapValue;

    DbManager dbManager("/home/lala/Workspace/INSA-5eme/SmartHome/Github-PFE-Smart-Home/Display/data.db");
    mapValue = dbManager.get_outdoor_data();
    qDebug() << "device:" << mapValue.value("device").toString();
    qDebug() << "sensor_id:" << mapValue.value("sensor_id").toString();
    qDebug() << "timestamp:" << mapValue.value("timestamp").toString();
    qDebug() << "temperature:" << mapValue.value("temperature").toDouble();
    qDebug() << "humidiy:" << mapValue.value("humidiy").toDouble();
    qDebug() << "mouvement:" << mapValue.value("mouvement").toDouble();
    qDebug() << "luminosity:" << mapValue.value("luminosity").toDouble();

    ui->IndoorTemp->setText(QString::number(mapValue.value("temperature").toDouble(), 'f', 1) + " °C");
    ui->IndoorHum->setText(QString::number(mapValue.value("humidiy").toDouble(), 'f', 1) + " %");
    ui->IndoorMov->setText(mapValue.value("mouvement").toString());
    ui->IndoorLum->setText(QString::number(mapValue.value("luminosity").toDouble(), 'f', 1));

    mapValue = dbManager.get_indoor_data();

    ui->OutdoorTemp->setText(QString::number(mapValue.value("temperature").toDouble(), 'f', 1) + " °C");
    ui->OutdoorHum->setText(QString::number(mapValue.value("humidiy").toDouble(), 'f', 1) + " %");
    ui->OutdoorMov->setText(mapValue.value("mouvement").toString());
    ui->OutdoorLum->setText(QString::number(mapValue.value("luminosity").toDouble(), 'f', 1));
}

Display::~Display()
{
    delete ui;
}

void Display::timerEvent(QTimerEvent* event)
{
    ui->TimeDisplay->setText(QTime::currentTime().toString("hh:mm:ss"));
    //    setWindowTitle();
}
