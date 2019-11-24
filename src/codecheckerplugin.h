#ifndef CODECHECKERPLUGIN_H
#define CODECHECKERPLUGIN_H

#include "codecheckerplugin_global.h"

#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <memory>

#include "codecheckeroptionspage.h"
#include "codecheckeroutputwindow.h"
#include "codecheckerservice.h"

#include "ThriftAPI/codeCheckerDBAccess.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/THttpClient.h>
#include "codecheckerbuildstepfactory.h"

namespace CodeCheckerQtPlugin {
namespace Internal {

class CodeCheckerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "CodeCheckerPlugin.json")

public:
    CodeCheckerPlugin();
    ~CodeCheckerPlugin();

    bool                                        initialize(const QStringList &arguments, QString *errorString);
    void                                        extensionsInitialized();
    ShutdownFlag                                aboutToShutdown();
private:
    std::shared_ptr<CodeCheckerService>         m_codeChecker;
    std::shared_ptr<CodeCheckerOptionsPage>     m_optionsPage;
    CodeCheckerOutputWindow*                    m_outputWindow;
    CodeCheckerBuildStepFactory                 m_buildFactory;
private slots:
    void                                        currentProjectChanged(ProjectExplorer::Project *project);
    void                                        activeTargetChanged(ProjectExplorer::Target *target);
    void                                        activeBuildConfigurationChanged(ProjectExplorer::BuildConfiguration *buildConfiguration);

    void                                        analyzeFinished(QString p_name);
    void                                        analyzePressed();
};

} // namespace Internal
} // namespace CodeCheckerPlugin

#endif // CODECHECKERPLUGIN_H
