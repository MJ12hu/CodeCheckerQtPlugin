#ifndef CODECHECKERPREBUILDSTEP_H
#define CODECHECKERPREBUILDSTEP_H

#include <QObject>
#include <projectexplorer/buildstep.h>


class CodeCheckerBuildStep : public ProjectExplorer::BuildStep
{
    Q_OBJECT
public:
    CodeCheckerBuildStep(ProjectExplorer::BuildStepList *bsl);

    bool                                        init();
    ProjectExplorer::BuildStepConfigWidget*     createConfigWidget();
    QVariant                                    data(Core::Id id) const;
private:
    virtual void                                doRun();
    virtual void                                doCancel();
private slots:
    void                                        analyzeFinished();
};

#endif // CODECHECKERPREBUILDSTEP_H
