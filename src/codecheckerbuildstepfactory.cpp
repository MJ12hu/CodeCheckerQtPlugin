#include "codecheckerbuildstepfactory.h"
#include "codecheckerbuildstep.h"
#include "projectexplorer/buildsteplist.h"
#include "projectexplorer/buildstep.h"
#include "projectexplorer/projectexplorerconstants.h"

CodeCheckerBuildStepFactory::CodeCheckerBuildStepFactory() :
    ProjectExplorer::BuildStepFactory()
{
    registerStep<CodeCheckerBuildStep>(Core::Id("CodeCheckerBuildStep"));

    //setSupportedConfiguration("Qt4ProjectManager.Qt4BuildConfiguration");
    setSupportedStepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    setDisplayName("CodeCheckerBuildStep");
    //setFlags(ProjectExplorer::BuildStepInfo::UniqueStep);
}

