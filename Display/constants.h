#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtCore/QString>

namespace database
{
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

namespace display
{
namespace timer
{
    const quint16 refreshtime{3000};  // refresh display every 3s
}
}  // namespace display
#endif  // CONSTANTS_H
