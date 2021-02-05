#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aip-cpp-sdk-0.7.10/ocr.h"
//初始化 RFID 模块的串口
int init_tty(int fd)
{
    struct termios old_flags,new_flags;
    bzero(&new_flags,sizeof(new_flags));

    //1. 获取旧的属性
    tcgetattr(fd,&old_flags);

    //2. 设置原始模式
    cfmakeraw(&new_flags);

    //3. 激活本地连接CLOCAL与接收使能CREAD的选项
    new_flags.c_cflag |= CLOCAL | CREAD;

    //4. 设置波特率
    cfsetispeed(&new_flags, B9600);
    cfsetospeed(&new_flags, B9600);

    //5. 设置数据位为8位
    new_flags.c_cflag &= ~CSIZE; //清空原有的数据位
    new_flags.c_cflag |= CS8;

    //6. 设置奇偶检验位
    new_flags.c_cflag &= ~PARENB;

    //7. 设置一位停止位
    new_flags.c_cflag &= ~CSTOPB;

    //8. 设置等待时间，最少接收字符个数
    new_flags.c_cc[VTIME] = 0;
    new_flags.c_cc[VMIN] = 1;

    //9. 清空串口缓冲区
    tcflush(fd, TCIFLUSH);

    //10. 设置串口的属性到文件中
    tcsetattr(fd, TCSANOW, &new_flags);

    return 0;
}

//校验和的算法
//get_bcc(wbuf,wbuf[0]-2);
char get_bcc(char *buf,int n)
{
    char bcc = 0;
    int i;
    for(i=0;i<n;i++)
    {
        bcc ^= buf[i];
    }

    return (~bcc);
}

