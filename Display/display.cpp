#include "display.h"

#include <QtCharts>

#include "constants.h"
#include "dbmanager.h"
#include "ui_display.h"

using namespace QtCharts;

Display::Display(QWidget* parent) : QMainWindow(parent), ui(new Ui::Display)
{
    ui->setupUi(this);
    // Initialize the display
    initialize_display();
    // Link element

    dataVisual = new DataVisualization();

    connect(ui->ExitButton, SIGNAL(clicked()), this, SLOT(exitButton_clicked()));
    connect(ui->Page1Button, SIGNAL(clicked()), this, SLOT(page1Button_clicked()));
    connect(ui->Page2Button, SIGNAL(clicked()), this, SLOT(page2Button_clicked()));
    connect(ui->Page3Button, SIGNAL(clicked()), this, SLOT(page3Button_clicked()));
    // Timer
    homeDisplayTimer = new QTimer(this);
    indoorChartsTimer = new QTimer(this);
    outdoorChartsTimer = new QTimer(this);
    connect(homeDisplayTimer, SIGNAL(timeout()), this, SLOT(update_home_data_display()));
    connect(indoorChartsTimer, SIGNAL(timeout()), this, SLOT(update_indoor_charts()));
    connect(outdoorChartsTimer, SIGNAL(timeout()), this, SLOT(update_outdoor_charts()));
    // Start Timer
    homeDisplayTimer->start(display::timer::refreshtime);
    startTimer(1000);  // 1-second timer for timeDisplay
}

void Display::exitButton_clicked()
{
    Display::close();
}

void Display::page1Button_clicked()
{
    ui->pages->setCurrentIndex(0);
    indoorChartsTimer->stop();
    outdoorChartsTimer->stop();
}

void Display::page2Button_clicked()
{
    update_indoor_charts();
    indoorChartsTimer->start(display::timer::refreshtime);
    outdoorChartsTimer->stop();
    ui->pages->setCurrentIndex(1);
}

void Display::page3Button_clicked()
{
    update_outdoor_charts();
    indoorChartsTimer->stop();
    outdoorChartsTimer->start(display::timer::refreshtime);
    ui->pages->setCurrentIndex(2);
}

void Display::initialize_display()
{
    update_home_data_display();
    update_indoor_charts();
    update_outdoor_charts();
    timerEvent();
}

void Display::update_home_data_display()
{
    mapValue = DbManager::instance().get_outdoor_data();
    update_data();
    ui->OutdoorTemp->setText(temperature);
    ui->OutdoorHum->setText(humidity);
    ui->OutdoorMov->setText(mouvement);
    ui->OutdoorLum->setText(luminosity);

    qDebug() << database::column::device << mapValue.value(database::column::device).toString();
    qDebug() << database::column::sensor_id << mapValue.value(database::column::sensor_id).toString();
    qDebug() << database::column::timestamp << mapValue.value(database::column::timestamp).toString();
    qDebug() << database::column::temperature << temperature;
    qDebug() << database::column::humidity << humidity;
    qDebug() << database::column::mouvement << mouvement;
    qDebug() << database::column::luminosity << luminosity;

    mapValue = DbManager::instance().get_indoor_data();
    update_data();
    ui->IndoorTemp->setText(temperature);
    ui->IndoorHum->setText(humidity);
    ui->IndoorMov->setText(mouvement);
    ui->IndoorLum->setText(luminosity);
}

void Display::update_indoor_charts()
{
    QList<QChart*> listCharts;
    listCharts = dataVisual->get_charts(sensorID::indoor);
    tempChart = listCharts.takeFirst();
    humChart = listCharts.takeLast();
    ui->IndoorTempChart->setChart(tempChart);
    ui->IndoorHumChart->setChart(humChart);
}
void Display::update_outdoor_charts()
{
    QList<QChart*> listCharts;
    listCharts = dataVisual->get_charts(sensorID::outdoor);
    tempChart = listCharts.takeFirst();
    humChart = listCharts.takeLast();
    ui->OutdoorTempChart->setChart(tempChart);
    ui->OutdoorHumChart->setChart(humChart);
}

void Display::update_data()
{
    temperature = QString::number(mapValue.value(database::column::temperature).toDouble(), 'f', 1);
    humidity = QString::number(mapValue.value(database::column::humidity).toDouble(), 'f', 0);
    mouvement = mapValue.value(database::column::mouvement).toString();
    luminosity = QString::number(mapValue.value(database::column::luminosity).toDouble(), 'f', 1);
}

void Display::timerEvent(QTimerEvent* /*event*/)
{
    ui->TimeDisplay->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->DateDisplay->setText(QDate::currentDate().toString("dd-MM-yyyy"));
    ui->DayDisplay->setText(QDate::currentDate().toString("dddd"));
}

Display::~Display()
{
    if(ui) delete ui;
    if(dataVisual) delete dataVisual;
    if(tempChart) delete tempChart;
    homeDisplayTimer->stop();
    indoorChartsTimer->stop();
    outdoorChartsTimer->stop();
}
