#include "serial.h"
#include "ui_serial.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>

Serial::Serial(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Serial),
    timer(new QTimer)
{
    serialport = new QSerialPort();
    ui->setupUi(this);
    ui->bt_disconect->setDisabled(true);
    ui->sand->setDisabled(true);
    connect(timer,&QTimer::timeout,this,&Serial::clock);
    connect(serialport,SIGNAL(readyRead()),this,SLOT(update()));
    connect(serialport,&QIODevice::aboutToClose,this,&Serial::portclose);
}

Serial::~Serial()
{
    delete ui;
}

void Serial::clock()
{
    serialport->write("pos now\r\n");
}

void Serial::portclose()
{
    qDebug()<<"asdf\n";
    timer->stop();
}

void Serial::on_bt_connect_clicked()
{
    qDebug()<<"asf\n";
	QString port = ui->serial->text();
    serialport->setPortName(port);
    qDebug()<<"df\n";
    if(serialport->open(QIODevice::ReadWrite)){
        qDebug()<<"dasf\n";
        serialport->setBaudRate(QSerialPort::Baud115200);
        serialport->setDataBits(QSerialPort::Data8);
        serialport->setStopBits(QSerialPort::OneStop);
        serialport->setParity(QSerialPort::NoParity);
        serialport->setFlowControl(QSerialPort::NoFlowControl);

        ui->bt_connect->setDisabled(true);
        ui->bt_disconect->setDisabled(false);
        ui->sand->setDisabled(false);
        timer->stop();
        timer->start(1000);

        serialport->write("pos select 1\r\n");

    }else
        QMessageBox::warning(nullptr,"error","Can not open SerialPort",QMessageBox::Retry);
}

void Serial::update()
{
    QStringList string;
    QStringList buflist;
    QStringList wordlist;
    requestData = serialport->readAll();  //读取数据
    qDebug()<<requestData;
    QString buf(requestData);
    qDebug()<<buf;
    if(!buf.endsWith('\n')){
        msg+=buf;
    }
    else{
        msg+=buf;
        string = msg.split("\r\n");
        msg.clear();

        for(QString buf:string)
        {
            if(buf!= NULL)
            {
                if(buf == "msg: Let's go!"){
                    serialport->write("pos select 1\r\n");
                }

                if(buf == "msg:left(0) or right(1):0")
                    ui->switch_side->setText(QString::fromLocal8Bit("红场"));
                else if(buf == "msg:left(0) or right(1):1")
                    ui->switch_side->setText(QString::fromLocal8Bit("蓝场"));

                if(buf.startsWith("x"))
                {
                    buflist = buf.split(' ');
                    for(QString word:buflist)
                    {
                        wordlist = word.split(':');
                        if(wordlist[0] == "x")
                        {
                            ui->x->setText("x:"+wordlist[1]);
                            x = wordlist[1];
                        }else if(wordlist[0] == "y")
                        {
                            ui->y->setText("y:"+wordlist[1]);
                            y = wordlist[1];
                        }else if(wordlist[0] == "yaw")
                        {
                            ui->ang->setText("ang:"+wordlist[1]);
                            ang = wordlist[1];
                        }
                    }
                }else if(buf.startsWith("left"))
                {
                    buflist = buf.split(' ');
                    for(QString word:buflist)
                    {
                        wordlist = word.split(':');
                        if(wordlist[0] == "pitch")
                        {
                            ui->left_pitch_bt->setText(wordlist[1]);
                        }else if(wordlist[0] == "yaw")
                        {
                            ui->left_yaw_bt->setText(wordlist[1]);
                        }else if(wordlist[0] == "speed")
                        {
                            ui->left_speed_bt->setText(wordlist[1]);
                        }
                    }
                    ui->message->append(buf);
                }else if(buf.startsWith("right"))
                {
                    buflist = buf.split(' ');
                    for(QString word:buflist)
                    {
                        wordlist = word.split(':');
                        if(wordlist[0] == "pitch")
                        {
                            ui->right_pitch_bt->setText(wordlist[1]);
                        }else if(wordlist[0] == "yaw")
                        {
                            ui->right_yaw_bt->setText(wordlist[1]);
                        }else if(wordlist[0] == "speed")
                        {
                            ui->right_speed_bt->setText(wordlist[1]);
                        }
                    }
                    ui->message->append(buf);
                }else if(buf.startsWith("speedmax"))
                {
                    buflist = buf.split(' ');
                    for(QString word:buflist)
                    {
                        wordlist = word.split(':');
                        if(wordlist[0] == "speedmax")
                        {
                            ui->speedmax->setText(wordlist[1]);
                        }else if(wordlist[0] == "speedmin")
                        {
                            ui->speedmin->setText(wordlist[1]);
                        }else if(wordlist[0] == "movespeed")
                        {
                            ui->movespeed->setText(wordlist[1]);
                        }else if(wordlist[0] == "moveradium")
                        {
                            ui->moveradium->setText(wordlist[1]);
                        }else if(wordlist[0] == "angleradium")
                        {
                            ui->angleradium->setText(wordlist[1]);
                        }else if(wordlist[0] == "anglespeed")
                        {
                            ui->anglespeed->setText(wordlist[1]);
                        }else if(wordlist[0] == "start")
                        {
                            ui->start_2->setText(wordlist[1]);
                        }else if(wordlist[0] == "factor")
                        {
                            ui->factor->setText(wordlist[1]);
                        }else if(wordlist[0] == "xfactor")
                        {
                            ui->xfactor->setText(wordlist[1]);
                        }
                    }
                    ui->message->append(buf);
                }else
                    ui->message->append(buf);
            }

        }
    }
    requestData.clear();
}

