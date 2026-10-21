#ifndef GROUND_STATION_H
#define GROUND_STATION_H

#include <QMap>
#include <QString>
#include <QVector>

class QDebug;

class GroundStation {
public:
    explicit GroundStation(const QString& name);

    void connect_drone(const QString& drone);
    void disconnect_drone();
    bool is_connected() const;
    void receive_altitude(double altitude);
    double average_altitude() const;
    void print_status() const;
    friend QDebug operator<<(QDebug debug, const GroundStation& station);

    void set_mission_parameter(const QString& name, double value);
    double get_mission_parameter(const QString& name) const;
    bool has_mission_parameter(const QString& name) const;

    QString station_name() const;
    QString drone_name() const;
    const QVector<double>& altitude_history() const;

private:
    QString station_name_;
    QString drone_name_;
    bool connected_ = false;
    QVector<double> altitude_history_;
    QMap<QString, double> mission_parameters_;
};

#endif
