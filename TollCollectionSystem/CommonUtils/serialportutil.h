#ifndef CSERIALHANDLER_H
#define CSERIALHANDLER_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <vector>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

using namespace itas109;
class SerialPortUtil : public QObject, public CSerialPortListener {
    Q_OBJECT
public:
    explicit SerialPortUtil(QObject *parent = 0);

    ~SerialPortUtil();

    // 打开串口
    bool openSerialPort(const char *portName, int baudrate = BaudRate::BaudRate9600, int parity = Parity::ParityNone,
                        int dataBits = DataBits::DataBits8, int stopBits = StopBits::StopOne,
                        int readIntervalTimeout = 0);

    // 关闭串口
    void closeSerialPort();

    // 获取串口最后的错误码
    int getLastError();

    // 获取串口最后的错误码信息
    const char *getLastErrorMsg();

    // 检查串口是否正常已打开
    bool isSerialPortOpened();

    // 向串口发送数据
    int sendSerialPortMsg(QByteArray msg);

    // 获取可用串口信息
    QVector<SerialPortInfo> availableSerialPorts();

protected:
    // 监听串口数据
    void onReadEvent(const char *portName, unsigned int readBufferLen) override;

private:
    CSerialPort *m_serialPort = nullptr;

signals:
    // 数据接收成功信号
    void receivedInfoSuccess(QByteArray data);

    // 数据接收失败信号
    void receivedInfoFailed();
};

#endif  // CSERIALHANDLER_H
