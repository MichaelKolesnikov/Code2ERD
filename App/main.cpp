#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QPluginLoader>
#include <QSettings>
#include <QStyleFactory>
#include "MainWindow.h"

bool isXcbPlatformAvailable()
{
   const QString pluginsPath = QLibraryInfo::location(QLibraryInfo::PluginsPath);
   QPluginLoader loader(pluginsPath + "/platforms/libqxcb.so");
   return loader.load();
}

int main(int argc, char **argv)
{
#ifdef Q_OS_LINUX
   if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM") && isXcbPlatformAvailable())
   {
      qputenv("QT_QPA_PLATFORM", "xcb");
   }
#endif
   QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
   QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

   QApplication app(argc, argv);
   QApplication::setOrganizationName("Code2ERD");
   QApplication::setApplicationName("Code2ERD");
   QApplication::setApplicationVersion("1.0.0");
   QApplication::setStyle(QStyleFactory::create("Fusion"));

   QCommandLineParser parser;
   parser.setApplicationDescription("Entity-relationship diagrams from text");
   parser.addHelpOption();
   parser.addVersionOption();
   parser.addPositionalArgument("file", "An .erd file to open");
   parser.process(app);

   auto mainWindow = new MainWindow();
   mainWindow->setAttribute(Qt::WA_DeleteOnClose);

   const QStringList arguments = parser.positionalArguments();
   if (!arguments.isEmpty())
   {
      mainWindow->openFile(arguments.first());
   }
   else
   {
      const QString lastFile = QSettings().value("lastFile").toString();
      if (!lastFile.isEmpty() && QFileInfo::exists(lastFile))
      {
         mainWindow->openFile(lastFile);
      }
      else if (lastFile.isEmpty() && !QSettings().contains("window/geometry"))
      {
         // the very first start shows what the language looks like
         mainWindow->loadExample();
      }
   }

   mainWindow->show();
   return app.exec();
}
