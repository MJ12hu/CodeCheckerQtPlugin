#include "optionspagewidget.h"
#include "ui_optionspagewidget.h"

OptionsPageWidget::OptionsPageWidget(QString p_path, QString p_env, bool p_checked) :
    QWidget(),
    ui(new Ui::OptionsPageWidget)
{
    ui->setupUi(this);
    ui->le_path->setText(p_path);
    ui->le_env->setText(p_env);
    ui->checkBox->setChecked(p_checked);
}

OptionsPageWidget::~OptionsPageWidget()
{
    delete ui;
}

QString OptionsPageWidget::getCodeCheckerPath()
{
    return ui->le_path->text();
}

QString OptionsPageWidget::getCodeCheckerEnvironment()
{
    return ui->le_env->text();
}

bool OptionsPageWidget::autoAnalyzeChecked()
{
    return ui->checkBox->isChecked();
}
