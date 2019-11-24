#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>
#include "ThriftAPI/codeCheckerDBAccess.h"
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/THttpClient.h>
#include <QTreeWidgetItem>
#include <QComboBox>

namespace Ui {
class OutputWidget;
}

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget *parent, QComboBox* p_comboBox);
    ~OutputWidget();

    void                loadData(QString p_name);
private:
    Ui::OutputWidget*   ui;
    QComboBox*          m_comboBox;
private slots:
    void                treeClicked(QTreeWidgetItem* p_item, int p_col);
    void                itemChanged(const QString &p_newItem);
};

#endif // OUTPUTWIDGET_H
