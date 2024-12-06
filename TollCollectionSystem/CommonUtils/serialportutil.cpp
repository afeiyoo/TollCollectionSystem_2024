#include "serialportutil.h"

SerialPortUtil::SerialPortUtil(QObject *parent) : QObject(parent) { m_serialPort = new CSerialPort(); }

SerialPortUtil::~SerialPortUtil() {
    m_serialPort->disconnectReadEvent();
    delete m_serialPort;
    m_serialPort = nullptr;
}

bool SerialPortUtil::openSerialPort(const char *portName, int baudrate, int parity, int dataBits, int stopBits) {
    // 串口已经打开，则清空串口数据内容后，重新打开
    if (m_serialPort->isOpen()) {
        m_serialPort->clearError();
        m_serialPort->flushBuffers();
        m_serialPort->close();
        m_serialPort->disconnectReadEvent();
    }

    m_serialPort->connectReadEvent(this);  // 连接读取事件
    m_serialPort->init(portName, BaudRate(baudrate), Parity(parity), DataBits(dataBits),
                       StopBits(stopBits));   // 串口初始化
    m_serialPort->setReadIntervalTimeout(0);  // 设置串口读取时间间隔

    return m_serialPort->open();  // 打开串口
}

void SerialPortUtil::closeSerialPort() { m_serialPort->close(); }

int SerialPortUtil::getLastError() { return m_serialPort->getLastError(); }

const char *SerialPortUtil::getLastErrorMsg() { return m_serialPort->getLastErrorMsg(); }

bool SerialPortUtil::isSerialPortOpened() { return m_serialPort->isOpen(); }

int SerialPortUtil::sendSerialPortMsg(QByteArray msg) {
    if (!m_serialPort->isOpen()) {
        return -1;
    }

    m_serialPort->flushWriteBuffers();
    int res = m_serialPort->writeData(msg.constData(), msg.length());

    return res;
}

void SerialPortUtil::onReadEvent(const char *portName, unsigned int readBufferLen) {
    if (readBufferLen > 0) {
        int  recLen     = 0;
        char buff[1024] = {0};
        recLen          = m_serialPort->readData(buff, readBufferLen);
        if (recLen > 0) {
            // 接收数据成功
            QByteArray data(buff, recLen);
            emit       receivedInfoSuccess(data);
        } else {
            emit receivedInfoFailed();
        }
    }
}