void Serial::on_bt_disconect_clicked()
{
    serialport->close();

    ui->bt_disconect->setDisabled(true);
    ui->sand->setDisabled(true);
    ui->bt_connect->setDisabled(false);
}

void Serial::on_sand_clicked()
{
    serialport->write((ui->et_send->text()+"\r\n").toLatin1());
}

void Serial::on_posPrint_clicked()
{
    serialport->write("pos print\r\n");
}

void Serial::on_toStart_clicked()
{
    serialport->write("auto start\r\n");
}

void Serial::on_toLoad_clicked()
{
    serialport->write("auto load\r\n");
}

void Serial::on_toPos_clicked()
{
    serialport->write("auto pos\r\n");
}

void Serial::on_posSelect_clicked()
{
    serialport->write(("pos select "+ui->et_select->text()+"\r\n").toLatin1());
}

void Serial::on_target_clicked()
{
    serialport->write(("launch target "+ui->et_target->text()+"\r\n").toLatin1());
}

void Serial::on_printLeft_clicked()
{
    serialport->write("launch print l\r\n");
}

void Serial::on_printRight_clicked()
{
     serialport->write("launch print r\r\n");
}

void Serial::on_pitchLeft_clicked()
{
    serialport->write(("launch set l pitch "+ui->left_pitch->text()+"\r\n").toLatin1());
}

void Serial::on_pitchRight_clicked()
{
    serialport->write(("launch set r pitch "+ui->right_pitch->text()+"\r\n").toLatin1());
}

void Serial::on_yawLeft_clicked()
{
    serialport->write(("launch set l yaw "+ui->left_yaw->text()+"\r\n").toLatin1());
}

void Serial::on_speedLeft_clicked()
{
    serialport->write(("launch set l speed "+ui->left_speed->text()+"\r\n").toLatin1());
}


void Serial::on_yawRight_clicked()
{
    serialport->write(("launch set r yaw "+ui->right_yaw->text()+"\r\n").toLatin1());
}

void Serial::on_speedRight_clicked()
{
    serialport->write(("launch set r speed "+ui->right_speed->text()+"\r\n").toLatin1());
}

void Serial::on_nextLeft_clicked()
{
    serialport->write("auto continute l\r\n");
}

void Serial::on_nextRight_clicked()
{
    serialport->write("auto continute r\r\n");
}

void Serial::on_left_add_clicked()
{
    serialport->write(("launch add l "+ui->left_no->text() + " " + ui->left_pitch->text() + " 0 " + ui->left_speed->text() + " " + ui->left_yaw->text() + " 0 " +"\r\n").toLatin1());
}

void Serial::on_left_modi_clicked()
{
    serialport->write(("launch modi l "+ui->left_no->text() + " " + ui->left_pitch->text() + " 0 " + ui->left_speed->text() + " " + ui->left_yaw->text() + " 0 " +"\r\n").toLatin1());
}

void Serial::on_right_add_clicked()
{
    serialport->write(("launch add r "+ui->right_no->text() + " " + ui->right_pitch->text() + " 0 " + ui->right_speed->text() + " " + ui->right_yaw->text() + " 0 " +"\r\n").toLatin1());
}

void Serial::on_right_modi_clicked()
{
    serialport->write(("launch modi r "+ui->right_no->text() + " " + ui->right_pitch->text() + " 0 " + ui->right_speed->text() + " " + ui->right_yaw->text() + " 0 " +"\r\n").toLatin1());
}

