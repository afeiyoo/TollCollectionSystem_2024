QT -= gui

TEMPLATE = lib
DEFINES += LIBELECTRONICBADGE_LIBRARY
DEFINES += _UNICODE

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/electronicbadge.cpp \
    src/electronicbadgemgskj.cpp

HEADERS += \
    src/LibElectronicBadge_global.h \
    src/electronicbadge.h \
    src/electronicbadgemgskj.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

include($$PWD/../CommonUtils/commonutils.pri)

win32:LIBS += -lsetupapi
unix:!macx:LIBS += -lpthread
macx {
    LIBS += -framework IOKit
    LIBS += -framework Foundation
}
