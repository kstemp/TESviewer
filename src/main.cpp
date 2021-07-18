#include <QtWidgets/QApplication>
#include <QFile>

#include "CK.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	// apply stylesheet
	QFile f(":qdarkstyle/dark/style.qss");
	f.open(QFile::ReadOnly | QFile::Text);

	QTextStream ts(&f);

	qApp->setStyleSheet(ts.readAll());

	// main window
	CK w;
	w.show();

	return a.exec();
}