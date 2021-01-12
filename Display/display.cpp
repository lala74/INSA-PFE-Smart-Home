#include "display.h"

#include <QtCharts>

#include "constants.h"
#include "dbmanager.h"
#include "ui_display.h"

using namespace QtCharts;

Display::Display(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::Display),
      tempChart(nullptr),
      humChart(nullptr),
      dataVisual(nullptr),
      homeDisplayTimer(nullptr),
      indoorChartsTimer(nullptr),
      outdoorChartsTimer(nullptr)
{
    ui->setupUi(this);
    // Initialize value
    ui->IndoorLumProgressBar->setMinimum(0);
    ui->IndoorLumProgressBar->setMaximum(1000);
    ui->OutdoorLumProgressBar->setMinimum(0);
    ui->OutdoorLumProgressBar->setMaximum(1000);
    // Initialize the display
    initialize_display();
    dataVisual = new DataVisualization();
    // Link element
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
    homeDisplayTimer->start(display::timer::homeDisplay);
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
    indoorChartsTimer->start(display::timer::charts);
    outdoorChartsTimer->stop();
    ui->pages->setCurrentIndex(1);
}

void Display::page3Button_clicked()
{
    update_outdoor_charts();
    indoorChartsTimer->stop();
    outdoorChartsTimer->start(display::timer::charts);
    ui->pages->setCurrentIndex(2);
}

void Display::initialize_display()
{
    timerEvent();
    update_home_data_display();

#ifdef QT_NO_DEBUG_OUTPUT
    ui->ExitButton->hide();
#endif
}

void Display::update_home_data_display()
{
    mapValue = DbManager::instance().get_outdoor_data();
    update_data();
    ui->OutdoorTemp->setText(temperature);
    ui->OutdoorHum->setText(humidity);
    //    ui->OutdoorMov->setText(mouvement);
    QPropertyAnimation* indoorLumProgressBarAnimation = new QPropertyAnimation(ui->OutdoorLumProgressBar, "value");
    indoorLumProgressBarAnimation->setDuration(500);
    indoorLumProgressBarAnimation->setStartValue(ui->OutdoorLumProgressBar->value());
    indoorLumProgressBarAnimation->setEndValue(luminosity.toInt());
    indoorLumProgressBarAnimation->start();

    mapValue = DbManager::instance().get_indoor_data();
    update_data();

    qDebug() << database::column::device << mapValue.value(database::column::device).toString();
    qDebug() << database::column::sensor_id << mapValue.value(database::column::sensor_id).toString();
    qDebug() << database::column::timestamp << mapValue.value(database::column::timestamp).toString();
    qDebug() << database::column::temperature << temperature;
    qDebug() << database::column::humidity << humidity;
    qDebug() << database::column::mouvement << mouvement;
    qDebug() << database::column::luminosity << luminosity;

    ui->IndoorTemp->setText(temperature);
    ui->IndoorHum->setText(humidity);
    //    ui->IndoorMov->setText(mouvement);
    QPropertyAnimation* outdoorLumProgressBarAnimation = new QPropertyAnimation(ui->IndoorLumProgressBar, "value");
    outdoorLumProgressBarAnimation->setDuration(500);
    outdoorLumProgressBarAnimation->setStartValue(ui->IndoorLumProgressBar->value());
    outdoorLumProgressBarAnimation->setEndValue(luminosity.toInt());
    outdoorLumProgressBarAnimation->start();
}

void Display::update_data()
{
    temperature = QString::number(mapValue.value(database::column::temperature).toDouble(), 'f', 1);
    humidity = QString::number(mapValue.value(database::column::humidity).toDouble(), 'f', 0);
    mouvement = mapValue.value(database::column::mouvement).toString();
    luminosity = QString::number(mapValue.value(database::column::luminosity).toDouble(), 'f', 0);
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
