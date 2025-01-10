QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color_blocks.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow2.cpp \
    thread.cpp

HEADERS += \
    QZXing.h \
    QZXing_global.h \
    color_blocks.h \
    mainwindow.h \
    mainwindow2.h \
    thread.h

FORMS += \
    color_blocks.ui \
    mainwindow.ui \
    mainwindow2.ui



INCLUDEPATH += D:\opencv\opencv\opencvs\install\include
LIBS += D:\opencv\opencv\opencvs\lib\libopencv_*.a

LIBS += -L$$PWD/QZXlib         -lQZXing3   #添加库
DEFINES += ENABLE_ENCODER_GENERIC   #开启二维码生成功能

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    QZXlib/QZXing3.dll \
    QZXlib/libQZXing3.a

RESOURCES += \
    img/img.qrc
