QT       += core gui multimedia multimediawidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    scaledview.cpp

HEADERS += \
    mainwindow.h \
    scaledview.h

FORMS += \
    mainwindow.ui

###For x86 Desktop###
#LIBS += `pkg-config --libs opencv4`
#QMAKE_CXXFLAGS += `pkg-config --cflags opencv4`

###For armhf###
LIBS += `pkg-config --libs /home/inventron/repo/sdk/imx8/sysroots/aarch64-poky-linux/usr/lib/pkgconfig/opencv4.pc`
QMAKE_CXXFLAGS += `pkg-config --cflags /home/inventron/repo/sdk/imx8/sysroots/aarch64-poky-linux/usr/lib/pkgconfig/opencv4.pc`
#QMAKE_LIBDIR += /home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/lib/
#QMAKE_LIBDIR += /home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/lib/arm-linux-gnueabihf
#QMAKE_LIBDIR += /home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/usr/lib/arm-linux-gnueabihf/
#QMAKE_LIBDIR += /home/serkan/Documents/imx8/sysroots/aarch64-poky-linux/usr/lib/

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0
PKGCONFIG += gstreamer-base-1.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/root/
!isEmpty(target.path): INSTALLS += target

