#-------------------------------------------------
#
# Project created by QtCreator 2014-06-16T13:13:39
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = xvpSample
TEMPLATE = lib
CONFIG += plugin

include(xvpSample.pri)

SOURCES += sampleplugin.cpp \
    MeanGrayMeasGraphDlg.cpp \
    NegativeChnbl.cpp \
    BayerPseudoColorsChnbl.cpp \
    MeanGrayMeasForm.cpp \
    FlipImageCnbl.cpp \
    FlipConfigurationWgt.cpp \
    CameraTriggerForm.cpp

HEADERS += sampleplugin.h \
    MeanGrayMeasGraphDlg.h \
    NegativeChnbl.h \
    BayerPseudoColorsChnbl.h \
    MeanGrayMeasForm.h \
    FlipImageCnbl.h \
    FlipConfigurationWgt.h \
    CameraTriggerForm.h

FORMS += \
    MeanGrayMeasForm.ui \
    FlipConfigurationWgt.ui \
    CameraTriggerForm.ui \
    MeanGrayMeasGraphDlg.ui

RESOURCES += \
    xvpSample.qrc
