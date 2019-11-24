#include "codecheckerbuildstep.h"
#include "codecheckerbuildstepconfigwidget.h"
#include "projectexplorer/processparameters.h"
#include "projectexplorer/buildconfiguration.h"

#include "codecheckerutils.h"
#include "codecheckerservice.h"

CodeCheckerBuildStep::CodeCheckerBuildStep(ProjectExplorer::BuildStepList *bsl)
    : BuildStep(bsl, Core::Id("CodeCheckerBuildStep"))
{
    setDisplayName("CodeChecker");
}

bool CodeCheckerBuildStep::init()
{
    return true;
}

ProjectExplorer::BuildStepConfigWidget *CodeCheckerBuildStep::createConfigWidget()
{
    return new CodeCheckerBuildStepConfigWidget(this);
}

QVariant CodeCheckerBuildStep::data(Core::Id id) const
{
    return QVariant();
}

void CodeCheckerBuildStep::doRun()
{
    if(project() && CodeCheckerUtils::needAutoAnalyze)
    {
        CodeCheckerUtils::generateCompilationDB(project());
        CodeCheckingInstance* t_inst = CodeCheckerUtils::codeCheckerInstance->analyze(buildConfiguration()->rawBuildDirectory().toString(), project()->displayName());
        connect(t_inst, &CodeCheckingInstance::analyzeFinished, this, &CodeCheckerBuildStep::analyzeFinished);
    }
    else
    {
        emit finished(true);
    }
}

void CodeCheckerBuildStep::doCancel()
{
    emit finished(false);
}

void CodeCheckerBuildStep::analyzeFinished()
{
    emit finished(true);
}
