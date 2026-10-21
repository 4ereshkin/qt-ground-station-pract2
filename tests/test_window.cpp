#include "main_window.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QtTest/QTest>

#include <cassert>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setFont(QFont(QStringLiteral("Segoe UI"), 10));
    MainWindow window;
    window.show();

    auto* drone = window.findChild<QLineEdit*>(QStringLiteral("droneInput"));
    auto* altitude = window.findChild<QDoubleSpinBox*>(QStringLiteral("altitudeInput"));
    auto* connect = window.findChild<QPushButton*>(QStringLiteral("connectButton"));
    auto* disconnect = window.findChild<QPushButton*>(QStringLiteral("disconnectButton"));
    auto* receive = window.findChild<QPushButton*>(QStringLiteral("receiveButton"));
    auto* history = window.findChild<QListWidget*>(QStringLiteral("historyList"));
    auto* average = window.findChild<QLabel*>(QStringLiteral("averageAltitude"));
    auto* target = window.findChild<QDoubleSpinBox*>(QStringLiteral("missionInput_target_altitude"));
    auto* radius = window.findChild<QDoubleSpinBox*>(QStringLiteral("missionInput_geofence_radius"));
    auto* battery = window.findChild<QDoubleSpinBox*>(QStringLiteral("missionInput_return_battery"));
    auto* saved_target = window.findChild<QLabel*>(QStringLiteral("missionValue_target_altitude"));
    auto* saved_radius = window.findChild<QLabel*>(QStringLiteral("missionValue_geofence_radius"));
    auto* saved_battery = window.findChild<QLabel*>(QStringLiteral("missionValue_return_battery"));
    auto* save = window.findChild<QPushButton*>(QStringLiteral("saveMissionButton"));
    auto* tabs = window.findChild<QTabWidget*>(QStringLiteral("mainTabs"));
    assert(drone && altitude && connect && disconnect && receive && history && average);
    assert(target && radius && battery && saved_target && saved_radius && saved_battery && save);
    assert(tabs && tabs->count() == 2);
    assert(!receive->isEnabled());
    assert(saved_target->text() == QStringLiteral("Не задано"));

    drone->setText(QStringLiteral("Survey-01"));
    QTest::mouseClick(connect, Qt::LeftButton);
    assert(receive->isEnabled());
    altitude->setValue(100.0);
    QTest::mouseClick(receive, Qt::LeftButton);
    altitude->setValue(140.0);
    QTest::mouseClick(receive, Qt::LeftButton);
    assert(history->count() == 2);
    assert(average->text().contains(QStringLiteral("120.00")));

    target->setValue(130.0);
    radius->setValue(1100.0);
    battery->setValue(30.0);
    tabs->setCurrentIndex(1);
    QTest::mouseClick(save, Qt::LeftButton);
    assert(saved_target->text().contains(QStringLiteral("130.00")));
    assert(saved_radius->text().contains(QStringLiteral("1100.00")));
    assert(saved_battery->text().contains(QStringLiteral("30.0")));

    const QString preview_dir = QCoreApplication::applicationDirPath();
    assert(window.grab().save(preview_dir + QStringLiteral("/mission-preview.png")));
    tabs->setCurrentIndex(0);
    assert(window.grab().save(preview_dir + QStringLiteral("/window-preview.png")));

    QTest::mouseClick(disconnect, Qt::LeftButton);
    assert(history->count() == 0);
    assert(!receive->isEnabled());
    assert(saved_target->text().contains(QStringLiteral("130.00")));
    return 0;
}
