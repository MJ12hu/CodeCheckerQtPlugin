#include "codecheckerservice.h"
#include <QDateTime>
#include <coreplugin/progressmanager/progressmanager.h>
#include <QDebug>
#include "codecheckerutils.h"

CodeCheckerService::CodeCheckerService(QObject *parent) : QObject(parent)
{
    m_serverProcess = std::make_shared<QProcess>();
}

void CodeCheckerService::settingsChanged()
{
    startServer();
}

CodeCheckingInstance* CodeCheckerService::analyze(QString p_buildPath, QString p_name)
{
    CodeCheckingInstance* t_newInstance = new CodeCheckingInstance(p_name, p_buildPath);
    m_runningInstances.push_back(t_newInstance);
    connect(t_newInstance, &CodeCheckingInstance::analyzeFinished, this, &CodeCheckerService::analyzeProcessFinished);
    return t_newInstance;
}

void CodeCheckerService::startServer()
{
    m_serverProcess->start("bash", QStringList() << "-c" << "source " + CodeCheckerUtils::codeCheckerEnv + " && " + CodeCheckerUtils::codeCheckerPath + " server");
}

void CodeCheckerService::analyzeProcessFinished(QString p_name)
{
    CodeCheckingInstance* t_inst = static_cast<CodeCheckingInstance*>(sender());
    m_runningInstances.removeOne(t_inst);
    t_inst->deleteLater();
    qDebug() << "emitting analyzeFinished";
    emit analyzeFinished(p_name);
}
