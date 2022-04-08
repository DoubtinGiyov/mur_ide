#include "Application.hxx"
#include "ApplicationController.hxx"
#include "QmlImageItem.hxx"
#include "QmlUtils.hxx"
#include "RemoteController.hxx"

#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFontDatabase>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QWindow>
#include <QtWebEngine>

namespace Ide::Ui {

Application *Application::instance = nullptr;
QString Application::m_resourceDirectory = {};

int Application::execute(int argc, char **argv)
{
    QtWebEngine::initialize();
    QApplication app(argc, argv);

    if (instance == nullptr) {
        instance = new Application;

        Ide::Ui::Application::initialize();
        return QApplication::exec();
    }

    /* Something really bad happened */
    return 0xDEADBEAF;
}

QString Application::getResourcesDirectory()
{
    return m_resourceDirectory;
}

void Application::initialize()
{
    auto path = QDir{QApplication::applicationDirPath()};

    for (const auto &c : path.absolutePath()) {
        if (c.unicode() > 127) {
            QMessageBox::critical(nullptr,
                                  "Error (Ошибка)",
                                  "Looks like the application path contains non ACII characters. "
                                  "Please move it to another directory or reinstall it\n"
                                  "Путь к приложению содержит символы отличные от ACII (русские "
                                  "буквы). Переместите приложение или переустановите его.",
                                  "OK");
            exit(0xDEADBEAF);
        }
    }

    m_resourceDirectory = path.absolutePath() + "/resources/";
    setupEnvironment();

    Ide::qml::InitializeControllers();

    auto engine = new QQmlApplicationEngine;
    auto context = engine->rootContext();
    context->setContextProperty("Controllers", ApplicationController::instance);

    addFontDirectory();

    engine->addImportPath("qrc:/qml");
    engine->addImportPath("qrc:/qml/Ui");
    engine->addImportPath("qrc:/qml/UiElements");
    qmlRegisterType<QMLImageItem>("mur", 1, 0, "ImageItem");
    engine->load("qrc:/qml/Ui/MainWindow.qml");

    auto root_object = engine->rootObjects().at(0);
    auto window = dynamic_cast<QWindow *>(root_object);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setDepthBufferSize(1);
    format.setStencilBufferSize(2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    window->setFormat(format);
}

void Application::setupEnvironment()
{
    SetEnvironmentVariable("GST_PLUGIN_PATH", m_resourceDirectory + "gstplugins/");

    SetEnvironmentVariable("GST_DEBUG", QString("0"));
    
#ifdef Q_OS_MACOS
    auto path = QDir{QApplication::applicationDirPath()};
    path.cdUp();
    auto contentsDirectory = path.absolutePath();
    auto gstFrameworkDirectory = contentsDirectory + "/Frameworks/GStreamer.framework";
    
    SetEnvironmentVariable("GST_PLUGIN_SYSTEM_PATH",
                           gstFrameworkDirectory + "/Versions/Current/lib/gstreamer-1.0");
    
    SetEnvironmentVariable("GST_PLUGIN_SCANNER",
                           gstFrameworkDirectory + "/Versions/Current/libexec/gstreamer-1.0/gst-plugin-scanner");
    
    SetEnvironmentVariable("GTK_PATH",
                           gstFrameworkDirectory + "/Versions/Current/");
    
    SetEnvironmentVariable("GIO_EXTRA_MODULES",
                           gstFrameworkDirectory + "/Versions/Current/lib/gio/modules");
#endif
}

void Application::SetEnvironmentVariable(const char* variableName, const QString variableValue)
{
    if (!qputenv(variableName, variableValue.toUtf8()))
    {
        qWarning() << "Unable to set " << variableName;
    }
}

void Application::addFontDirectory()
{
    QFontDatabase::addApplicationFont(":/fonts/fontawesome-webfont.ttf");
}
} // namespace Ide::Ui
