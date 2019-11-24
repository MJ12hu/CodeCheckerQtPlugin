DEFINES += CODECHECKERPLUGIN_LIBRARY

# CodeCheckerPlugin files

SOURCES += \
    ThriftAPI/codeCheckerDBAccess.cpp \
    ThriftAPI/codeCheckerDBAccess_server.skeleton.cpp \
    ThriftAPI/report_server_constants.cpp \
    ThriftAPI/report_server_types.cpp \
    ThriftAPI/shared_constants.cpp \
    ThriftAPI/shared_types.cpp \
    codecheckerbuildstep.cpp \
    codecheckerbuildstepconfigwidget.cpp \
    codecheckerbuildstepfactory.cpp \
    codecheckeroptionspage.cpp \
    codecheckerservice.cpp \
    codecheckerutils.cpp \
    codecheckinginstance.cpp \
    optionspagewidget.cpp \
    codecheckerplugin.cpp \
    codecheckeroutputwindow.cpp \
    outputwidget.cpp

HEADERS += \
    ThriftAPI/codeCheckerDBAccess.h \
    ThriftAPI/report_server_constants.h \
    ThriftAPI/report_server_types.h \
    ThriftAPI/shared_constants.h \
    ThriftAPI/shared_types.h \
    codecheckerbuildstep.h \
    codecheckerbuildstepconfigwidget.h \
    codecheckerbuildstepfactory.h \
        codecheckerplugin_global.h \
        codecheckerpluginconstants.h \
    codecheckeroptionspage.h \
    codecheckerservice.h \
    codecheckerutils.h \
    codecheckinginstance.h \
    optionspagewidget.h \
    codecheckerplugin.h \
    codecheckeroutputwindow.h \
    outputwidget.h

CONFIG += c++11

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
#isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/mj12hu/qt/qt-creator
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=../../qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
#isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/mj12hu/qt/qt-creator/build
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=../../qt-creator/build

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = CodeCheckerPlugin
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    debugger \
    projectexplorer \
    qmakeprojectmanager

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

FORMS += \
    optionspagewidget.ui \
    outputwidget.ui

unix:!macx: LIBS += /usr/local/lib/libthrift.a

INCLUDEPATH += /usr/local/include/thrift
DEPENDPATH += /usr/local/include/thrift

unix:!macx: PRE_TARGETDEPS += /usr/local/lib/libthrift.a
