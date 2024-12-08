
#include <QApplication>

#include "basedialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    BaseDialog w;
    w.show();
    return a.exec();
}
