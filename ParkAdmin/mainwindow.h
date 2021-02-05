#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QInputDialog>
#include <QDateTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSlider>
#include <QProcess>
#include <QDir>


extern "C"
{
    #include <termios.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    #include <stdlib.h>
    #include <pthread.h>

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h> /* superset of previous */
    #include <arpa/inet.h>
    #include "yuyv.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void show_table();                      //数据库表格刷新
    QString get_plate(QString &plateJpg);   //车牌识别
    void compose_video();                   //视频合成
    void video_play(QString vd_path);       //视频回放


signals:
    void send_id(unsigned int);         //发送卡号信号

private slots:
    void data_insert(unsigned int);         //各个功能
    void on_tableWidget_cellClicked(int row, int column);
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_caradmin_clicked();             //车辆管理

    void on_lookout_clicked();              //实时监控
    void video_update();                    //获取摄像头数据
    void compose_ok();                      //视频合成完毕
    void test();

    void on_video_clicked();
    void onTimeout();       //播放器定时器
    void video_data();      //视频数据处理
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);    //双击列表播放视频
    void on_horizontalSlider_actionTriggered(int action);
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QStringList headerLabels;//表头名
    QString mod_content;    //当前项内容
    int g_row;              //当前行数
    int g_column=0;         //当前列数
    QTimer *timer;          //摄像头定时器
    QTimer *timer2;         //视频录制定时器
    QString cost;           //停车消费金额
    QString plate;          //车牌号
    QString card_type;      //卡类

    //卡剩余时间
    QString card_day="30";
    QString card_hour="30";

    QFile jpgPath;
    int num=0;              //目录图片数量
    bool jpgflag=false;
    int i=1;                //视频数量
    QProcess *videoComps;    //合成进程
    QProcess *videoPlay;     //播放进程

    QString mp_path;
    QTimer *timer_play;
    int vd_length=0;            //视频总时长
    int ntime=0;                //进度条的值
};
#endif // MAINWINDOW_H
