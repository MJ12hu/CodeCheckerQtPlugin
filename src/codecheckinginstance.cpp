#include "codecheckinginstance.h"
#include <coreplugin/progressmanager/progressmanager.h>
#include <QFuture>
#include <QString>
#include "codecheckerutils.h"

CodeCheckingInstance::CodeCheckingInstance(QString p_name, QString p_buildPath)
    : m_name(p_name)
{
    m_buildProgress = new QFutureInterface<void>;
    m_buildProgress->setProgressRange(0, 100);
    Core::ProgressManager::addTask(m_buildProgress->future(), tr("Analyzing"), "CodeChecker.Analyze");
    m_buildProgress->reportStarted();
    m_buildProgress->setProgressValue(50);
    m_analyzeProcess = std::make_shared<QProcess>();
    connect(m_analyzeProcess.get(), SIGNAL(finished(int)), this, SLOT(analyzeProcessStateChanged()));
    m_analyzeProcess->start("bash", QStringList() << "-c" << "source " + CodeCheckerUtils::codeCheckerEnv + " && " + CodeCheckerUtils::codeCheckerPath + " analyze " + p_buildPath + "/codechecker_compiledb.json -o " + p_buildPath + "/codecheckery_analysis && " + CodeCheckerUtils::codeCheckerPath + " store --name " + p_name + " " + p_buildPath + "/codecheckery_analysis");
}

CodeCheckingInstance::~CodeCheckingInstance()
{
    delete m_buildProgress;
}

void CodeCheckingInstance::analyzeProcessStateChanged()
{
    m_buildProgress->reportFinished();
    emit analyzeFinished(m_name);
}
