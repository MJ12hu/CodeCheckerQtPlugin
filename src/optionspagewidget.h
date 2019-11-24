#ifndef OPTIONSPAGEWIDGET_H
#define OPTIONSPAGEWIDGET_H

#include <QWidget>

namespace Ui {
class OptionsPageWidget;
}

class OptionsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsPageWidget(QString p_path, QString p_env, bool p_checked);
    ~OptionsPageWidget();

    QString                 getCodeCheckerPath();
    QString                 getCodeCheckerEnvironment();
    bool                    autoAnalyzeChecked();
private:
    Ui::OptionsPageWidget*  ui;
};

#endif // OPTIONSPAGEWIDGET_H
