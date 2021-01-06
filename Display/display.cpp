#include "display.h"

#include "dbmanager.h"
#include "ui_display.h"

Display::Display(QWidget* parent) : QMainWindow(parent), ui(new Ui::Display)
{
    ui->setupUi(this);
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_display()));
    QTimer::singleShot(0, this, SLOT(update_display()));
    timer->start(3000);
    startTimer(1000);  // 1-second timer for timeDisplay
}

Display::~Display()
{
    delete ui;
}

void Display::update_display()
{
    QMap<QString, QVariant> mapValue;
    DbManager dbManager("/home/lala/Workspace/INSA-5eme/SmartHome/Github-PFE-Smart-Home/Display/data.db");
    //    DbManager dbManager("/mnt/Etudie/5eme_Annee_S1/PFE/Code-projet/PFE-Smart-Home/Data/data.db");

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

void Display::timerEvent(QTimerEvent* /*event*/)
{
    ui->TimeDisplay->setText(QTime::currentTime().toString("hh:mm:ss"));
}
