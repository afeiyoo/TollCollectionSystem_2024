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
    virtual int connect(const char* portName, int rate, int sleep) override;

    // 关闭连接
    virtual void disconnect() override;

    // 文字显示
    virtual int printText(QByteArray content) override;

    // 清屏操作
    virtual int resetScreen() override;

    // 检查连接是否正常
    virtual bool checkConnection() override;

private:
    void checkChar(char* hexBuff, int hexLen);

signals:

private:
    SerialPortUtil* m_serialPortUtil = nullptr;
    CommLogger*     m_logger         = nullptr;
    CHexBufTool     m_hexTool;
};

#endif  // ELECTRONICBADGEMGSKJ_H
