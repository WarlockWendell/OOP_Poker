QT       += core gui network
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
    blackjackoffline.cpp \
    blackjackofflinedesk.cpp \
    card.cpp \
    card_ddz.cpp \
    cardpicture.cpp \
    choosegame.cpp \
    clock.cpp \
    handcards.cpp \
    handcards_blackjack.cpp \
    landlordoffline.cpp \
    landlordofflinedesk.cpp \
    main.cpp \
    mainscene.cpp \
    mypushbutton.cpp \
    shuffle.cpp

HEADERS += \
    blackjackoffline.h \
    blackjackofflinedesk.h \
    card.h \
    card_ddz.h \
    cardpicture.h \
    choosegame.h \
    clock.h \
    handcards.h \
    handcards_blackjack.h \
    landlordoffline.h \
    landlordofflinedesk.h \
    mainscene.h \
    mypushbutton.h \
    shuffle.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