//获取卡号线程任务函数
void *rfid(void *arg)
{
    MainWindow  *pid = (MainWindow  *)arg;
    //1. 访问串口1
    int fd = open("/dev/ttySAC1",O_RDWR | O_NOCTTY);

    //2. 配置串口1参数
    init_tty(fd);

    while(1)
    {
        while(1)
        {
            //3.制定请求协议  A （请求命令）
            char wbuf[7];

            //请求数据格式
            bzero(wbuf,7);
            wbuf[0] = 0x07; //帧长=7byte
            wbuf[1] = 0x02; //命令类型
            wbuf[2] = 'A';  //命令'A' 代表请求
            //wbuf[2] = 0x41;
            wbuf[3] = 0x01; //信息长度
            wbuf[4] = 0x52; //信息  ALL=0x52
            wbuf[5] = get_bcc(wbuf,wbuf[0]-2); //计算校验和
            wbuf[6] = 0x03; //结束标志

            //发送A 命令
            tcflush(fd,TCIFLUSH);
            write(fd,wbuf,sizeof(wbuf));

            //获取从机应答数据
            char  rbuf[8]={0};
            int size=read(fd,rbuf,sizeof(rbuf));
            if(size > 0)
            {
                //判断从机的状态码
                if(rbuf[2] == 0x00)
                {
                    printf("请求成功\n");

                    //进行防碰撞操作获取卡号
                    char buf_b[8]={0};
                    buf_b[0] = 0x08;
                    buf_b[1] = 0x02;
                    buf_b[2] = 0x42;
                    buf_b[3] = 0x02;
                    buf_b[4] = 0x93;
                    buf_b[5] = 0x00;
                    buf_b[6] = get_bcc(buf_b,buf_b[0]-2);;
                    buf_b[7] = 0x03;

                    //发送防止碰撞协议
                    tcflush(fd,TCIFLUSH);
                    write(fd,buf_b,sizeof(buf_b));

                    char r_buf_b[10]={0};
                    read(fd,r_buf_b,10);

                    if(r_buf_b[2] == 0x00)
                    {
                        printf("防止碰撞请求成功\n");  //把该卡的卡号给取出来

                        unsigned int car_id=0;
                        char *p = (char *)&car_id;

                        *p = r_buf_b[4];
                        *(p+1) = r_buf_b[5];
                        *(p+2) = r_buf_b[6];
                        *(p+3) = r_buf_b[7];
                        printf("car_id=%u\n",car_id);
                        emit pid->send_id(car_id);

                        sleep(3);   //防止重复读卡
                    }
                }
            }
         }
     }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->update->hide();
    ui->tableWidget->hide();
    ui->textBrowser->hide();
    ui->tableWidget->hide();
    ui->horizontalSlider->hide();
    //摄像头定时器
   timer = new QTimer(this);
   timer2= new QTimer(this);
   connect(timer2, SIGNAL(timeout()), this, SLOT(test()));
   connect(timer, SIGNAL(timeout()), this, SLOT(video_update()));       //摄像头数据更新

    //初始化摄像头
    linux_v4l2_yuyv_init("/dev/video7");

   //开启捕捉功能
    linux_v4l2_start_yuyv_capturing();
    timer->start(5);

    connect(this,SIGNAL(send_id(unsigned int)),this,SLOT(data_insert(unsigned int)));   //刷卡感应槽函数

    db = QSqlDatabase::addDatabase("QSQLITE");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //设置列宽自适应


    //打开数据库
    db.setDatabaseName("./parkcar.db");
    if(db.open())
    {
        qDebug()<<"登录数据库成功";
    }
    else
    {
        qDebug()<<"登录数据库失败";
    }

    //创建表格
    QSqlQuery query;
    QString cmd = "create  table  parkcar(车牌 char unique,卡号 int unique,卡类 char,进场时间 char,车辆照片名 char);";
    query.exec(cmd);

    show_table();

    //开启识别线程
    pthread_t tid;
    pthread_create(&tid,NULL,rfid,(void *)this);

    //设置线程为分离属性
    pthread_detach(tid);
    //sleep(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_table()//数据库表格刷新
{
    //把字段名显示在tablewidget中
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->takeHorizontalHeaderItem(0);
    headerLabels.clear();

    QSqlQuery query1;
    QSqlQuery query2;

    QString sql = "select * from parkcar;";     //表格内容
    QString pra = "pragma table_info(parkcar);";   //表格字段名

    query1.exec(sql);       //执行命令
    query1.last();
    query2.exec(pra);
    query2.last();

    int nRow,nColumn;
    int i = 0, j = 0;
    nRow = query1.at() + 1;
    ui->tableWidget->setRowCount(nRow);        //设置表格的行数
    nColumn = query2.at()+1;
    ui->tableWidget->setColumnCount(nColumn);  //设置表格的列数
    query1.first();
    query2.first();

    while(j<nColumn)
    {
        headerLabels<< query2.value(1).toString();
        j++;
        query2.next();             //下一个

    }
     //qDebug()<<headerLabels;
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels); //设置表头名
    j=0;

   //将数据显示到TableWidget中
   while(j<nRow)
    {
        for (i = 0; i<nColumn; i++)
        {
            QTableWidgetItem *item2 = new QTableWidgetItem(query1.value(i).toString());
            //printf("%s\n",query1.value(i).toString().toUtf8().data());
            ui->tableWidget->setItem(j, i,item2);
        }
        j++;
        query1.next();
   }
}

