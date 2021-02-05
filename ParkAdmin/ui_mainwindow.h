/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCommandLinkButton *caradmin;
    QCommandLinkButton *lookout;
    QLabel *label_2;
    QCommandLinkButton *video;
    QTableWidget *tableWidget;
    QLabel *update;
    QTextBrowser *textBrowser;
    QSlider *horizontalSlider;
    QListWidget *listWidget;
    QLabel *vd_po;
    QLabel *vd_len;
    QCommandLinkButton *commandLinkButton;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 480);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(50, 54, 63);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        caradmin = new QCommandLinkButton(centralwidget);
        caradmin->setObjectName(QStringLiteral("caradmin"));
        caradmin->setGeometry(QRect(530, 30, 101, 41));
        caradmin->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/pic/admin.png"), QSize(), QIcon::Normal, QIcon::Off);
        caradmin->setIcon(icon);
        caradmin->setCheckable(false);
        caradmin->setChecked(false);
        lookout = new QCommandLinkButton(centralwidget);
        lookout->setObjectName(QStringLiteral("lookout"));
        lookout->setGeometry(QRect(410, 30, 101, 41));
        lookout->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/pic/jiankong.png"), QSize(), QIcon::Normal, QIcon::Off);
        lookout->setIcon(icon1);
        lookout->setCheckable(true);
        lookout->setChecked(false);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(80, 10, 231, 51));
        QFont font;
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label_2->setFont(font);
        label_2->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        video = new QCommandLinkButton(centralwidget);
        video->setObjectName(QStringLiteral("video"));
        video->setGeometry(QRect(650, 30, 101, 41));
        video->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/pic/video.png"), QSize(), QIcon::Normal, QIcon::Off);
        video->setIcon(icon2);
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 90, 801, 391));
        tableWidget->setStyleSheet(QLatin1String("color: rgb(243, 243, 243);\n"
"\n"
"QTableWidget{\n"
"color:#DCDCDC;\n"
"background:#444444;\n"
"border:1px solid #242424;\n"
"alternate-background-color:#525252;\n"
"gridline-color:#242424;\n"
"}\n"
"\n"
"QTableWidget::item:selected{\n"
"color:#DCDCDC;\n"
"background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 #484848,stop:1 #383838);\n"
"}\n"
"\n"
"\n"
"QTableWidget::item:hover{\n"
"background:#5B5B5B;\n"
"}\n"
"\n"
"QHeaderView::section{\n"
"text-align:center;\n"
"background:#5E5E5E;\n"
"padding:3px;\n"
"margin:0px;\n"
"color:#DCDCDC;\n"
"border:1px solid #242424;\n"
"border-left-width:0;\n"
"}\n"
"\n"
"QScrollBar:vertical{\n"
"background:#484848;\n"
"padding:0px;\n"
"border-radius:6px;\n"
"max-width:12px;\n"
"}\n"
"\n"
"\n"
"QScrollBar::handle:vertical{\n"
"background:#CCCCCC;\n"
"}\n"
"\n"
"QScrollBar::handle:hover:vertical,QScrollBar::handle:pressed:vertical{\n"
"background:#A7A7A7;\n"
"}\n"
"\n"
"QScrollBar::sub-page:vertical{\n"
"background:444444;\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical"
                        "{\n"
"background:5B5B5B;\n"
"}\n"
"QScrollBar::add-line:vertical{\n"
"background:none;\n"
"}\n"
"QScrollBar::sub-line:vertical{\n"
"background:none;\n"
"}\n"
""));
        update = new QLabel(centralwidget);
        update->setObjectName(QStringLiteral("update"));
        update->setGeometry(QRect(0, 90, 611, 391));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(610, 90, 191, 391));
        QFont font1;
        font1.setPointSize(10);
        textBrowser->setFont(font1);
        textBrowser->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        horizontalSlider = new QSlider(centralwidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(0, 430, 611, 20));
        horizontalSlider->setStyleSheet(QLatin1String("QWidget#centralWidget{\n"
"       border-style:solid;\n"
"	   background: transparent;\n"
"	   background-color:#0066CC;\n"
"	   \n"
"}\n"
" \n"
"QSlider#horizontalSlider::groove:horizontal {  \n"
"border: 1px solid #4A708B;  \n"
"background: #575757;\n"
"height: 5px;  \n"
"border-radius: 1px;  \n"
"padding-left:-1px;  \n"
"padding-right:-1px;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::sub-page:horizontal {  \n"
"background: qlineargradient(x1:0, y1:0, x2:0, y2:1,   \n"
"    stop:0 #B1B1B1, stop:1 #c4c4c4);  \n"
"background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \n"
"    stop: 0 #5DCCFF, stop: 1 #1874CD);  \n"
"border: 1px solid #4A708B;  \n"
"height: 10px;  \n"
"border-radius: 2px;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::add-page:horizontal {  \n"
"background: #575757;  \n"
"border: 0px solid #777;  \n"
"height: 10px;  \n"
"border-radius: 2px;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::handle:horizontal   \n"
"{  \n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0"
                        ".5, fx:0.5, fy:0.5,   \n"
"    stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  \n"
"  \n"
"    width: 11px;  \n"
"    margin-top: -3px;  \n"
"    margin-bottom: -3px;  \n"
"    border-radius: 5px;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::handle:horizontal:hover {  \n"
"    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,   \n"
"    stop:0.778409 rgba(255, 255, 255, 255));  \n"
"  \n"
"    width: 11px;  \n"
"    margin-top: -3px;  \n"
"    margin-bottom: -3px;  \n"
"    border-radius: 5px;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::sub-page:horizontal:disabled {  \n"
"background: #00009C;  \n"
"border-color: #999;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::add-page:horizontal:disabled {  \n"
"background: #eee;  \n"
"border-color: #999;  \n"
"}  \n"
"  \n"
"QSlider#horizontalSlider::handle:horizontal:disabled {  \n"
"background: #eee;  \n"
"border: 1px solid #aaa;  \n"
"border-radius: 4px;  \n"
"}  "));
        horizontalSlider->setOrientation(Qt::Horizontal);
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(610, 90, 191, 391));
        listWidget->setStyleSheet(QLatin1String("background-color:transparent;\n"
"color: rgb(243, 243, 243);"));
        vd_po = new QLabel(centralwidget);
        vd_po->setObjectName(QStringLiteral("vd_po"));
        vd_po->setGeometry(QRect(10, 450, 131, 21));
        vd_po->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        vd_len = new QLabel(centralwidget);
        vd_len->setObjectName(QStringLiteral("vd_len"));
        vd_len->setGeometry(QRect(480, 450, 131, 21));
        vd_len->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        vd_len->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        commandLinkButton = new QCommandLinkButton(centralwidget);
        commandLinkButton->setObjectName(QStringLiteral("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(0, 0, 71, 81));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/prefix1/pic/parking.png"), QSize(), QIcon::Normal, QIcon::Off);
        commandLinkButton->setIcon(icon3);
        commandLinkButton->setIconSize(QSize(60, 60));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 50, 261, 31));
        QFont font2;
        font2.setPointSize(9);
        label->setFont(font2);
        label->setStyleSheet(QStringLiteral("color: rgb(243, 243, 243);"));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        caradmin->setText(QApplication::translate("MainWindow", "\350\275\246\350\276\206\347\256\241\347\220\206", 0));
        lookout->setText(QApplication::translate("MainWindow", "\345\256\236\346\227\266\347\233\221\346\216\247", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\201\234\350\275\246\345\234\272\347\256\241\347\220\206\347\263\273\347\273\237", 0));
        video->setText(QApplication::translate("MainWindow", "\350\247\206\351\242\221\345\233\236\346\224\276", 0));
        update->setText(QString());
        vd_po->setText(QString());
        vd_len->setText(QString());
        commandLinkButton->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Parking Management System", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
