#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QAbstractButton>
#include <QComboBox>
#include <QDialog>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog {
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    QJsonObject inputJson() const;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::InputDialog *ui;

    QLabel    *label1 = nullptr;
    QLabel    *label2 = nullptr;
    QLabel    *label3 = nullptr;
    QLineEdit *input1 = nullptr;
    QComboBox *input2 = nullptr;
    QComboBox *input3 = nullptr;

    QJsonObject m_inputJson;
};

#endif  // INPUTDIALOG_H
