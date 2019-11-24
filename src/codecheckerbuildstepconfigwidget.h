#ifndef CODECHECKERPREBUILDSTEPCONFIGWIDGET_H
#define CODECHECKERPREBUILDSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

class CodeCheckerBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
public:
    explicit CodeCheckerBuildStepConfigWidget(ProjectExplorer::BuildStep *step);
};

#endif // CODECHECKERPREBUILDSTEPCONFIGWIDGET_H
