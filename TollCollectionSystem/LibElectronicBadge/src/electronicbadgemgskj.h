#ifndef ELECTRONICBADGEMGSKJ_H
#define ELECTRONICBADGEMGSKJ_H

#include <QObject>

#include "CHexBufTool.h"
#include "CommLogger.h"
#include "electronicbadge.h"
#include "serialportutil.h"

class ElectronicBadgeMgskj : public QObject, public ElectronicBadge {
    Q_OBJECT
public:
    explicit ElectronicBadgeMgskj(QObject* parent = nullptr);

    ~ElectronicBadgeMgskj();

    // 建立连接
    virtual bool connect(const char* portName, int baudrate, int parity = 0, int dataBits = 8, int stopBits = 0,
                         int readIntervalTimeout = 0) override;

    // 关闭连接
    virtual void disconnect() override;

    // 文字显示
    virtual int printText(QByteArray content, int color, int fontSize,
                          const SendMessageCallback& callback = nullptr) override;

    // 清屏操作
    virtual int resetScreen(const SendMessageCallback& callback = nullptr) override;

    // 检查连接是否正常
    virtual bool checkConnection() override;

private:
    void checkChar(char* hexBuff, int hexLen);

private:
    SerialPortUtil* m_serialPortUtil = nullptr;
    CommLogger*     m_logger         = nullptr;
    CHexBufTool     m_hexTool;
};

#endif  // ELECTRONICBADGEMGSKJ_H
