#ifndef ELECTRONICBADGE_H
#define ELECTRONICBADGE_H

#include "LibElectronicBadge_global.h"

class LIBELECTRONICBADGE_EXPORT ElectronicBadge {
public:
    ElectronicBadge();

    // 建立连接
    virtual int connect(const char* portName, int rate, int sleep) = 0;

    // 关闭连接
    virtual void disconnect() = 0;

    // 文字显示
    virtual int printText(QByteArray content) = 0;

    // 清屏操作
    virtual int resetScreen() = 0;

    // 检查连接是否正常
    virtual bool checkConnection() = 0;
};

extern "C" LIBELECTRONICBADGE_EXPORT ElectronicBadge* getElectronicBadge(int type);

#endif  // ELECTRONICBADGE_H
