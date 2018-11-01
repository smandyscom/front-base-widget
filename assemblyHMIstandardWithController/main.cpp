#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		a.exec();
	}
	catch (const std::exception &e)
	{
		qDebug() << e.what();
	}

	return 0;
}
