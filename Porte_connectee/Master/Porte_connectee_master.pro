QT      += core gui
QT      += printsupport
QT      += sql

LIBS += -lqtqrencode

FORMS += \
    acceuil.ui \
    creerauthaurisation.ui \
    generation_qrcode.ui \
    row_of_list_acces.ui \


HEADERS += \
    acceuil.h \
    creerauthaurisation.h \
    generation_qrcode.h \
    row_of_list_acces.h \

SOURCES += \
    acceuil.cpp \
    creerauthaurisation.cpp \
    generation_qrcode.cpp \
    main.cpp \
    row_of_list_acces.cpp
