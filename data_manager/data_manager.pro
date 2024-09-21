QT       += core gui
QT       += network printsupport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    human_plot.cpp \
    login_dialog.cpp \
    manager_database.cpp \
    qcustomplot.cpp \
    data_window.cpp \
    main.cpp \
    main_window.cpp \
    settings_window.cpp \
    user_list_item.cpp \
    user_modifier_window.cpp \
    user_adder_window.cpp \
    users_management_window.cpp \
    users_manager.cpp \
    tcp_server.cpp \
    communication.cpp \
    waveform_window.cpp

HEADERS += \
    human_plot.h \
    login_dialog.h \
    manager_database.h \
    qcustomplot.h \
    data_window.h \
    main_window.h \
    settings_window.h \
    user_list_item.h \
    user_modifier_window.h \
    user_adder_window.h \
    users_management_window.h \
    users_manager.h \
    tcp_server.h \
    communication.h \
    waveform_window.h

FORMS += \
    data_window.ui \
    login_dialog.ui \
    main_window.ui \
    settings_window.ui \
    user_list_item.ui \
    user_modifier_window.ui \
    user_adder_window.ui \
    users_management_window.ui \
    waveform_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
