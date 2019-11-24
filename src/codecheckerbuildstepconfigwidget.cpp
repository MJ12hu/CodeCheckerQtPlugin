#include "codecheckerbuildstepconfigwidget.h"
#include <QString>

CodeCheckerBuildStepConfigWidget::CodeCheckerBuildStepConfigWidget(ProjectExplorer::BuildStep *step)
    : ProjectExplorer::BuildStepConfigWidget (step)
{
    setDisplayName("Codechecker Analyze");
    setSummaryText("Analyzing");
}
