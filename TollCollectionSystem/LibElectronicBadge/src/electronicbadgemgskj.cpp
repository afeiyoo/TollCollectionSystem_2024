#include "electronicbadgemgskj.h"

ElectronicBadgeMgskj::ElectronicBadgeMgskj(QObject *parent) : QObject{parent} {
    // 创建日志
    QString sFilePath = QCoreApplication::applicationDirPath() + "/log";
    QString sFileName = QCoreApplication::applicationDirPath() + "/log/ElectronicBadge.log";
    QDir    dir(sFilePath);
    if (!dir.exists()) dir.mkpath(sFilePath);
    m_logger = new CommLogger(sFileName.toStdString().c_str(), 0);
    m_logger->setsavedays(30);

    // 创建串口对象
    m_serialPortUtil = new SerialPortUtil();
}

ElectronicBadgeMgskj::~ElectronicBadgeMgskj() {
    if (checkConnection()) disconnect();
    delete m_serialPortUtil;
    delete m_logger;
}

bool ElectronicBadgeMgskj::connect(const char *portName, int baudrate, int parity, int dataBits, int stopBits,
                                   int readIntervalTimeout) {
    bool result = m_serialPortUtil->openSerialPort(portName, baudrate, parity, dataBits, stopBits, readIntervalTimeout);
    if (result == false) {
        m_logger->error("串口 %s 打开失败: 错误代码 %d, 错误码信息 %s", portName, m_serialPortUtil->getLastError(),
                        m_serialPortUtil->getLastErrorMsg());
    } else {
        m_logger->info("串口 %s 打开成功", portName);
    }
    return result;
}

void ElectronicBadgeMgskj::disconnect() {
    m_logger->info("串口已关闭");
    m_serialPortUtil->closeSerialPort();
}

int ElectronicBadgeMgskj::printText(QByteArray content, int color, int fontSize, const SendMessageCallback &callback) {
    // 1. 检查连接是否正常
    if (!checkConnection()) {
        m_logger->error("串口未打开，无法发送数据");
        return -1;
    }

    // 2. 组装数据
    char hexContent[96 * 2 + 1] = {0};
    int  contentLen             = content.length();
    m_hexTool.Str2Hex(m_hexTool.FillCharRight(content.constData(), ' ', contentLen, 96), hexContent, 96 * 2);
    checkChar(hexContent, 96 * 2);

    char XORBuff[(1 + 1 + 1 + 96 + 1) * 2 + 1] = {0};  // 需要进行XOR校验的数据内容
    strcpy(XORBuff, "50");                             // CMD

    char colorStr[3] = {0};
    snprintf(colorStr, sizeof(colorStr), "%02d", color);
    strcpy(XORBuff + 2, colorStr);  // Color

    char fontSizeStr[3] = {0};
    snprintf(fontSizeStr, sizeof(fontSizeStr), "%02d", fontSize);
    strcpy(XORBuff + 4, fontSizeStr);  // Fontsize

    strcpy(XORBuff + 6, hexContent);
    m_hexTool.AppendXor(XORBuff);

    char buff[512] = {0};
    strcpy(buff, "0B");
    strcpy(buff + 2, XORBuff);
    strcpy(buff + 2 + (1 + 1 + 1 + 96 + 1) * 2, "0D");

    char hexBuff[512] = {0};  // 最终要发送的数据内容（十六进制）
    m_hexTool.Hex2Buf(buff, hexBuff, strlen(buff));

    // 3. 发送数据
    int        res = m_serialPortUtil->sendSerialPortMsg(QByteArray((char *)hexBuff, strlen(buff) / 2));
    QByteArray sendData;
    if (res < 0) {
        m_logger->error("发送数据失败: 错误码 %d, 错误码信息 %s", m_serialPortUtil->getLastError(),
                        m_serialPortUtil->getLastErrorMsg());
        sendData = "";
    } else {
        sendData = QByteArray((char *)hexBuff, strlen(buff) / 2);
        m_logger->info("发送数据成功: 总体数据 %d 字节，成功发送 %d 字节数据", strlen(buff) / 2, res);
        m_logger->info("数据: %s", sendData.toHex().toUpper().constData());
    }

    if (callback) callback(res, sendData);
    return res;
}

int ElectronicBadgeMgskj::resetScreen(const SendMessageCallback &callback) {
    // 1. 检查连接是否正常
    if (!checkConnection()) {
        m_logger->error("串口未打开，无法发送数据");
        return -1;
    }

    // 2. 组装数据
    char  hexBuff[256] = {0};  // 最终要发送的数据内容（十六进制）
    char *buff         = (char *)"0B9020B00D";
    int   buffLen      = strlen(buff);  // buff 是一个十六进制字符串，每两个字符代表一个字节
    m_hexTool.Hex2Buf(buff, hexBuff, buffLen);

    // 3. 发送数据
    int        res = m_serialPortUtil->sendSerialPortMsg(QByteArray((char *)hexBuff, buffLen / 2));
    QByteArray sendData;
    if (res < 0) {
        m_logger->error("发送数据失败: 错误码 %d, 错误码信息 %s", m_serialPortUtil->getLastError(),
                        m_serialPortUtil->getLastErrorMsg());
        sendData = "";
    } else {
        sendData = QByteArray((char *)hexBuff, strlen(buff) / 2);
        m_logger->info("发送数据成功: 总体数据 %d 字节，成功发送 %d 字节数据", buffLen / 2, res);
        m_logger->info("数据: %s", sendData.toHex().toUpper().constData());
    }

    if (callback) callback(res, sendData);
    return res;
}

bool ElectronicBadgeMgskj::checkConnection() { return m_serialPortUtil->isSerialPortOpened(); }

void ElectronicBadgeMgskj::checkChar(char *hexBuff, int hexLen) {
    if (strlen(hexBuff) < (unsigned int)hexLen) hexLen = strlen(hexBuff);
    int  nTmp     = 0;
    char buf[512] = {0};
    for (int i = 0; i < hexLen; i = i + 2) {
        sscanf(hexBuff + i, "%02X", &nTmp);
        if (nTmp == 11 || nTmp == 13)
            sprintf(buf + i, "%02X", 32);
        else
            sprintf(buf + i, "%02X", nTmp);
        nTmp = 0;
    }
    strcpy(hexBuff, buf);
}
