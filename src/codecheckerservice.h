#ifndef CODECHECKER_H
#define CODECHECKER_H

#include <QObject>
#include <QProcess>
#include <QFutureInterface>
#include <memory>
#include "codecheckinginstance.h"

class CodeCheckerService : public QObject
{
    Q_OBJECT
public:
    explicit CodeCheckerService(QObject *parent = 0);

    void                            settingsChanged();
    CodeCheckingInstance*           analyze(QString p_buildPath, QString p_name);
private:
    std::shared_ptr<QProcess>       m_serverProcess;
    QVector<CodeCheckingInstance*>  m_runningInstances;

    void                            startServer();
signals:
    void                            analyzeFinished(QString p_name);
public slots:
    void                            analyzeProcessFinished(QString p_name);
};

#endif // CODECHECKER_H
