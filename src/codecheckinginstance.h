#ifndef CODECHECKINGINSTANCE_H
#define CODECHECKINGINSTANCE_H

#include <memory>
#include <QProcess>
#include <QFutureInterface>

class CodeCheckingInstance : public QObject
{
    Q_OBJECT
public:
    CodeCheckingInstance(QString p_name, QString p_buildPath);
    ~CodeCheckingInstance();
private:
    std::shared_ptr<QProcess>       m_analyzeProcess;
    QFutureInterface<void>*         m_buildProgress;
    QString                         m_name;
private slots:
    void                            analyzeProcessStateChanged();
signals:
    void                            analyzeFinished(QString p_name);
};

#endif // CODECHECKINGINSTANCE_H
