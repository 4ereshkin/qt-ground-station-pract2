#include "ground_station.h"

#include <QDebug>
#include <cmath>

GroundStation::GroundStation(const QString& name)
    : station_name_(name) {}

void GroundStation::connect_drone(const QString& drone) {
    if (connected_) {
        qDebug() << "У станции уже есть дрон";
        return;
    }
    drone_name_ = drone;
    connected_ = true;
    qDebug() << "Подключён дрон:" << drone_name_;
}

void GroundStation::disconnect_drone() {
    if (!connected_) {
        qDebug() << "Дронов не обнаружено";
        return;
    }
    qDebug() << "Отключён дрон:" << drone_name_;
    drone_name_.clear();
    connected_ = false;
    altitude_history_.clear();
}

bool GroundStation::is_connected() const {
    return connected_;
}

void GroundStation::receive_altitude(double altitude) {
    if (!connected_) {
        qDebug() << "Отказ телеметрии: станция не активна";
        return;
    }
    if (!std::isfinite(altitude) || altitude < 0.0) {
        qDebug() << "Отказ телеметрии: некорректная высота";
        return;
    }
    altitude_history_.append(altitude);
}

double GroundStation::average_altitude() const {
    if (altitude_history_.isEmpty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (double altitude : altitude_history_) {
        sum += altitude;
    }
    return sum / altitude_history_.size();
}

void GroundStation::print_status() const {
    qDebug() << *this;
    if (connected_) {
        qDebug() << "Средняя высота:" << average_altitude();
    }
    if (!mission_parameters_.isEmpty()) {
        qDebug() << "Параметры миссии:";
        for (auto it = mission_parameters_.cbegin(); it != mission_parameters_.cend(); ++it) {
            qDebug() << it.key() << it.value();
        }
    }
}

QDebug operator<<(QDebug debug, const GroundStation& station) {
    QDebugStateSaver saver(debug);
    debug.nospace()
        << "GroundStation(name=" << station.station_name_
        << ", drone=" << station.drone_name_
        << ", connected=" << station.connected_
        << ", records=" << station.altitude_history_.size() << ")";
    return debug;
}

void GroundStation::set_mission_parameter(const QString& name, double value) {
    if (!std::isfinite(value)) {
        qWarning() << "Некорректный параметр миссии:" << name << value;
        return;
    }

    bool valid = false;
    if (name == QStringLiteral("target_altitude")
        || name == QStringLiteral("max_speed")) {
        valid = value >= 0.0;
    } else if (name == QStringLiteral("geofence_radius")
               || name == QStringLiteral("mission_timeout")) {
        valid = value > 0.0;
    } else if (name == QStringLiteral("return_battery")) {
        valid = value >= 0.0 && value <= 100.0;
    }

    if (!valid) {
        qWarning() << "Некорректный параметр миссии:" << name << value;
        return;
    }
    mission_parameters_.insert(name, value);
}

double GroundStation::get_mission_parameter(const QString& name) const {
    return mission_parameters_.value(name, 0.0);
}

bool GroundStation::has_mission_parameter(const QString& name) const {
    return mission_parameters_.contains(name);
}

QString GroundStation::station_name() const {
    return station_name_;
}

QString GroundStation::drone_name() const {
    return drone_name_;
}

const QVector<double>& GroundStation::altitude_history() const {
    return altitude_history_;
}
