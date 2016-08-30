#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T18:11:15
#
#-------------------------------------------------

QT       += core gui network sql positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VetEpiGIS
TEMPLATE = app

deployment.files += db.sqlite
deployment.path = /assets
INSTALLS += deployment

SOURCES += main.cpp\
        frmmain.cpp \
    xaffected.cpp

HEADERS  += frmmain.h \
    xaffected.h

FORMS    += frmmain.ui \
    xaffected.ui

CONFIG += mobility
MOBILITY += location sql

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/AndroidManifest.xml \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/gradle/wrapper/gradle-wrapper.jar \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/gradlew \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/res/values/libs.xml \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/build.gradle \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/gradle/wrapper/gradle-wrapper.properties \
    ../../../../../../media/sn/WD2T/dev/QGISplugins/VetEpiGIS/mobile/android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
