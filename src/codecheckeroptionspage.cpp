#include "codecheckeroptionspage.h"
#include <coreplugin/icore.h>
#include <QDebug>
#include "codecheckerutils.h"

CodeCheckerQtPlugin::Internal::CodeCheckerOptionsPage::CodeCheckerOptionsPage(QObject *parent)
    : Core::IOptionsPage(parent)
{
    setId(Core::Id("CodeChecker.OptionsPage"));
    setDisplayName(tr("CodeChecker Plugin"));
    setCategory("T.Analyzer");
    setDisplayCategory(tr("Analyzer"));


    // Create a new category for the options page. Here we create a totally
    // new category. In that case we also provide an icon. If we choose in
    // 'setCategory' an already existing category, the options page is added
    // the chosen category and an additional tab. No icon is set in this case.
    /*setCategory(Constants::MYOPTIONSPAGE_CATEGORY);
   setDisplayCategory(QLatin1String(
      Constants::MYOPTIONSPAGE_CATEGORY_TR_CATEGORY));
   setCategoryIcon(
      QLatin1String(Constants::MYOPTIONSPAGE_CATEGORY_CATEGORY_ICON));*/


}

QWidget *CodeCheckerQtPlugin::Internal::CodeCheckerOptionsPage::widget()
{
    m_widget = new OptionsPageWidget(CodeCheckerUtils::codeCheckerPath,
                                     CodeCheckerUtils::codeCheckerEnv,
                                     CodeCheckerUtils::needAutoAnalyze);
    return m_widget;
}

void CodeCheckerQtPlugin::Internal::CodeCheckerOptionsPage::apply()
{
    if(m_widget)
    {
        CodeCheckerUtils::codeCheckerPath = m_widget->getCodeCheckerPath();
        CodeCheckerUtils::codeCheckerEnv = m_widget->getCodeCheckerEnvironment();
        CodeCheckerUtils::needAutoAnalyze = m_widget->autoAnalyzeChecked();
        Core::ICore::instance()->settings()->setValue("codeCheckerPath", CodeCheckerUtils::codeCheckerPath);
        Core::ICore::instance()->settings()->setValue("codeCheckerEnv", CodeCheckerUtils::codeCheckerEnv);
        Core::ICore::instance()->settings()->setValue("codeCheckerAutoAnalyze", CodeCheckerUtils::needAutoAnalyze);
        emit settingsChanged();
    }
}

void CodeCheckerQtPlugin::Internal::CodeCheckerOptionsPage::finish()
{
    m_widget = nullptr;
}
