#include "UpdateController.hxx"

#include <QApplication>
#include <QMessageBox>
#include <QNetworkConfigurationManager>
#include <QtConcurrent>

namespace Ide::Ui {

UpdateController *UpdateController::instance = nullptr;
qml::RegisterType<UpdateController> UpdateController::Register;

UpdateController::UpdateController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of UpdateController already exists");
    }

    loadSettings();
    QtConcurrent::run([this]() { onCheckConnection(); });
}

void UpdateController::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    m_isCheckForUpdate = settings.value("checkForUpdate", true).toBool();
}

void UpdateController::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("checkForUpdate", m_isCheckForUpdate);
}

UpdateController *UpdateController::Create()
{
    instance = new UpdateController();
    return instance;
}

bool UpdateController::isUpdateAvailable()
{
    return m_isUpdateAvailable;
}

bool UpdateController::isConnected()
{
    return m_isConnected;
}

bool UpdateController::isCheckForUpdate()
{
    return m_isCheckForUpdate;
}

void UpdateController::setCheckForUpdate(bool flag)
{
    m_isCheckForUpdate = flag;
    emit checkForUpdate();
    saveSettings();
}

void UpdateController::onCheckForUpdates()
{
#ifdef Q_OS_LINUX
    return;
#endif

    QNetworkConfigurationManager manager;

    if (!manager.isOnline()) return;

    QProcess process;
    
#ifdef Q_OS_WIN32
    process.setProgram("maintenancetool");
    process.setArguments(QStringList() << "--checkupdates");
#endif
    
#ifdef Q_OS_MACOS
    auto dirExec = QDir{QApplication::applicationDirPath()};
    auto pathToDirExec = dirExec.absolutePath();
    auto pathToDefaultDir = pathToDirExec + "/../../..";
    process.setProgram(pathToDefaultDir + "/maintenancetool.app/Contents/MacOS/MaintenanceTool");
    process.setArguments(QStringList() << "--checkupdates");
#endif
    
    process.start();

    process.waitForFinished();

    if (process.error() != QProcess::UnknownError) {
        return;
    }

    if (process.readAllStandardOutput().contains("<update")) {
        m_isUpdateAvailable = true;
        emit updateAvailable();
    }
    else {
        m_isUpdateAvailable = false;
        emit updateAvailable();
    }
}

void UpdateController::onUpdate()
{
    int ret = QMessageBox::question(nullptr,
                                    tr("murIDE"),
                                    tr("Update process require closing the IDE.\n"
                                       "Do you want to proceed?"),
                                    QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }
    
    QStringList args("--updater");
    
#ifdef Q_OS_WIN32
    QProcess::startDetached("maintenancetool", args);
#endif
    
#ifdef Q_OS_MACOS
    auto dirExec = QDir{QApplication::applicationDirPath()};
    auto pathToDirExec = dirExec.absolutePath();
    auto pathToDefaultDir = pathToDirExec + "/../../..";
    QProcess::startDetached(pathToDefaultDir + "/maintenancetool.app/Contents/MacOS/MaintenanceTool", args);
#endif
    
    QApplication::exit(0);
}

void UpdateController::onCheckConnection()
{
    if (!m_isCheckForUpdate) {
        return;
    }

    QProcess process;
    process.start("ping -n 1 8.8.8.8");
    process.waitForFinished(1000);

    if (process.exitCode() == 0) {
        m_isConnected = true;
        onCheckForUpdates();
        return;
    }
}

} // namespace Ide::Ui