void MainWindow::data_insert(unsigned int car_id)    //更新数据，入库出库，拍照，识别车牌，语音播报         car_id 为卡号
{
    //1.创建 TCP  通信协议
    int tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //2.设置链接的服务器地址信息
    struct sockaddr_in  addr1;
    addr1.sin_family   = AF_INET; //IPV4 协议
    addr1.sin_port     = htons(8888); //端口
    addr1.sin_addr.s_addr = inet_addr("192.168.99.89"); //服务器的IP 地址

    //3.链接服务器,发送数据
    ::connect(tcp_sockfd,(struct sockaddr *)&addr1,sizeof(addr1));

    //根据卡号判断卡的类别
    if(car_id>2000000000)
    {
        card_type = "包月卡";
        //card_time  = card_day;
    }
    else
    {
        card_type = "临时卡";
        //card_time = card_hour;
    }

    if(ui->tableWidget->item(0,0)!=NULL)    //如果不为空
    {
        QStringList  tmp;
        //把卡号数据全部获取下来
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            tmp.append(ui->tableWidget->item(i,ui->tableWidget->columnCount()-4)->text());
        }
        qDebug() << "id num:"<<tmp;
        //把时间数据全部获取下来
        QStringList  time_tmp;
        for(int i=0;i<ui->tableWidget->rowCount();i++)
         {
             time_tmp.append(ui->tableWidget->item(i,ui->tableWidget->columnCount()-2)->text());
         }
         //qDebug() <<"time_tmp:"<< time_tmp;
        for(int i=0;i<tmp.length();i++) //遍历卡号与新卡号比较，有则出库，无则入库
        {
            int index = tmp.indexOf(QString::number(car_id));
            if(index+1)  //车辆出库
            {
               //1、根据卡号删除数据库车辆信息
               QSqlQuery query;
               query.exec(QString("delete from parkcar where %1 = '%2';").arg(headerLabels[1]).arg(car_id));
               ui->tableWidget->removeRow(index);   //删除tablewidget项
               tmp.takeAt(index);                   //删除list中的卡号内容

               //2、显示入库时的照片
               //把数据显示到qlabel中
               on_lookout_clicked();
               timer->stop();
               ui->update->clear();
               QString car_pic = QString("./v4l2jpg/%1.jpg").arg(car_id);
               QPixmap pic;
               pic.load(car_pic);
               ui->update->setPixmap(pic);
               //显示照片3s恢复视频流
               timer2->start(3000);

               //识别车牌号
               plate = get_plate(car_pic);

               //计算停车费用
               QString timein = time_tmp[index];
               QDateTime in = QDateTime::fromString(timein,"yyyy-MM-dd hh:mm:ss");
               QDateTime out = QDateTime::currentDateTime();
               int time_dis = in.secsTo(out);    //计算秒差
               qDebug() << "time_money"<<time_dis;
               cost = QString::number(time_dis);    //消费

               //3、根据卡类出库
               if(card_type == "包月卡")
               {
                   card_day = QString::number(card_day.toUInt()-time_dis/10);
                   qDebug()<<"CARD_DAY:"<< card_day;
                   //发送语音播报数据
                   QString cost_alsa = QString(plate+"您的消费金额为"+cost+"元，剩余"+card_day+"天，一路顺风");
                   write(tcp_sockfd,cost_alsa.toUtf8().data(),strlen(cost_alsa.toUtf8().data()));

               }

               else if(card_type == "临时卡")
               {
                   card_hour = QString::number(card_hour.toUInt()-time_dis/10);
                   qDebug()<<"CARD_HOUR:"<< card_hour;
                   //发送语音播报数据
                   QString cost_alsa = QString(plate+"您的消费金额为"+cost+"元，剩余"+card_hour+"小时，一路顺风");
                   write(tcp_sockfd,cost_alsa.toUtf8().data(),strlen(cost_alsa.toUtf8().data()));

               }
               //4、显示出场信息
               ui->textBrowser->clear();
               ui->textBrowser->append("车牌号："+plate);
               ui->textBrowser->append("卡号："+QString::number(car_id));
               ui->textBrowser->append("消费："+cost);

               show_table();
            }
            else
            {
                //1、刷卡拍照
                timer->stop();
                //退出摄像头
                linux_v4l2_yuyv_quit();
                //拍照保持到./v4l2jpg/中
                QString pic = QString("ffmpeg -y  -f v4l2 -s 640X480  -r 1  -t 1  -i /dev/video7 -f image2  ./v4l2jpg/%1.jpg").arg(car_id);
                system(pic.toUtf8().data());

                //2、识别车牌号
                QString car_pic=QString("./v4l2jpg/%1.jpg").arg(car_id);
                plate = get_plate(car_pic);
                //printf("车牌号：%s\n",plate.toUtf8().data());

                //3、往表格中添加一行
                int rowcount=ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(rowcount +1);
                //(车牌 char primary key,卡号 int unique,卡类 char,进场时间 char);
                QString cmd = QString("insert  into  parkcar values('%1','%2','%3',datetime('now','localtime'),'%2.jpg');").arg(plate).arg(car_id).arg(card_type);

                QSqlQuery query;
                query.exec(cmd);
                show_table();

                //4、发送语音播报数据
                QString carid_alsa = QString(plate+"您的卡号为"+QString::number(car_id)+card_type);
                write(tcp_sockfd,carid_alsa.toUtf8().data(),strlen(carid_alsa.toUtf8().data()));
                sleep(3);
                //初始化摄像头
                linux_v4l2_yuyv_init("/dev/video7");

               //开启捕捉功能
                linux_v4l2_start_yuyv_capturing();
                timer->start(5);
            }
        }
    }
    else            //如果表格为空
    {
        //1、刷卡拍照
        timer->stop();
       //开启捕捉功能
        //退出摄像头
        linux_v4l2_yuyv_quit();
        QString pic = QString("ffmpeg  -y -f v4l2 -s 640X480  -r 1  -t 1  -i /dev/video7    -f image2  ./v4l2jpg/%1.jpg").arg(car_id);
        system(pic.toUtf8().data());

        //2、识别车牌号
        QString car_pic=QString("./v4l2jpg/%1.jpg").arg(car_id);
        plate = get_plate(car_pic);
        //printf("车牌号：%s\n",plate.toUtf8().data());

        //3、往表格中添加一行
        int rowcount=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowcount +1);
        QString cmd = QString("insert  into  parkcar values('%1','%2','%3',datetime('now','localtime'),'%2.jpg');").arg(plate).arg(car_id).arg(card_type);

        QSqlQuery query;
        query.exec(cmd);
        show_table();

        //4、发送语音播报数据
        QString carid_alsa = QString(plate+"您的卡号为"+QString::number(car_id)+card_type);
        write(tcp_sockfd,carid_alsa.toUtf8().data(),strlen(carid_alsa.toUtf8().data()));

        sleep(3);
        //初始化摄像头
        linux_v4l2_yuyv_init("/dev/video7");

       //开启捕捉功能
        linux_v4l2_start_yuyv_capturing();
        timer->start(5);
    }
    //show_table();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)//获取当前项信息
{
    if(ui->tableWidget->currentItem()==NULL)    //如果为空退出
            return;
     mod_content=ui->tableWidget->currentItem()->text();
     g_row=row;
     g_column=column;
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)//修改数据
{
    ui->tableWidget->blockSignals(true);        //进入阻塞模式，阻塞信号
    //printf("表格内容:%s\n",item->text().toUtf8().data());
    //printf("字段名:%s\n",headerLabels[g_column].toUtf8().data());

    // update   表名  set  age="19"  where age="18";
    QSqlQuery query;
    query.exec(QString("update   parkcar  set  %1 = '%2'  where %1= '%3';").arg(headerLabels[g_column]).arg(item->text()).arg(mod_content));

    show_table();
    ui->tableWidget->blockSignals(false);      //退出阻塞模式
}

