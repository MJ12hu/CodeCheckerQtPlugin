#ifndef CODECHECKEROUTPUTWINDOW_H
#define CODECHECKEROUTPUTWINDOW_H

#include <coreplugin/ioutputpane.h>
#include "outputwidget.h"

namespace CodeCheckerQtPlugin {
namespace Internal {

class CodeCheckerOutputWindow : public Core::IOutputPane
{
public:
    CodeCheckerOutputWindow();

    QWidget *outputWidget(QWidget *p_parent);

    QList<QWidget *>    toolBarWidgets() const override;
    QString             displayName() const override;
    int                 priorityInStatusBar() const override;
    void                clearContents() override;
    void                visibilityChanged(bool visible) override;
    bool                canFocus() const override;
    bool                hasFocus() const override;
    void                setFocus() override;

    bool                canNext() const override;
    bool                canPrevious() const override;
    void                goToNext() override;
    void                goToPrev() override;
    bool                canNavigate() const override;

    OutputWidget*       getOutputWidget();

private:
    OutputWidget*       m_outputWidget;
    QComboBox*          m_comboBox;
};
}
}

#endif // CODECHECKEROUTPUTWINDOW_H
