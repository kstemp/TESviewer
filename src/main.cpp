#include <QtWidgets/QApplication>

#include "CK.h"

int main(int argc, char* argv[]){

    QApplication a(argc, argv);

    CK w;
    w.show();

    return a.exec();

}