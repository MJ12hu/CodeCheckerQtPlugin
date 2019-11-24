#include "codecheckerplugin.h"
#include "codecheckerpluginconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <extensionsystem/pluginmanager.h>
#include <debugger/debuggercore.h>
#include <debugger/debuggeritem.h>
#include <debugger/debuggeritemmanager.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/target.h>
#include <projectexplorer/kit.h>

#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/projectexplorerconstants.h>

#include "codecheckerutils.h"


#include <projectexplorer/kitinformation.h>
#include <cpptools/cppmodelmanager.h>
#include <QJsonDocument>
#include <QJsonArray>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "codecheckerbuildstep.h"

using namespace CodeCheckerQtPlugin::Internal;

CodeCheckerPlugin::CodeCheckerPlugin()
{
    // Create your members
}

CodeCheckerPlugin::~CodeCheckerPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool CodeCheckerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_outputWindow = new CodeCheckerOutputWindow();
    ExtensionSystem::PluginManager::addObject(m_outputWindow);

    //connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished, this, [this](bool p_succ) {qDebug() << "Build finished" << p_succ; buildFinished(p_succ);});
    //connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildStateChanged, this, [this](ProjectExplorer::Project *pro) {qDebug() << "--------buildStateChanged" << pro->displayName();});

    connect(ProjectExplorer::ProjectTree::instance(), &ProjectExplorer::ProjectTree::currentProjectChanged, this, &CodeCheckerPlugin::currentProjectChanged);

    Core::ActionContainer *mproject =
            Core::ActionManager::actionContainer(ProjectExplorer::Constants::M_PROJECTCONTEXT);

    const Core::Context projectContext("Qt4ProjectManager.Qt4Project");
    QAction *analyzeaction = new QAction(tr("CodeChecker analysis"), this);
    const Core::Context globalcontext(Core::Constants::C_GLOBAL);
    Core::Command *command = Core::ActionManager::registerAction(analyzeaction, Constants::ANALYZE_ID, projectContext);
    command->setAttribute(Core::Command::CA_Hide);
    mproject->addAction(command, ProjectExplorer::Constants::G_PROJECT_BUILD);
    connect(analyzeaction, &QAction::triggered, this, &CodeCheckerPlugin::analyzePressed);

    m_optionsPage = std::make_shared<CodeCheckerOptionsPage>();

    CodeCheckerUtils::codeCheckerPath = Core::ICore::instance()->settings()->value("codeCheckerPath").toString();
    CodeCheckerUtils::codeCheckerEnv = Core::ICore::instance()->settings()->value("codeCheckerEnv").toString();
    CodeCheckerUtils::needAutoAnalyze = Core::ICore::instance()->settings()->value("codeCheckerAutoAnalyze", true).toBool();
    m_codeChecker = std::make_shared<CodeCheckerService>();
    connect(m_optionsPage.get(), &CodeCheckerOptionsPage::settingsChanged, m_codeChecker.get(), &CodeCheckerService::settingsChanged);
    m_codeChecker->settingsChanged();

    CodeCheckerUtils::codeCheckerInstance = m_codeChecker.get();

    connect(m_codeChecker.get(), &CodeCheckerService::analyzeFinished, this, &CodeCheckerPlugin::analyzeFinished);

    return true;
}

void CodeCheckerPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag CodeCheckerPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)

    ExtensionSystem::PluginManager::removeObject(m_outputWindow);
    delete m_outputWindow;
    return SynchronousShutdown;
}

void CodeCheckerPlugin::currentProjectChanged(ProjectExplorer::Project *project)
{
    disconnect(this, SLOT(activeTargetChanged(ProjectExplorer::Target*)));
    if(!project)
        return;
    connect(project, &ProjectExplorer::Project::activeTargetChanged, this, &CodeCheckerPlugin::activeTargetChanged);
    activeTargetChanged(project->activeTarget());
}

void CodeCheckerPlugin::activeTargetChanged(ProjectExplorer::Target *target)
{
    disconnect(this, SLOT(activeBuildConfigurationChanged(ProjectExplorer::BuildConfiguration*)));
    if(!target)
        return;
    connect(target, &ProjectExplorer::Target::activeBuildConfigurationChanged, this, &CodeCheckerPlugin::activeBuildConfigurationChanged);
    activeBuildConfigurationChanged(target->activeBuildConfiguration());
}

void CodeCheckerPlugin::activeBuildConfigurationChanged(ProjectExplorer::BuildConfiguration *buildConfiguration)
{
    //disconnect(this, SLOT(buildOutput(QString,ProjectExplorer::BuildStep::OutputFormat,ProjectExplorer::BuildStep::OutputNewlineSetting)));
    if(!buildConfiguration)
        return;
    ProjectExplorer::BuildStepList* t_list = buildConfiguration->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));

    if(t_list == nullptr)
        return;

    if(!t_list->contains("CodeCheckerBuildStep"))
        t_list->insertStep(t_list->count(), "CodeCheckerBuildStep");

    //accessing build output
    /*for(int i = 0; i < t_list->count(); i++)
    {
        if(t_list->at(i)->displayName().toLower() == "make")
            connect(t_list->at(i), &ProjectExplorer::BuildStep::addOutput, this, &CodeCheckerPlugin::buildOutput);
    }*/
}

void CodeCheckerPlugin::analyzeFinished(QString p_name)
{
    if(m_outputWindow->getOutputWidget())
        m_outputWindow->getOutputWidget()->loadData(p_name);
}

void CodeCheckerPlugin::analyzePressed()
{
    if(ProjectExplorer::ProjectTree::currentProject() && ProjectExplorer::ProjectTree::currentProject()->activeTarget() && ProjectExplorer::ProjectTree::currentProject()->activeTarget()->activeBuildConfiguration())
    {
        CodeCheckerUtils::generateCompilationDB(ProjectExplorer::ProjectTree::currentProject());
        //connect(t_inst, &CodeCheckingInstance::analyzeFinished, this, &CodeCheckerBuildStep::analyzeFinished);
        CodeCheckingInstance* t_inst = CodeCheckerUtils::codeCheckerInstance->analyze(ProjectExplorer::ProjectTree::currentProject()->activeTarget()->activeBuildConfiguration()->rawBuildDirectory().toString(), ProjectExplorer::ProjectTree::currentProject()->displayName());
    }
    //
}
