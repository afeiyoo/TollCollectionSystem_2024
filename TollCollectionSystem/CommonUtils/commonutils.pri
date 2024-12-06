HEADERS += \
    $$PWD/CHexBufTool.h \
    $$PWD/CommLogger.h \
    $$PWD/CommonTool.h  \
    $$PWD/serialportutil.h

SOURCES += \
    $$PWD/serialportutil.cpp \
    $$PWD/CHexBufTool.cpp \
    $$PWD/CommLogger.cpp \
    $$PWD/CommonTool.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# 引入 CSerialPort 库
include($$PWD/../third/CSerialPort/cserialport.pri)

win32:LIBS += -lsetupapi
unix:!macx:LIBS += -lpthread
macx {
    LIBS += -framework IOKit
    LIBS += -framework Foundation
}
