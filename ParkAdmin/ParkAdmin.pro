QT       += core gui    sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  /home/gec/libcurl/include/  /home/gec/libjson/include/  /home/gec/libopenssl/include/

LIBS +=  -L/home/gec/libcurl/lib   -L/home/gec/libjson   -L/home/gec/libopenssl/lib  -lcurl -lcrypto -ljsoncpp -lssl

LIBS += -L./   -lapi_v4l2_arm1  -ljpeg

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    aip-cpp-sdk-0.7.10/ocr.h \
    mainwindow.h \
    yuyv.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
