#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QMap>

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
    void timerEvent(QTimerEvent* event);

private slots:
    void update_home_data_display();
    void exit();

private:
    void initialize_display();
    void update_data();

private:
    Ui::Display* ui;
    QMap<QString, QVariant> mapValue;
    QString temperature, humidity, mouvement, luminosity;
};
#endif  // DISPLAY_H
