#include "LocalScriptsController.hxx"
#include "Application.hxx"
#include "ApplicationLogger.hxx"
#include "EditorController.hxx"
#include <QApplication>
#include <QDebug>

namespace Ide::Ui {

LocalScriptsController *LocalScriptsController::instance = nullptr;
qml::RegisterType<LocalScriptsController> LocalScriptsController::Register;

LocalScriptsController::LocalScriptsController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of local_scripts_controller already exists");
    }
    setupProcess();
}

LocalScriptsController *LocalScriptsController::Create()
{
    instance = new LocalScriptsController();
    return instance;
}

void LocalScriptsController::run()
{
    auto script_path = Ide::Ui::EditorController::instance->getFileUrl();

    if (script_path.size() < 2) {
        ApplicationLogger::instance->addEntry("Unable to start: script does not exists.");
        return;
    }

    auto dirExec = QDir{QApplication::applicationDirPath()};
    auto pathToDirExec = dirExec.absolutePath();
    auto pathToPython = pathToDirExec + "/../share/mur-ide/venv/bin/python3";

    QString runCommand = "xterm -e \"" + pathToPython + " " + script_path + " & sleep 99999\"";

    qDebug() << runCommand;
    m_scriptProcess->start(runCommand);
    m_scriptProcess->waitForStarted();
    m_pid = m_scriptProcess->pid();
    ApplicationLogger::instance->addEntry("Program started.\n");

    return;
}

void LocalScriptsController::stop()
{
    if (m_scriptProcess->state() == QProcess::NotRunning) {
        return;
    }
    m_scriptProcess->terminate();
}

bool LocalScriptsController::isRunning()
{
    return m_scriptProcess->state() == QProcess::Running;
}

bool LocalScriptsController::isLocal()
{
    return m_isLocal;
}

void LocalScriptsController::setLocal()
{
    m_isLocal = true;
    emit targetStateChanged();
}

void LocalScriptsController::setRemote()
{
    if (isRunning()) {
        m_scriptProcess->kill();
    }

    m_isLocal = false;
    emit targetStateChanged();
}

LocalScriptsController::~LocalScriptsController()
{
    if (m_scriptProcess == nullptr) {
        return;
    }

    if (m_scriptProcess->state() == QProcess::Running) {
        m_scriptProcess->kill();
        m_scriptProcess->waitForFinished();
    }
}

void LocalScriptsController::setupProcess()
{
    m_scriptProcess = new QProcess{};

    connect(m_scriptProcess,
            &QProcess::started,
            this,
            &LocalScriptsController::runningStateChanged);

    connect(m_scriptProcess,
            qOverload<int>(&QProcess::finished),
            this,
            &LocalScriptsController::runningStateChanged);
}

} // namespace ide::ui
