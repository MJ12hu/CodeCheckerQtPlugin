#include "codecheckeroutputwindow.h"
#include <QDebug>

CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::CodeCheckerOutputWindow()
{
    //setId(Core::Id("CodeChecker"));
    //setDisplayName(tr("CodeChecker Plugin"));
    m_comboBox = new QComboBox();
}

QWidget *CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::outputWidget(QWidget *p_parent)
{
    m_outputWidget = new OutputWidget(p_parent, m_comboBox);
    return m_outputWidget;
}

QList<QWidget *> CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::toolBarWidgets() const
{
    return QList<QWidget*>() << m_comboBox;
}

QString CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::displayName() const
{
    return tr("CodeChecker");
}

int CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::priorityInStatusBar() const
{
    return 6;
}

void CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::clearContents()
{

}

void CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::visibilityChanged(bool visible)
{

}

bool CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::canFocus() const
{
    return true;
}

bool CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::hasFocus() const
{
    return false;
}

void CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::setFocus()
{

}

bool CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::canNext() const
{
    return false;
}

bool CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::canPrevious() const
{
    return false;
}

void CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::goToNext()
{

}

void CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::goToPrev()
{

}

bool CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::canNavigate() const
{
    return false;
}

OutputWidget *CodeCheckerQtPlugin::Internal::CodeCheckerOutputWindow::getOutputWidget()
{
    return m_outputWidget;
}
