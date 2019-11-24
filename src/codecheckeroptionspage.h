#ifndef CODECHECKEROPTIONSPAGE_H
#define CODECHECKEROPTIONSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include "optionspagewidget.h"

namespace CodeCheckerQtPlugin {
namespace Internal {
class CodeCheckerOptionsPage : public Core::IOptionsPage
{
    Q_OBJECT
public:
    explicit CodeCheckerOptionsPage(QObject *parent = 0);

    QWidget*            widget();
    void                apply(void);
    void                finish();
private:
    OptionsPageWidget*  m_widget;
};
}
}

#endif // CODECHECKEROPTIONSPAGE_H
