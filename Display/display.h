#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include <QtCharts>

#include "datavisualization.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class Display;
}
QT_END_NAMESPACE

class Display : public QMainWindow
{
    Q_OBJECT
public:
    Display(QWidget* parent = nullptr);
    ~Display();

protected:
    void timerEvent(QTimerEvent* event = nullptr);

private slots:
    void update_home_data_display();
    void update_indoor_charts();
    void update_outdoor_charts();
    void exitButton_clicked();
    void page1Button_clicked();
    void page2Button_clicked();
    void page3Button_clicked();
    void blink_indoor_mov_warning();
    void blink_outdoor_mov_warning();

private:
    void initialize_display();
    void update_data();

private:  // UI
    Ui::Display* ui;

private:
    QMap<QString, QVariant> mapValue;
    QString temperature, humidity, mouvement, luminosity;

    QChart* tempChart;
    QChart* humChart;
    DataVisualization* dataVisual;
    QTimer* homeDisplayTimer;
    QTimer* indoorChartsTimer;
    QTimer* outdoorChartsTimer;
    QTimer* indoorMovWarnTimer;
    QTimer* outdoorMovWarnTimer;
};
#endif  // DISPLAY_H