void MainWindow::on_caradmin_clicked()      //1、车辆管理
{
    system("killall -9 mplayer");
    ui->update->hide();
    ui->horizontalSlider->hide();
    ui->listWidget->hide();
    ui->textBrowser->hide();
    ui->vd_len->hide();
    ui->vd_po->hide();
    ui->tableWidget->show();
}

QString MainWindow::get_plate(QString &plateJpg)    //车牌识别
{
    std::string app_id = "23603411";
    std::string api_key = "LpKUz4r23t6AnmGVbUZIbQT3";
    std::string secret_key = "y8TDDLkkyWWvyglV4BUPI5yRnzkU7CEe";
    aip::Ocr client(app_id, api_key, secret_key);

    Json::Value result;

    //选择拍照的文件进行识别
    std::string image;
    aip::get_file_content(plateJpg.toUtf8().data(), &image);

    // 调用通用文字识别（高精度版）
    result = client.accurate_basic(image, aip::null);

    //把 JSON 数据转换为字符串数据进行打印
    QString msg = result.toStyledString().data();
    //获取车牌号
    QString getPlate = (QJsonDocument::fromJson(msg.toUtf8())).object().take("words_result").toArray().at(0).toObject().take("words").toString();
    printf("识别出的车牌号：%s\n",getPlate.toUtf8().data());

    return getPlate;
}

