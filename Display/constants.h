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
    const QString humidity{"humidiy"};
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
    const quint16 refreshtime{3000};  // refresh display every 3s
}
}  // namespace display
#endif  // CONSTANTS_H
