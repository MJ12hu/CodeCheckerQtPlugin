#ifndef CODECHECKERUTILS_H
#define CODECHECKERUTILS_H

#include <cpptools/projectpart.h>
#include <cpptools/projectinfo.h>
#include <cpptools/compileroptionsbuilder.h>
#include <clangcodemodel/clangutils.h>
#include <projectexplorer/project.h>

class CodeCheckerService;

class CodeCheckerUtils
{
public:
    CodeCheckerUtils();
    static void generateCompilationDB(ProjectExplorer::Project *project);
    static QStringList projectPartArguments(const CppTools::ProjectPart &projectPart);
    static ::Utils::FilePath compilerPath(const CppTools::ProjectPart &projectPart);
    static ::Utils::FilePath buildDirectory(const ProjectExplorer::Project &project);
    static QJsonObject createFileObject(const ::Utils::FilePath &buildDir,
                                        const QStringList &arguments,
                                        const CppTools::ProjectPart &projectPart,
                                        const CppTools::ProjectFile &projFile);

    static CodeCheckerService* codeCheckerInstance;
    static QString codeCheckerPath;
    static QString codeCheckerEnv;
    static bool needAutoAnalyze;
};

#endif // CODECHECKERUTILS_H
