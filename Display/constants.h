#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtCore/QString>

namespace database
{
#ifdef RASPBERRY_PI
const QString path("/home/pi/PFE-Smart-Home/Data/data.db");
#else
const QString path{"/home/lala/Workspace/INSA-5eme/SmartHome/Github-PFE-Smart-Home/Data/data.db"};
#endif

const QString timestampFormat("yy-MM-dd hh:mm:ss");

namespace column
{
    const QString device{"device"};
    const QString sensor_id{"sensor_id"};
    const QString timestamp{"timestamp"};
    const QString temperature{"temperature"};
    const QString humidity{"humidity"};
    const QString mouvement{"mouvement"};
    const QString luminosity{"luminosity"};
}  // namespace column
}  // namespace database

namespace sensorID
{
const QString outdoor{"sensorOutdoor"};
const QString indoor{"sensorIndoor"};
}  // namespace sensorID

namespace display
{
namespace timer
{
    const quint16 homeDisplay{3000};  // refresh display every 3s
    const quint16 charts{5000};       // refresh charts every 5s
    const quint16 warning{500};       // refresh movement warning every 0.5s
}  // namespace timer
}  // namespace display

namespace stylesheet
{
namespace button
{
    const QString normal{
        "QPushButton { background-color:#161616; color:#FFFFFF; font: bold; border:none; } QPushButton:hover { "
        "background-color: #555555; color: white; padding: 5px;} QPushButton:pressed { background-color: #a9a9a9; "
        "color: black; padding: 5px; }"};
    const QString clicked{
        "QPushButton { background-color:#161616; color:#FFFFFF; font: bold; border:none; } QPushButton:hover { "
        "background-color: #555555; color: white; padding: 5px;} QPushButton:pressed { background-color: #a9a9a9; "
        "color: black; padding: 5px; }"};

}  // namespace button
}  // namespace stylesheet

#endif  // CONSTANTS_H
