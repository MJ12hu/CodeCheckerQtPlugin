#include "outputwidget.h"
#include "ui_outputwidget.h"

#include <iostream>
#include <QDebug>

using namespace boost;
using namespace apache::thrift;

#include <coreplugin/editormanager/editormanager.h>

OutputWidget::OutputWidget(QWidget *parent, QComboBox *p_comboBox) :
    QWidget(parent),
    m_comboBox(p_comboBox),
    ui(new Ui::OutputWidget)
{
    ui->setupUi(this);

    //loadData();
    connect(ui->resultsWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(treeClicked(QTreeWidgetItem*,int)));
    qDebug() << "OPTIONSPAGE CREATED";
    connect(m_comboBox, &QComboBox::currentTextChanged, this, &OutputWidget::itemChanged);
}

OutputWidget::~OutputWidget()
{
    delete ui;
}

void OutputWidget::loadData(QString p_name)
{
    if(m_comboBox->findText(p_name) == -1)
        m_comboBox->addItem(p_name);
    else
    {
        if(m_comboBox->currentText() == p_name)
            itemChanged(p_name);
        else
            m_comboBox->setCurrentText(p_name);
    }
}

void OutputWidget::treeClicked(QTreeWidgetItem *p_item, int p_col)
{
    if(p_col == 1)
    {
        QStringList t_asd = p_item->data(1, Qt::DisplayRole).toString().split(':');
        Core::EditorManager::instance()->openEditorAt(t_asd.first(), t_asd.last().toInt());
        qDebug() << p_item->data(1, Qt::DisplayRole);
    }
}

void OutputWidget::itemChanged(const QString &p_newItem)
{
    if(p_newItem.isEmpty())
        return;
    apache::thrift::stdcxx::shared_ptr<apache::thrift::transport::TTransport> transport(new apache::thrift::transport::THttpClient("localhost", 8001, "/Default/v6.21/CodeCheckerService"));
    apache::thrift::stdcxx::shared_ptr<apache::thrift::protocol::TProtocol> protocol(new apache::thrift::protocol::TJSONProtocol(transport));
    codeCheckerDBAccessClient* t_db = new codeCheckerDBAccessClient(protocol);
    ui->resultsWidget->clear();

    QFont t_font;
    t_font.setUnderline(true);
    RunDataList t_data;
    ReportDataList t_rdata;
    try {
        transport->open();
        if(transport->isOpen())
        {
            std::string t_asd;
            //t_db->getRunData(t_data);

            ReportDetails t_details;


            ReportFilter t_filter;
            t_filter.runName.push_back(p_newItem.toStdString());
            std::vector<int64_t> t_runIDs;
            //t_runIDs.emplace_back(1);
            t_db->getRunResults(t_rdata, t_runIDs, 1000, 0, std::vector<SortMode>(), t_filter, CompareData(), true);
            /*qDebug() << QString::fromStdString(t_data.front().name);
            for(RunData it : t_data)
            {
                std::cout << it.duration << " " << it.name << " " << it.resultCount << " " << it.runCmd << " " << it.runDate << " " << it.runId << std::endl;
            }*/
            for(ReportData it : t_rdata)
            {
                //std::cout << it.checkedFile << " " << it.checkerMsg << " " << it.lastBugPosition.startLine << " " << it.lastBugPosition.startCol << " " << it.reportId << std::endl;
                ReportDetails t_details;

                QTreeWidgetItem* t_item = new QTreeWidgetItem();
                t_item->setData(0, Qt::DisplayRole, QString::fromStdString(it.checkerMsg));
                t_item->setData(1, Qt::DisplayRole, QString(QString::fromStdString(it.checkedFile) + ":" + QString::number(it.line)));
                transport->close();
                transport->open();
                t_item->setTextColor(1, Qt::blue);
                t_item->setFont(1, t_font);
                t_db->getReportDetails(t_details, it.reportId);
                for(BugPathEvent it_bpe : t_details.pathEvents)
                {
                    QTreeWidgetItem* t_innerItem = new QTreeWidgetItem();
                    t_innerItem->setData(0, Qt::DisplayRole, QString::fromStdString(it_bpe.msg));
                    t_innerItem->setData(1, Qt::DisplayRole, QString(QString::fromStdString(it_bpe.filePath) + ":" + QString::number(it_bpe.startLine)));
                    t_innerItem->setTextColor(1, Qt::blue);
                    t_innerItem->setFont(1, t_font);
                    t_item->addChild(t_innerItem);
                }
                //std::cout << t_details.pathEvents.size() << " " << t_details.executionPath.size();
                ui->resultsWidget->addTopLevelItem(t_item);
            }
        }
        else
        {
            //qDebug() << "Not open";
        }
    } catch (apache::thrift::TException& tx) {
        qDebug() << "THRIFT ERROR: " << tx.what();
    }
}