void MainWindow::on_lookout_clicked()   //2、实时监控
{
    system("killall -9 mplayer");
    ui->tableWidget->hide();
    ui->horizontalSlider->hide();
    ui->listWidget->hide();
    ui->vd_len->hide();
    ui->vd_po->hide();
    ui->textBrowser->show();
    ui->update->show();
}

void MainWindow::video_update()         //获取摄像头数据
{
    //获取一针摄像头的数据
     struct jpg_data jpg_buf;
     linux_v4l2_get_yuyv_data(&jpg_buf);

     //把数据显示到qlabel中
     QPixmap pic;
     pic.loadFromData(jpg_buf.jpg_data,jpg_buf.jpg_size);  //加载本地像素数据
     ui->update->setPixmap(pic);

     //保持到本地目录中,并合成视频
     QString jpgName;
     if(num==1500)   //开始合成视频
     {
        compose_video();
        num=0;
     }
     if(num<1500)    //一个目录最多1500张图片
     {
         jpgflag=true;
         jpgName=QString("./vdpic1/%1.jpg").arg(num++);
     }
/*     else
//     {
//         jpgflag=false;
//         jpgName=QString("./vdpic2/%1.jpg").arg(num++);
     }
*/
     //创建文件
     jpgPath.setFileName(jpgName);
     //打开文件，写入像素数据,关闭文件
     jpgPath.open(QIODevice::ReadWrite|QIODevice::Truncate);
     jpgPath.write((const char *)jpg_buf.jpg_data,jpg_buf.jpg_size);
     jpgPath.close();

}

void MainWindow::compose_video()        //图片合成视频
{
    videoComps = new QProcess(this);
    //videoComps->setProcessChannelMode(QProcess::MergedChannels);
    connect(videoComps,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(compose_ok())); //合成完成发送信号
    if(jpgflag)
    {
        //ffmpeg -y -r 10 -pattern_type glob -i ./vdpic1/*.jpg -vf fps=25 -pix_fmt yuv420p %1.avi
        QString cmd = QString("ffmpeg  -y -f image2 -i ./vdpic1/%d.jpg  ./%1.avi").arg(i++);
        videoComps->start(cmd);
        videoComps->write("y\n");
    }
/*    else
    {
        QString cmd = QString("ffmpeg -y -f image2 -i ./vdpic2/%d.jpg  ./vdpic2/%1.avi").arg("video2");
        videoComps->start(cmd);
        videoComps->write("y\n");
    }*/
}

void MainWindow::compose_ok()   //视频合成完成
{
    printf("合成视频完成！\n");
}

void MainWindow::test()
{
    timer->start(5);
}

void MainWindow::on_video_clicked() //3、视频回放
{
    ui->update->hide();
    ui->textBrowser->hide();
    ui->tableWidget->hide();
    ui->horizontalSlider->show();
    ui->listWidget->show();
    ui->vd_len->show();
    ui->vd_po->show();
    ui->vd_len->setText("00:00:00");
    ui->vd_po->setText("00:00:00");

    //创建进程
    videoPlay = new QProcess;
    videoPlay->setProcessChannelMode(QProcess::MergedChannels);

    //mplayer路径
    mp_path="/bin/mplayer";
    //创建定时器
    timer_play = new QTimer(this);
    connect(timer_play,&QTimer::timeout,this,&MainWindow::onTimeout);
    //设置步长
    ui->horizontalSlider->setSingleStep(10);
    //设置关闭listwidget的滚动条
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //添加视频文件到列表中
    QDir videoDir("/JR");
    QStringList path;
    QStringList pf;
    QStringList filters;
    QStringList aviFile;

    path<<"/JR/";
    filters << "*.avi";             //视频文件
    pf= videoDir.entryList(filters);     //添加列表

    for(int i=0;i<pf.size();i++)
    {
        aviFile<<path.at(0)+pf.at(i);  //拼接路径和文件名
        qDebug() <<"aviFile"<<aviFile;
    }
    ui->listWidget->clear();
    ui->listWidget->addItems(aviFile);  //添加视频文件到列表中


}

