#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ground_station.h"

#include <QMainWindow>

class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void connect_drone();
    void disconnect_drone();
    void receive_altitude();
    void save_mission_parameters();
    void update_view();
    void show_message(const QString& text);

    GroundStation station_{QStringLiteral("База-1")};
    QLineEdit* drone_input_ = nullptr;
    QDoubleSpinBox* altitude_input_ = nullptr;
    QPushButton* connect_button_ = nullptr;
    QPushButton* disconnect_button_ = nullptr;
    QPushButton* receive_button_ = nullptr;
    QLabel* connection_label_ = nullptr;
    QLabel* drone_label_ = nullptr;
    QLabel* record_count_label_ = nullptr;
    QLabel* average_label_ = nullptr;
    QLabel* message_label_ = nullptr;
    QListWidget* history_list_ = nullptr;
    QMap<QString, QDoubleSpinBox*> mission_inputs_;
    QMap<QString, QLabel*> mission_values_;
    QPushButton* save_mission_button_ = nullptr;
};

#endif
