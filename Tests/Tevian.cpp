#include "Defines.hpp"
#include "Gui/Window.hpp"
#include <QApplication>


using namespace Tevian;
using namespace Tevian::Gui;
using namespace Tevian::Client;

int main(int argc, char** argv) try
{
	QCoreApplication::setOrganizationName(ORGANIZATION);
	QCoreApplication::setOrganizationDomain(BACKEND_URL);
	QCoreApplication::setApplicationName(PROJECT_NAME);
	QCoreApplication::setApplicationVersion(PROJECT_VERSION);
	
	QApplication app(argc, argv);
	Window mainWin;
	mainWin.show();
	return app.exec();
} catch (std::exception& e)
{
	qWarning() << e.what();
}