void Serial::on_start_clicked()
{
    serialport->write("stop 0\r\n");
}

void Serial::on_stop_clicked()
{
    serialport->write("stop\r\n");
}

void Serial::on_auto_print_clicked()
{
    serialport->write("auto print\r\n");
}

void Serial::on_left_target_clicked()
{
    serialport->write(("auto select l "+ui->target_l->text() +"\r\n").toLatin1());
}

void Serial::on_right_target_clicked()
{
    serialport->write(("auto select r "+ui->target_r->text() +"\r\n").toLatin1());
}

void Serial::on_home_clicked()
{
    serialport->write("launch set home\r\n");
}

void Serial::on_left_fly_clicked()
{
    serialport->write("launch fly l\r\n");
}

void Serial::on_right_fly_clicked()
{
    serialport->write("launch fly r\r\n");
}

void Serial::on_left_next_clicked()
{
    serialport->write("launch continute l\r\n");
}

void Serial::on_right_next_clicked()
{
    serialport->write("launch continute r\r\n");
}

void Serial::on_pos_save_clicked()
{
    serialport->write("pos save\r\n");
}

void Serial::on_pushButton_clicked()
{
    serialport->write("test home\r\n");
}

void Serial::on_switch_red_clicked()
{
    serialport->write("switch left\r\n");
}

void Serial::on_switch_blue_clicked()
{
    serialport->write("switch right\r\n");
}

void Serial::on_left_pitch_bt_clicked()
{
    ui->left_pitch->setText(ui->left_pitch_bt->text());
}

void Serial::on_left_yaw_bt_clicked()
{
    ui->left_yaw->setText(ui->left_yaw_bt->text());
}

void Serial::on_left_speed_bt_clicked()
{
    ui->left_speed->setText(ui->left_speed_bt->text());
}

void Serial::on_right_pitch_bt_clicked()
{
    ui->right_pitch->setText(ui->right_pitch_bt->text());
}

void Serial::on_right_yaw_bt_clicked()
{
    ui->right_yaw->setText(ui->right_yaw_bt->text());
}

void Serial::on_right_speed_bt_clicked()
{
    ui->right_speed->setText(ui->right_speed_bt->text());
}

void Serial::on_debug_clicked()
{
    serialport->write("test debug\r\n");
}

void Serial::on_debug_print_clicked()
{
    serialport->write("test debugp\r\n");
}


void Serial::on_chassis_read_clicked()
{
    serialport->write("param print\r\n");
}

void Serial::on_chassis_save_clicked()
{
    serialport->write("param save\r\n");
}

void Serial::on_speedmax_bt_clicked()
{
    serialport->write(("param speedmax "+ui->speedmax->text()+"\r\n").toLatin1());
}

void Serial::on_movespeed_bt_clicked()
{
    serialport->write(("param movespeed "+ui->movespeed->text()+"\r\n").toLatin1());
}

void Serial::on_anglespeed_bt_clicked()
{
    serialport->write(("param anglespeed "+ui->anglespeed->text()+"\r\n").toLatin1());
}

void Serial::on_moveradium_bt_clicked()
{
    serialport->write(("param moveradium "+ui->moveradium->text()+"\r\n").toLatin1());
}

void Serial::on_angleradium_bt_clicked()
{
    serialport->write(("param angleradium "+ui->angleradium->text()+"\r\n").toLatin1());
}

void Serial::on_start_bt_clicked()
{
    serialport->write(("param start "+ui->start_2->text()+"\r\n").toLatin1());
}

void Serial::on_speedmin_bt_clicked()
{
    serialport->write(("param speedmin "+ui->speedmin->text()+"\r\n").toLatin1());
}

void Serial::on_xfactor_bt_clicked()
{
    serialport->write(("param xfactor "+ui->xfactor->text()+"\r\n").toLatin1());
}

void Serial::on_factor_bt_clicked()
{
    serialport->write(("param factor "+ui->factor->text()+"\r\n").toLatin1());
}

void Serial::on_go_clicked()
{
    serialport->write(("action "+ui->pos_x->text()+" "+ui->pos_y->text()+" "+ui->pos_ang->text()+"\r\n").toLatin1());
}

void Serial::on_modi_pos_clicked()
{
    serialport->write(("pos modi 1 "+x+" "+y+" "+ang+"\r\n").toLatin1());
}

void Serial::on_load_clicked()
{
    serialport->write(("auto loadarea "+x+" "+y+" "+ang+"\r\n").toLatin1());
}

void Serial::on_auto_save_clicked()
{
    serialport->write("auto save\r\n");
}
