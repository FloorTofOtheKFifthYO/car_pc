#ifndef SERIAL_H
#define SERIAL_H

#include <QMainWindow>
#include <QByteArray>
#include <QString>
#include <QTimer>

class QSerialPort;

namespace Ui {
class Serial;
}

class Serial : public QMainWindow
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    ~Serial();

private slots:
    void clock();

    void portclose();

    void on_bt_connect_clicked();

    void on_bt_disconect_clicked();

    void on_sand_clicked();
    void update();

    void on_posPrint_clicked();

    void on_toStart_clicked();

    void on_toLoad_clicked();

    void on_toPos_clicked();

    void on_posSelect_clicked();

    void on_target_clicked();

    void on_printLeft_clicked();

    void on_printRight_clicked();

    void on_pitchLeft_clicked();

    void on_yawLeft_clicked();

    void on_speedLeft_clicked();


    void on_pitchRight_clicked();

    void on_yawRight_clicked();

    void on_speedRight_clicked();

    void on_nextLeft_clicked();

    void on_nextRight_clicked();

    void on_left_add_clicked();

    void on_left_modi_clicked();

    void on_right_add_clicked();

    void on_right_modi_clicked();

    void on_start_clicked();

    void on_stop_clicked();

    void on_auto_print_clicked();

    void on_left_target_clicked();

    void on_right_target_clicked();

    void on_home_clicked();

    void on_left_fly_clicked();

    void on_right_fly_clicked();

    void on_left_next_clicked();

    void on_right_next_clicked();

    void on_pos_save_clicked();

    void on_pushButton_clicked();

    void on_switch_red_clicked();

    void on_switch_blue_clicked();

    void on_left_pitch_bt_clicked();

    void on_left_yaw_bt_clicked();

    void on_left_speed_bt_clicked();

    void on_right_pitch_bt_clicked();

    void on_right_yaw_bt_clicked();

    void on_right_speed_bt_clicked();

    void on_debug_clicked();


    void on_debug_print_clicked();


    void on_chassis_read_clicked();

    void on_chassis_save_clicked();

    void on_speedmax_bt_clicked();

    void on_movespeed_bt_clicked();

    void on_anglespeed_bt_clicked();

    void on_moveradium_bt_clicked();

    void on_angleradium_bt_clicked();

    void on_start_bt_clicked();

    void on_speedmin_bt_clicked();

    void on_xfactor_bt_clicked();

    void on_factor_bt_clicked();

    void on_go_clicked();

    void on_modi_pos_clicked();

    void on_load_clicked();

    void on_auto_save_clicked();

private:
    Ui::Serial *ui;
    QSerialPort *serialport;
    QByteArray requestData;
    QString msg;
    QTimer *timer;
    QString x;
    QString y;
    QString ang;
};

#endif // SERIAL_H
