#include "display.h"

#include "constants.h"
#include "dbmanager.h"
#include "ui_display.h"

Display::Display(QWidget* parent) : QMainWindow(parent), ui(new Ui::Display)
{
    ui->setupUi(this);
    // Initialize the display
    initialize_display();
    // Start timer
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_home_data_display()));
    timer->start(display::timer::refreshtime);
    startTimer(1000);  // 1-second timer for timeDisplay
}

void Display::initialize_display()
{
    update_home_data_display();
    ui->TimeDisplay->display(QTime::currentTime().toString("hh:mm:ss"));
}

void Display::update_home_data_display()
{
    DbManager dbManager("/home/lala/Workspace/INSA-5eme/SmartHome/Github-PFE-Smart-Home/Display/data.db");
    //    DbManager dbManager("/mnt/Etudie/5eme_Annee_S1/PFE/Code-projet/PFE-Smart-Home/Data/data.db");

    mapValue = dbManager.get_outdoor_data();
    update_data();
    ui->IndoorTemp->setText(temperature);
    ui->IndoorHum->setText(humidity);
    ui->IndoorMov->setText(mouvement);
    ui->IndoorLum->setText(luminosity);

    qDebug() << database::column::device << mapValue.value(database::column::device).toString();
    qDebug() << database::column::sensor_id << mapValue.value(database::column::sensor_id).toString();
    qDebug() << database::column::timestamp << mapValue.value(database::column::timestamp).toString();
    qDebug() << database::column::temperature << temperature;
    qDebug() << database::column::humidity << humidity;
    qDebug() << database::column::mouvement << mouvement;
    qDebug() << database::column::luminosity << luminosity;

    mapValue = dbManager.get_indoor_data();
    update_data();
    ui->OutdoorTemp->setText(temperature);
    ui->OutdoorHum->setText(humidity);
    ui->OutdoorMov->setText(mouvement);
    ui->OutdoorLum->setText(luminosity);
}

void Display::update_data()
{
    temperature = QString::number(mapValue.value(database::column::temperature).toDouble(), 'f', 1) + " °C";
    humidity = QString::number(mapValue.value(database::column::humidity).toDouble(), 'f', 1) + " %";
    mouvement = mapValue.value(database::column::mouvement).toString();
    luminosity = QString::number(mapValue.value(database::column::luminosity).toDouble(), 'f', 1);
}

void Display::timerEvent(QTimerEvent* /*event*/)
{
    ui->TimeDisplay->display(QTime::currentTime().toString("hh:mm:ss"));
}

Display::~Display()
{
    delete ui;
}
