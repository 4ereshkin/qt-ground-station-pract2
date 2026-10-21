#include "ground_station.h"

#include <QDebug>
#include <cassert>
#include <cmath>
#include <limits>

int main() {
    GroundStation station(QStringLiteral("База-1"));
    assert(!station.is_connected());
    assert(station.average_altitude() == 0.0);

    station.receive_altitude(100.0);
    assert(station.altitude_history().isEmpty());

    station.connect_drone(QStringLiteral("Survey-01"));
    station.connect_drone(QStringLiteral("Cargo-01"));
    assert(station.is_connected());
    assert(station.drone_name() == QStringLiteral("Survey-01"));

    station.receive_altitude(100.0);
    station.receive_altitude(-5.0);
    station.receive_altitude(std::numeric_limits<double>::quiet_NaN());
    station.receive_altitude(120.0);
    station.receive_altitude(140.0);
    assert(station.altitude_history().size() == 3);
    assert(std::abs(station.average_altitude() - 120.0) < 1e-9);
    QString connected_status;
    QDebug(&connected_status) << station;
    assert(connected_status.contains(QStringLiteral("name=\"База-1\"")));
    assert(connected_status.contains(QStringLiteral("drone=\"Survey-01\"")));
    assert(connected_status.contains(QStringLiteral("connected=true")));
    assert(connected_status.contains(QStringLiteral("records=3")));

    station.disconnect_drone();
    assert(!station.is_connected());
    assert(station.drone_name().isEmpty());
    assert(station.altitude_history().isEmpty());
    assert(station.average_altitude() == 0.0);
    QString disconnected_status;
    QDebug(&disconnected_status) << station;
    assert(disconnected_status.contains(QStringLiteral("connected=false")));
    assert(disconnected_status.contains(QStringLiteral("records=0")));

    station.connect_drone(QStringLiteral("Cargo-01"));
    assert(station.drone_name() == QStringLiteral("Cargo-01"));
    assert(station.altitude_history().isEmpty());

    station.set_mission_parameter(QStringLiteral("target_altitude"), 120.0);
    station.set_mission_parameter(QStringLiteral("max_speed"), 15.0);
    station.set_mission_parameter(QStringLiteral("geofence_radius"), 1000.0);
    station.set_mission_parameter(QStringLiteral("return_battery"), 25.0);
    station.set_mission_parameter(QStringLiteral("mission_timeout"), 600.0);
    assert(station.get_mission_parameter(QStringLiteral("target_altitude")) == 120.0);
    assert(station.get_mission_parameter(QStringLiteral("max_speed")) == 15.0);
    assert(station.get_mission_parameter(QStringLiteral("geofence_radius")) == 1000.0);
    assert(station.get_mission_parameter(QStringLiteral("return_battery")) == 25.0);
    assert(station.get_mission_parameter(QStringLiteral("mission_timeout")) == 600.0);
    assert(!station.has_mission_parameter(QStringLiteral("unknown")));
    assert(station.get_mission_parameter(QStringLiteral("unknown")) == 0.0);

    station.set_mission_parameter(QStringLiteral("target_altitude"), -1.0);
    station.set_mission_parameter(QStringLiteral("max_speed"), -1.0);
    station.set_mission_parameter(QStringLiteral("geofence_radius"), 0.0);
    station.set_mission_parameter(QStringLiteral("return_battery"), 101.0);
    station.set_mission_parameter(QStringLiteral("mission_timeout"), 0.0);
    station.set_mission_parameter(QStringLiteral("target_altitude"),
                                  std::numeric_limits<double>::quiet_NaN());
    station.set_mission_parameter(QStringLiteral("unknown"), 5.0);
    assert(station.get_mission_parameter(QStringLiteral("target_altitude")) == 120.0);
    assert(station.get_mission_parameter(QStringLiteral("max_speed")) == 15.0);
    assert(station.get_mission_parameter(QStringLiteral("geofence_radius")) == 1000.0);
    assert(station.get_mission_parameter(QStringLiteral("return_battery")) == 25.0);
    assert(station.get_mission_parameter(QStringLiteral("mission_timeout")) == 600.0);
    assert(!station.has_mission_parameter(QStringLiteral("unknown")));

    station.disconnect_drone();
    assert(station.get_mission_parameter(QStringLiteral("target_altitude")) == 120.0);
    station.print_status();
}
