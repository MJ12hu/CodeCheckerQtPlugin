#include "codecheckerutils.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/kitinformation.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <projectexplorer/kit.h>
#include <cpptools/cppmodelmanager.h>

CodeCheckerUtils::CodeCheckerUtils()
{

}

CodeCheckerService* CodeCheckerUtils::codeCheckerInstance = 0;
QString CodeCheckerUtils::codeCheckerEnv = "";
QString CodeCheckerUtils::codeCheckerPath = "";
bool CodeCheckerUtils::needAutoAnalyze = true;

void CodeCheckerUtils::generateCompilationDB(ProjectExplorer::Project *project)
{
    CppTools::ProjectInfo projectInfo = CppTools::CppModelManager::instance()->projectInfo(project);
    const ::Utils::FilePath buildDir = buildDirectory(*projectInfo.project());

    QDir dir(buildDir.toString());
    if (!dir.exists())
        dir.mkpath(dir.path());
    QFile compileCommandsFile(buildDir.toString() + "/codechecker_compiledb.json");
    const bool fileOpened = compileCommandsFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (!fileOpened) {
        /*return GenerateCompilationDbResult(QString(),
                QCoreApplication::translate("ClangUtils", "Could not create \"%1\": %2")
                    .arg(compileCommandsFile.fileName(), compileCommandsFile.errorString()));*/
    }
    compileCommandsFile.write("[");

    for (CppTools::ProjectPart::Ptr projectPart : projectInfo.projectParts()) {
        const QStringList args = projectPartArguments(*projectPart);
        for (const CppTools::ProjectFile &projFile : projectPart->files) {
            const QJsonObject json = createFileObject(buildDir, args, *projectPart, projFile);
            if (compileCommandsFile.size() > 1)
                compileCommandsFile.write(",");
            compileCommandsFile.write('\n' + QJsonDocument(json).toJson().trimmed());
        }
    }

    compileCommandsFile.write("\n]");
    compileCommandsFile.close();
    //return GenerateCompilationDbResult(compileCommandsFile.fileName(), QString());
}

QStringList CodeCheckerUtils::projectPartArguments(const CppTools::ProjectPart &projectPart)
{
    QStringList args;
    args << compilerPath(projectPart).toString();
    args << "-c";
    if (projectPart.toolchainType != ProjectExplorer::Constants::MSVC_TOOLCHAIN_TYPEID) {
        args << "--target=" + projectPart.toolChainTargetTriple;
        args << (projectPart.toolChainWordWidth == CppTools::ProjectPart::WordWidth64Bit
                 ? QLatin1String("-m64")
                 : QLatin1String("-m32"));
    }
    args << projectPart.compilerFlags;
    for (const ProjectExplorer::HeaderPath &headerPath : projectPart.headerPaths) {
        if (headerPath.type == ProjectExplorer::HeaderPathType::User) {
            args << "-I" + headerPath.path;
        } else if (headerPath.type == ProjectExplorer::HeaderPathType::System) {
            args << (projectPart.toolchainType == ProjectExplorer::Constants::MSVC_TOOLCHAIN_TYPEID
                     ? "-I"
                     : "-isystem")
                    + headerPath.path;
        }
    }
    for (const ProjectExplorer::Macro &macro : projectPart.projectMacros) {
        args.append(QString::fromUtf8(
                        macro.toKeyValue(macro.type == ProjectExplorer::MacroType::Define ? "-D" : "-U")));
    }

    return args;
}

Utils::FilePath CodeCheckerUtils::compilerPath(const CppTools::ProjectPart &projectPart)
{
    ProjectExplorer::Target *target = projectPart.project->activeTarget();
    if (!target)
        return ::Utils::FilePath();

    ProjectExplorer::ToolChain *toolchain = ProjectExplorer::ToolChainKitAspect::toolChain(
                target->kit(), ProjectExplorer::Constants::CXX_LANGUAGE_ID);

    return toolchain->compilerCommand();
}

Utils::FilePath CodeCheckerUtils::buildDirectory(const ProjectExplorer::Project &project)
{
    ProjectExplorer::Target *target = project.activeTarget();
    if (!target)
        return ::Utils::FilePath();

    ProjectExplorer::BuildConfiguration *buildConfig = target->activeBuildConfiguration();
    if (!buildConfig)
        return ::Utils::FilePath();

    return buildConfig->buildDirectory();
}

QJsonObject CodeCheckerUtils::createFileObject(const Utils::FilePath &buildDir, const QStringList &arguments, const CppTools::ProjectPart &projectPart, const CppTools::ProjectFile &projFile)
{
    QJsonObject fileObject;
    fileObject["file"] = projFile.path;
    QStringList args = arguments;

    const CppTools::ProjectFile::Kind kind = CppTools::ProjectFile::classify(projFile.path);
    if (projectPart.toolchainType == ProjectExplorer::Constants::MSVC_TOOLCHAIN_TYPEID
            || projectPart.toolchainType == ProjectExplorer::Constants::CLANG_CL_TOOLCHAIN_TYPEID) {
        if (CppTools::ProjectFile::isC(kind))
            args.append("/TC");
        else if (CppTools::ProjectFile::isCxx(kind))
            args.append("/TP");
    } else {
        QStringList langOption
                = createLanguageOptionGcc(kind,
                                          projectPart.languageExtensions
                                          & ::Utils::LanguageExtension::ObjectiveC);
        for (const QString &langOptionPart : langOption)
            args.append(langOptionPart);
    }
    args.append(QDir::toNativeSeparators(projFile.path));
    fileObject["command"] = args.join(" ");
    fileObject["directory"] = buildDir.toString();
    return fileObject;
}
