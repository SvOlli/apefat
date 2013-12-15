#include <QtGui/QApplication>
#include <QtGui/QWindowsStyle>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{

   QApplication a(argc, argv);
   a.setOrganizationName("SvOlli");
   a.setOrganizationDomain("svolli.org");
   a.setApplicationName("apefat");
   // workaround: seems to be the most compact
   QApplication::setStyle( new QWindowsStyle() );

   MainWindow w;
   w.show();

   return a.exec();
}
