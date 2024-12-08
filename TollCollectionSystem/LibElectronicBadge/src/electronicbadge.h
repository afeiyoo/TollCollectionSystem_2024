#ifndef ELECTRONICBADGE_H
#define ELECTRONICBADGE_H

#include <functional>

#include "LibElectronicBadge_global.h"

// 定义回调函数，用于回调获取输入输出
using SendMessageCallback = std::function<void(int dataLen, QByteArray data)>;

class LIBELECTRONICBADGE_EXPORT ElectronicBadge {
public:
    ElectronicBadge();

    virtual ~ElectronicBadge() = 0;

    // 建立连接
    virtual bool connect(const char* portName, int baudarate, int parity = 0, int dataBits = 8, int stopBits = 0,
                         int readIntervalTimeout = 0) = 0;

    // 关闭连接
    virtual void disconnect() = 0;

    // 文字显示
    virtual int printText(QByteArray content, int color, int fontSize,
                          const SendMessageCallback& callback = nullptr) = 0;

    // 清屏操作
    virtual int resetScreen(const SendMessageCallback& callback = nullptr) = 0;

    // 检查连接是否正常
    virtual bool checkConnection() = 0;
};

extern "C" LIBELECTRONICBADGE_EXPORT ElectronicBadge* getElectronicBadge(int type);

#endif  // ELECTRONICBADGE_H
