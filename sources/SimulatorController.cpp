#include "SimulatorController.hxx"
#include "Application.hxx"
#include <QDebug>
#include <QApplication>

namespace Ide::Ui {

SimulatorController *SimulatorController::instance = nullptr;
qml::RegisterType<SimulatorController> SimulatorController::Register;

SimulatorController::SimulatorController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of simulator_controller already exists");
    }
    setup_process();
}

void SimulatorController::setup_process()
{
    m_simulator_process = new QProcess{};

    connect(m_simulator_process,
            &QProcess::started,
            this,
            &SimulatorController::runningStateChanged);

    connect(m_simulator_process,
            qOverload<int>(&QProcess::finished),
            this,
            &SimulatorController::runningStateChanged);
}

SimulatorController *Ide::Ui::SimulatorController::Create()
{
    instance = new SimulatorController{};
    return instance;
}

void SimulatorController::run()
{
    if (m_simulator_process->state() != QProcess::NotRunning) return;

    auto dirExec = QDir{QApplication::applicationDirPath()};
    auto pathToDirExec = dirExec.absolutePath();
    auto pathToSim = pathToDirExec + "/simulator";
    m_simulator_process->setProgram(pathToSim);
    m_simulator_process->start();
}

bool SimulatorController::isRunning()
{
    return m_simulator_process->state() == QProcess::Running;
}

SimulatorController::~SimulatorController()
{
    if (m_simulator_process == nullptr) {
        return;
    }

    if (m_simulator_process->state() == QProcess::Running) {
        m_simulator_process->kill();
        m_simulator_process->waitForFinished();
    }
}

} // namespace ide::ui
