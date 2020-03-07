#include "RMR_QT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RMR_QT w;
	w.show();
	return a.exec();
}
