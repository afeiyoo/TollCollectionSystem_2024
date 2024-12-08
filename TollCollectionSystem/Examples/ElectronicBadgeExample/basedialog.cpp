#include "basedialog.h"

#include <QDateTime>
#include <QLibrary>

#include "inputdialog.h"
#include "ui_basedialog.h"

BaseDialog::BaseDialog(QWidget* parent) : QDialog(parent), ui(new Ui::BaseDialog) {
    ui->setupUi(this);
    setWindowTitle(this->windowTitle() + " 电子工牌设备调试工具");

    // init
    QVector<SerialPortInfo> serialPortsList = m_serialPortUtil.availableSerialPorts();
    for (auto port : serialPortsList) {
        int i = 0;
        ui->cmbSerialPorts->insertItem(i++, QString(port.portName));
    }

    int baudrates[] = {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 115200, 921600};
    for (size_t i = 0; i < sizeof(baudrates) / sizeof(int); i++) {
        ui->cmbBaudrates->insertItem(i, QString::number(baudrates[i]));
    }

    ui->cmbParities->insertItem(0, QString("无校验"));
    ui->cmbParities->insertItem(1, QString("奇校验"));
    ui->cmbParities->insertItem(2, QString("偶校验"));
    ui->cmbParities->insertItem(3, QString("1校验"));
    ui->cmbParities->insertItem(4, QString("0校验"));

    ui->cmbDataBits->insertItem(0, QString::number(5));
    ui->cmbDataBits->insertItem(1, QString::number(6));
    ui->cmbDataBits->insertItem(2, QString::number(7));
    ui->cmbDataBits->insertItem(3, QString::number(8));

    ui->cmbStopBits->insertItem(0, QString::number(1));
    ui->cmbStopBits->insertItem(1, QString::number(1.5));
    ui->cmbStopBits->insertItem(2, QString::number(2));

    ui->cmbReadInterval->insertItem(0, QString::number(0));
    ui->cmbReadInterval->insertItem(1, QString::number(5));
    ui->cmbReadInterval->insertItem(2, QString::number(10));

    ui->cmbBaudrates->setCurrentText(tr("9600"));
    ui->cmbDataBits->setCurrentText(tr("8"));

    ui->cmbCmd->insertItem(0, tr("整屏清空"));
    ui->cmbCmd->insertItem(1, tr("整屏显示"));

    ui->pedtReceive->setDisabled(true);

    loadLib("LibElectronicBadge");  // 加载动态库
}

BaseDialog::~BaseDialog() {
    delete ui;
    delete m_electronicBadge;
}

void BaseDialog::on_btnOpenDevice_clicked() {
    if (ui->btnOpenDevice->text() == tr("关闭设备")) {
        m_electronicBadge->disconnect();
        ui->pedtReceive->insertPlainText(
            QString("%1 设备已关闭!\n\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")));
        ui->btnOpenDevice->setText("打开设备");
        ui->btnOpenDevice->setStyleSheet(QString(""));
        return;
    }

    const char* portName     = ui->cmbSerialPorts->currentText().toLocal8Bit().constData();
    int         baudrate     = ui->cmbBaudrates->currentText().toInt();
    int         parity       = ui->cmbParities->currentIndex();
    int         databits     = ui->cmbDataBits->currentText().toInt();
    int         stopbits     = ui->cmbStopBits->currentIndex();
    int         readInterval = ui->cmbReadInterval->currentText().toInt();

    bool res = m_electronicBadge->connect(portName, baudrate, parity, databits, stopbits, readInterval);
    if (!res) {
        ui->pedtReceive->insertPlainText(
            QString("%1 设备打开失败!\n\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")));
        ui->pedtReceive->insertPlainText(QString("%1 错误码: %2, 错误信息: %3\n\n")
                                             .arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz"))
                                             .arg(m_serialPortUtil.getLastError())
                                             .arg(m_serialPortUtil.getLastErrorMsg()));
    } else {
        ui->pedtReceive->insertPlainText(
            QString("%1 设备打开成功!\n\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")));
        ui->btnOpenDevice->setText(tr("关闭设备"));
        ui->btnOpenDevice->setStyleSheet(QString("background-color: yellow;"));
    }
    ui->pedtReceive->moveCursor(QTextCursor::NextRow);
}

void BaseDialog::on_btnClear_clicked() {
    ui->pedtReceive->clear();
    ui->labelSendBytes->setText(tr("发送数据(Bytes)"));
}

void BaseDialog::on_pushButton_clicked() {
    if (!m_electronicBadge->checkConnection()) {
        ui->pedtReceive->insertPlainText(
            QString("%1 请先打开设备\n\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")));
        ui->pedtReceive->moveCursor(QTextCursor::NextRow);
        return;
    }

    auto callback = [=](int dataLen, QByteArray data) {
        if (dataLen < 0) {
            ui->pedtReceive->insertPlainText(
                QString("%1 数据写入失败\n\n").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")));
        } else {
            ui->pedtReceive->insertPlainText(QString("%1 数据写入成功 写入数据长度: %2 数据内容: %3\n\n")
                                                 .arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz"))
                                                 .arg(dataLen)
                                                 .arg(data.toHex().toUpper().constData()));
            ui->labelSendBytes->setText(tr("发送数据(Bytes): %1").arg(dataLen));
        }
        ui->pedtReceive->moveCursor(QTextCursor::NextRow);
    };

    int cmd = ui->cmbCmd->currentIndex();
    switch (cmd) {
        case 0: {
            // 整屏清空
            m_electronicBadge->resetScreen(callback);
        } break;
        case 1: {
            // 整屏显示
            InputDialog inputDialog;
            if (inputDialog.exec() == QDialog::Accepted) {
                QJsonObject inputData = inputDialog.inputJson();
                int         color     = inputData.value("color").toInt();
                int         fontSize  = inputData.value("font-size").toString().toInt();
                QByteArray  content   = inputData.value("content").toString().toLocal8Bit();
                m_electronicBadge->printText(content, color, fontSize, callback);
            }
        } break;
        default:
            break;
    }
}

void BaseDialog::loadLib(QString libName) {
    QLibrary lib(libName);
    if (lib.isLoaded()) return;

    FuncGetElectronicBadge GetElectronicBadge = (FuncGetElectronicBadge)lib.resolve("getElectronicBadge");
    m_electronicBadge                         = GetElectronicBadge(MGSKJ_ELECTRONIC_BADGE);
}