void MainWindow::video_play(QString vd_path)    //视频播放
{
    videoPlay->kill();
    videoPlay=new QProcess;

    //播放视频命令
    QStringList cmd;
    //"-loop"循环播放 |"-geometry"视频窗口开始位置| "-zoom" 视频窗口大小
    cmd <<vd_path<< "-slave"<<"-quiet" <<"-geometry" << "0:90"<<"-zoom" << "-x"<<"611" <<"-y"<<"340"<<"-loop"<<"0";

    videoPlay->start(mp_path,cmd);          //开启进程
    connect(videoPlay,SIGNAL(readyReadStandardOutput()),this,SLOT(video_data()));//关联视频数据处理槽函数
    timer_play->start(1000);                 //设置定时1s
}

void MainWindow::video_data()       //视频信息
{
    videoPlay->write("get_time_length\n");  //视频时长
    videoPlay->write("get_time_pos\n");     //视频当前播放位置
    videoPlay->write("get_percent_pos\n");  //视频当前播放位置百分比

    while(videoPlay->canReadLine())
    {
        //获取数据
        QString  message(videoPlay->readLine());    //读取数据
        QStringList mess_list = message.split("=");     //跳过等号
        if(mess_list[0] == "ANS_LENGTH")                //视频时长
        {
            vd_length = mess_list[1].toDouble();        //获取时长
            ui->horizontalSlider->setRange(0,vd_length);    //设置进度条范围
            QTime n(0,0,0);
            QTime time = n.addSecs(vd_length);
            qDebug()<<time.toString("hh:mm:ss");
            ui->vd_len->setText(time.toString("hh:mm:ss")); //将时长转换为时间戳格式
        }
        if(mess_list[0] == "ANS_TIME_POSITION")//当前时间位置
        {
            ntime = mess_list[1].toDouble();        //获取当前播放时间
            ui->horizontalSlider->setValue(ntime);  //设置进度条当前位置
            QTime n(0,0,0);
            QTime time = n.addSecs(ntime);
            qDebug()<<time.toString("hh:mm:ss");
            ui->vd_po->setText(time.toString("hh:mm:ss"));//将时长转换为时间戳格式
        }
    }
}

void MainWindow::onTimeout()        //视频播放定时器
{
    videoPlay->write("get_time_pos\n");
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) //双击列表播放
{
    video_play(item->text());
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)//进度条快进退
{
    if(action==1||action==3)    //单击右边快进
    {
        QString slide="seek +10 0\n";
        videoPlay->write(slide.toUtf8());
    }
    if(action==2||action==4)        //单击右边快退
    {
        QString slide="seek -10 0\n";
        videoPlay->write(slide.toUtf8());
    }
}

void MainWindow::on_horizontalSlider_sliderPressed()//进度条按下
{
    videoPlay->write("pause\n");
    disconnect(videoPlay,SIGNAL(readyReadStandardOutput()),this,SLOT(video_data()));
}

void MainWindow::on_horizontalSlider_sliderReleased()//进度条弹起
{
    QString slide="seek "+QString::number(ntime)+" 2\n";
    videoPlay->write(slide.toUtf8());
    videoPlay->write("pause\n");
    connect(videoPlay,SIGNAL(readyReadStandardOutput()),this,SLOT(video_data()));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)//进度条滑动
{
    ntime=position;
}
