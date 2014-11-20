#-------------------------------------------------
#
# Project created by QtCreator 2014-08-07T03:43:34
#
#-------------------------------------------------

QT  += core gui
QT  += xlsx
QT  += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATeachers
TEMPLATE = app

include(qtxlsx/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp\
    commandclassadd.cpp \
    commandclassdelete.cpp \
    commandclassedit.cpp \
    commandclassmove.cpp \
    commandclasspaste.cpp \
    commandteacheradd.cpp \
    commandteacherdelete.cpp \
    commandteacheredit.cpp \
    editteacherdialog.cpp \
    maintablewidget.cpp \
    mainwindow.cpp \
    newclassdialog.cpp \
    newteacherdialog.cpp \
    horizontalheaderview.cpp \
    commandteachermove.cpp \
    rowgradesdialog.cpp \
    maintableoptions.cpp \
    commandclasshighlight.cpp \
    highlightitemdelegate.cpp \
    helpdialog.cpp \
    classhelper.cpp \
    teacherhelper.cpp

HEADERS  += \
    commandclassadd.h \
    commandclassdelete.h \
    commandclassedit.h \
    commandclassmove.h \
    commandclasspaste.h \
    commandteacheradd.h \
    commandteacherdelete.h \
    commandteacheredit.h \
    editteacherdialog.h \
    maintablewidget.h \
    mainwindow.h \
    newclassdialog.h \
    newteacherdialog.h \
    horizontalheaderview.h \
    commandteachermove.h \
    rowgradesdialog.h \
    maintableoptions.h \
    commandclasshighlight.h \
    highlightitemdelegate.h \
    helpdialog.h \
    classhelper.h \
    teacherhelper.h
