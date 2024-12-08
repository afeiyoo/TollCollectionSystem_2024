#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QObject>

#include "include/electronicbadge.h"
#include "serialportutil.h"

namespace Ui {
class BaseDialog;
}

typedef ElectronicBadge *(*FuncGetElectronicBadge)(int);  // 定义函数指针

class BaseDialog : public QDialog {
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr);
    ~BaseDialog();

private slots:
    void on_btnOpenDevice_clicked();

    void on_btnClear_clicked();

    void on_pushButton_clicked();

private:
    void loadLib(QString libName);

    void getElectronicBadgeIO(int dataLen, QByteArray data);

private:
    Ui::BaseDialog *ui;

    int              m_receiveBytes    = 0;        // 接收数据量
    int              m_transferBytes   = 0;        // 发送数据量
    ElectronicBadge *m_electronicBadge = nullptr;  // 电子工牌对象
    SerialPortUtil   m_serialPortUtil;             // 串口库工具
};

#endif  // BASEDIALOG_H
