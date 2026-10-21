#include "main_window.h"

#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace {

QFrame* make_card(QWidget* parent, QVBoxLayout*& content) {
    auto* card = new QFrame(parent);
    card->setObjectName(QStringLiteral("card"));
    content = new QVBoxLayout(card);
    content->setContentsMargins(22, 20, 22, 22);
    content->setSpacing(12);
    return card;
}

QLabel* make_section_title(const QString& title, QWidget* parent) {
    auto* label = new QLabel(title, parent);
    label->setObjectName(QStringLiteral("sectionTitle"));
    return label;
}

QLabel* make_caption(const QString& title, QWidget* parent) {
    auto* label = new QLabel(title, parent);
    label->setObjectName(QStringLiteral("caption"));
    return label;
}

}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral("Наземная станция — задание 3"));
    resize(980, 800);
    setMinimumSize(760, 600);

    auto* root = new QWidget(this);
    root->setObjectName(QStringLiteral("root"));
    setCentralWidget(root);
    auto* page = new QVBoxLayout(root);
    page->setContentsMargins(28, 26, 28, 26);
    page->setSpacing(18);

    auto* title = new QLabel(QStringLiteral("Наземная станция"), root);
    title->setObjectName(QStringLiteral("pageTitle"));
    page->addWidget(title);
    auto* subtitle = new QLabel(
        QStringLiteral("Подключите БПЛА и принимайте значения высоты"), root);
    subtitle->setObjectName(QStringLiteral("subtitle"));
    page->addWidget(subtitle);

    auto* tabs = new QTabWidget(root);
    tabs->setObjectName(QStringLiteral("mainTabs"));
    page->addWidget(tabs, 1);
    auto* station_tab = new QWidget(tabs);
    station_tab->setObjectName(QStringLiteral("stationTab"));
    tabs->addTab(station_tab, QStringLiteral("Станция"));

    auto* columns = new QHBoxLayout();
    columns->setContentsMargins(0, 16, 0, 0);
    columns->setSpacing(18);
    station_tab->setLayout(columns);

    QVBoxLayout* controls = nullptr;
    auto* control_card = make_card(station_tab, controls);
    columns->addWidget(control_card, 4);
    controls->addWidget(make_section_title(QStringLiteral("Управление"), control_card));
    controls->addWidget(make_caption(QStringLiteral("Станция"), control_card));
    auto* station_name = new QLabel(station_.station_name(), control_card);
    station_name->setObjectName(QStringLiteral("stationName"));
    controls->addWidget(station_name);

    controls->addSpacing(8);
    controls->addWidget(make_caption(QStringLiteral("Название БПЛА"), control_card));
    drone_input_ = new QLineEdit(control_card);
    drone_input_->setObjectName(QStringLiteral("droneInput"));
    drone_input_->setPlaceholderText(QStringLiteral("Например, Survey-01"));
    controls->addWidget(drone_input_);

    auto* connection_buttons = new QHBoxLayout();
    connection_buttons->setSpacing(10);
    connect_button_ = new QPushButton(QStringLiteral("Подключить"), control_card);
    connect_button_->setObjectName(QStringLiteral("connectButton"));
    disconnect_button_ = new QPushButton(QStringLiteral("Отключить"), control_card);
    disconnect_button_->setObjectName(QStringLiteral("disconnectButton"));
    connection_buttons->addWidget(connect_button_);
    connection_buttons->addWidget(disconnect_button_);
    controls->addLayout(connection_buttons);

    auto* divider = new QFrame(control_card);
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName(QStringLiteral("divider"));
    controls->addWidget(divider);
    controls->addWidget(make_section_title(QStringLiteral("Приём телеметрии"), control_card));
    controls->addWidget(make_caption(QStringLiteral("Высота, м"), control_card));
    altitude_input_ = new QDoubleSpinBox(control_card);
    altitude_input_->setObjectName(QStringLiteral("altitudeInput"));
    altitude_input_->setRange(0.0, 100000.0);
    altitude_input_->setDecimals(2);
    altitude_input_->setSingleStep(10.0);
    altitude_input_->setSuffix(QStringLiteral(" м"));
    controls->addWidget(altitude_input_);
    receive_button_ = new QPushButton(QStringLiteral("Принять высоту"), control_card);
    receive_button_->setObjectName(QStringLiteral("receiveButton"));
    controls->addWidget(receive_button_);
    controls->addStretch();

    QVBoxLayout* status = nullptr;
    auto* status_card = make_card(station_tab, status);
    columns->addWidget(status_card, 5);
    status->addWidget(make_section_title(QStringLiteral("Состояние"), status_card));

    connection_label_ = new QLabel(status_card);
    connection_label_->setObjectName(QStringLiteral("connectionStatus"));
    status->addWidget(connection_label_);
    drone_label_ = new QLabel(status_card);
    drone_label_->setObjectName(QStringLiteral("droneStatus"));
    record_count_label_ = new QLabel(status_card);
    record_count_label_->setObjectName(QStringLiteral("recordCount"));
    average_label_ = new QLabel(status_card);
    average_label_->setObjectName(QStringLiteral("averageAltitude"));
    status->addWidget(drone_label_);
    status->addWidget(record_count_label_);
    status->addWidget(average_label_);
    status->addSpacing(8);
    status->addWidget(make_caption(QStringLiteral("История высоты"), status_card));
    history_list_ = new QListWidget(status_card);
    history_list_->setObjectName(QStringLiteral("historyList"));
    history_list_->setAlternatingRowColors(true);
    status->addWidget(history_list_, 1);

    auto* mission_tab = new QWidget(tabs);
    mission_tab->setObjectName(QStringLiteral("missionTab"));
    tabs->addTab(mission_tab, QStringLiteral("Параметры миссии"));
    auto* mission_page = new QVBoxLayout(mission_tab);
    mission_page->setContentsMargins(0, 16, 0, 0);
    QVBoxLayout* mission = nullptr;
    auto* mission_card = make_card(mission_tab, mission);
    mission_page->addWidget(mission_card);
    mission->addWidget(make_section_title(QStringLiteral("Параметры миссии"), mission_card));
    mission->addWidget(make_caption(
        QStringLiteral("Введите значения и сохраните их в станции. После закрытия программы они сбросятся."),
        mission_card));

    auto* mission_grid = new QGridLayout();
    mission_grid->setHorizontalSpacing(16);
    mission_grid->setVerticalSpacing(10);
    mission_grid->addWidget(make_caption(QStringLiteral("Параметр"), mission_card), 0, 0);
    mission_grid->addWidget(make_caption(QStringLiteral("Новое значение"), mission_card), 0, 1);
    mission_grid->addWidget(make_caption(QStringLiteral("В станции"), mission_card), 0, 2);

    struct MissionField {
        QString key;
        QString label;
        QString unit;
        double minimum;
        double maximum;
        double initial;
        int decimals;
    };
    const MissionField fields[] = {
        {QStringLiteral("target_altitude"), QStringLiteral("Целевая высота"),
         QStringLiteral(" м"), 0.0, 100000.0, 120.0, 2},
        {QStringLiteral("max_speed"), QStringLiteral("Максимальная скорость"),
         QStringLiteral(" м/с"), 0.0, 1000.0, 15.0, 2},
        {QStringLiteral("geofence_radius"), QStringLiteral("Радиус геозоны"),
         QStringLiteral(" м"), 0.01, 10000000.0, 1000.0, 2},
        {QStringLiteral("return_battery"), QStringLiteral("Заряд для возврата"),
         QStringLiteral(" %"), 0.0, 100.0, 25.0, 1},
        {QStringLiteral("mission_timeout"), QStringLiteral("Время миссии"),
         QStringLiteral(" с"), 1.0, 86400.0, 600.0, 0}
    };
    for (int i = 0; i < 5; ++i) {
        const auto& field = fields[i];
        mission_grid->addWidget(new QLabel(field.label, mission_card), i + 1, 0);
        auto* input = new QDoubleSpinBox(mission_card);
        input->setObjectName(QStringLiteral("missionInput_") + field.key);
        input->setDecimals(field.decimals);
        input->setRange(field.minimum, field.maximum);
        input->setValue(field.initial);
        input->setSuffix(field.unit);
        mission_inputs_.insert(field.key, input);
        mission_grid->addWidget(input, i + 1, 1);
        auto* saved = new QLabel(QStringLiteral("Не задано"), mission_card);
        saved->setObjectName(QStringLiteral("missionValue_") + field.key);
        mission_values_.insert(field.key, saved);
        mission_grid->addWidget(saved, i + 1, 2);
    }
    mission_grid->setColumnStretch(0, 3);
    mission_grid->setColumnStretch(1, 2);
    mission_grid->setColumnStretch(2, 2);
    mission->addLayout(mission_grid);
    save_mission_button_ = new QPushButton(QStringLiteral("Сохранить параметры миссии"), mission_card);
    save_mission_button_->setObjectName(QStringLiteral("saveMissionButton"));
    mission->addWidget(save_mission_button_, 0, Qt::AlignRight);

    message_label_ = new QLabel(root);
    message_label_->setObjectName(QStringLiteral("message"));
    page->addWidget(message_label_);

    root->setStyleSheet(QStringLiteral(R"(
        QWidget#root { background: #f3f6fa; color: #172338; font-size: 14px; }
        QWidget#stationTab, QWidget#missionTab { background: #f3f6fa; }
        QTabWidget::pane { border: none; }
        QTabBar::tab { background: #e7edf5; color: #53657b; padding: 10px 18px;
                       border: 1px solid #d8e1eb; border-bottom: none;
                       border-top-left-radius: 6px; border-top-right-radius: 6px; }
        QTabBar::tab:selected { background: white; color: #1767b3; }
        QLabel#pageTitle { font-size: 27px; font-weight: 700; }
        QLabel#subtitle, QLabel#caption { color: #617187; }
        QLabel#sectionTitle { font-size: 18px; font-weight: 600; }
        QLabel#stationName { font-size: 17px; font-weight: 600; }
        QFrame#card { background: white; border: 1px solid #dde5ed; border-radius: 12px; }
        QFrame#divider { border: none; border-top: 1px solid #e4e9f0; }
        QLineEdit, QDoubleSpinBox, QListWidget {
            background: white; border: 1px solid #cbd5e1; border-radius: 6px;
            padding: 7px; min-height: 25px;
        }
        QListWidget { alternate-background-color: #f5f8fb; }
        QPushButton { border: 1px solid #becbd8; border-radius: 6px;
                      background: #f8fafc; padding: 9px 12px; }
        QPushButton:hover { background: #eaf0f6; }
        QPushButton#connectButton, QPushButton#receiveButton, QPushButton#saveMissionButton {
            background: #1767b3; color: white; border-color: #1767b3;
        }
        QPushButton#connectButton:hover, QPushButton#receiveButton:hover,
        QPushButton#saveMissionButton:hover { background: #0f548f; }
        QPushButton:disabled { color: #8a98a9; background: #e8edf2; border-color: #e8edf2; }
        QLabel#connectionStatus { font-weight: 600; }
        QLabel#message { color: #53657b; }
    )"));

    connect(connect_button_, &QPushButton::clicked, this, [this] { connect_drone(); });
    connect(disconnect_button_, &QPushButton::clicked, this, [this] { disconnect_drone(); });
    connect(receive_button_, &QPushButton::clicked, this, [this] { receive_altitude(); });
    connect(save_mission_button_, &QPushButton::clicked, this,
            [this] { save_mission_parameters(); });
    connect(drone_input_, &QLineEdit::returnPressed, this, [this] { connect_drone(); });

    show_message(QStringLiteral("Введите название БПЛА для подключения."));
    update_view();
}

void MainWindow::connect_drone() {
    const QString drone = drone_input_->text().trimmed();
    if (drone.isEmpty()) {
        show_message(QStringLiteral("Введите название БПЛА."));
        return;
    }
    if (station_.is_connected()) {
        show_message(QStringLiteral("Сначала отключите текущий БПЛА."));
        return;
    }
    station_.connect_drone(drone);
    show_message(QStringLiteral("Подключён БПЛА %1.").arg(drone));
    update_view();
}

void MainWindow::disconnect_drone() {
    if (!station_.is_connected()) {
        return;
    }
    station_.disconnect_drone();
    show_message(QStringLiteral("БПЛА отключён. История очищена."));
    update_view();
}

void MainWindow::receive_altitude() {
    if (!station_.is_connected()) {
        return;
    }
    const double altitude = altitude_input_->value();
    station_.receive_altitude(altitude);
    show_message(QStringLiteral("Принята высота %1 м.").arg(altitude, 0, 'f', 2));
    update_view();
}

void MainWindow::save_mission_parameters() {
    for (auto it = mission_inputs_.cbegin(); it != mission_inputs_.cend(); ++it) {
        station_.set_mission_parameter(it.key(), it.value()->value());
    }
    show_message(QStringLiteral("Параметры миссии сохранены в станции."));
    update_view();
}

void MainWindow::update_view() {
    const bool connected = station_.is_connected();
    connect_button_->setEnabled(!connected);
    disconnect_button_->setEnabled(connected);
    receive_button_->setEnabled(connected);
    altitude_input_->setEnabled(connected);
    drone_input_->setEnabled(!connected);
    connection_label_->setText(connected
        ? QStringLiteral("● Подключено") : QStringLiteral("○ Нет подключения"));
    connection_label_->setStyleSheet(connected
        ? QStringLiteral("color: #177245;") : QStringLiteral("color: #68798d;"));
    drone_label_->setText(QStringLiteral("БПЛА: %1").arg(
        connected ? station_.drone_name() : QStringLiteral("—")));

    const auto& history = station_.altitude_history();
    record_count_label_->setText(QStringLiteral("Записей: %1").arg(history.size()));
    average_label_->setText(QStringLiteral("Средняя высота: %1 м")
        .arg(station_.average_altitude(), 0, 'f', 2));
    history_list_->clear();
    for (int i = 0; i < history.size(); ++i) {
        history_list_->addItem(QStringLiteral("%1.  %2 м")
            .arg(i + 1).arg(history.at(i), 0, 'f', 2));
    }
    for (auto it = mission_inputs_.cbegin(); it != mission_inputs_.cend(); ++it) {
        const QString& key = it.key();
        auto* saved = mission_values_.value(key);
        if (station_.has_mission_parameter(key)) {
            const auto* input = it.value();
            saved->setText(QString::number(station_.get_mission_parameter(key),
                                          'f', input->decimals()) + input->suffix());
        } else {
            saved->setText(QStringLiteral("Не задано"));
        }
    }
}

void MainWindow::show_message(const QString& text) {
    message_label_->setText(text);
}
