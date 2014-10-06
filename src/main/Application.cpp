
#include <QtGlobal>

#if QT_VERSION < 0x050000
#include <QtGui/QApplication>
#include <QtGui/QWindowsStyle>
#else
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#endif

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setOrganizationName("SvOlli");
   a.setOrganizationDomain("svolli.org");
   a.setApplicationName("apefat");
#if QT_VERSION < 0x050000
   // workaround: seems to be the most compact
   QApplication::setStyle( new QWindowsStyle() );
#else
   //qDebug() << QStyleFactory::keys();
   QApplication::setStyle( QStyleFactory::create( QString("Windows") ) );
#endif

   MainWindow w;
   w.show();

   return a.exec();
}
