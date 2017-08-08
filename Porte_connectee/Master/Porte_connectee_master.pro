QT      += core gui
QT      += printsupport
QT      += sql
QT      += serialport

LIBS += -lqtqrencode

QMAKE_CXXFLAGS += -std=c++11

FORMS += \
    acceuil.ui \
    creerauthaurisation.ui \
    row_of_list_acces.ui \
    dialog_log.ui


HEADERS += \
    acceuil.h \
    creerauthaurisation.h \
    row_of_list_acces.h \
    dialog_log.h

SOURCES += \
    acceuil.cpp \
    creerauthaurisation.cpp \
    main.cpp \
    row_of_list_acces.cpp \
    dialog_log.cpp
