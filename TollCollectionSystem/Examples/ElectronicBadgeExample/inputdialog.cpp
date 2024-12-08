#include "inputdialog.h"

#include <QDebug>

#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InputDialog) {
    ui->setupUi(this);

    label1 = new QLabel("请输入显示内容:", this);
    input1 = new QLineEdit(this);

    label2 = new QLabel("请选择字体大小:", this);
    input2 = new QComboBox(this);
    input2->insertItem(0, QString::number(12));
    input2->insertItem(1, QString::number(14));
    input2->insertItem(2, QString::number(16));

    label3 = new QLabel("请选择显示颜色:", this);
    input3 = new QComboBox(this);
    input3->insertItem(0, tr("0 默认"));
    input3->insertItem(1, tr("1 红色"));
    input3->insertItem(2, tr("2 绿色"));
    input3->insertItem(3, tr("3 黄色"));

    // 添加到布局
    ui->verticalLayoutContent->addWidget(label1);
    ui->verticalLayoutContent->addWidget(input1);

    ui->verticalLayoutContent->addWidget(label2);
    ui->verticalLayoutContent->addWidget(input2);

    ui->verticalLayoutContent->addWidget(label3);
    ui->verticalLayoutContent->addWidget(input3);
}

InputDialog::~InputDialog() {
    delete ui;
    delete label1;
    delete label2;
    delete label3;
    delete input1;
    delete input2;
    delete input3;
}

void InputDialog::on_buttonBox_clicked(QAbstractButton *button) {
    QDialogButtonBox::StandardButton btnRole = ui->buttonBox->standardButton(button);
    if (btnRole == QDialogButtonBox::Ok) {
        // 发送 accept 信号
        m_inputJson.insert("content", input1->text());
        m_inputJson.insert("font-size", input2->currentText());
        m_inputJson.insert("color", input3->currentIndex());
        emit accept();
    } else if (btnRole == QDialogButtonBox::Cancel) {
        // 发送 reject 信号
        emit reject();
    }
}

QJsonObject InputDialog::inputJson() const { return m_inputJson; }
