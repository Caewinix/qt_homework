#include <QDateTime>
#include <QMessageBox>
#include "data_window.h"
#include "ui_data_window.h"
#include <QDebug>

DataWindow::DataWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataWindow)
{

    _initDatabase();

    ui->setupUi(this);


    model = new QSqlTableModel(this,(QSqlDatabase&)(*_db));
    model->setTable(UsersManager::current());
    model->select();
    ui->tableView->setModel(model);

    _btnGroup=new QButtonGroup(this);

    connect(ui->btnSearch,&QPushButton::clicked,this,&DataWindow::_onBtnSearchClicked);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::initTableWidget()
{

    model = new QSqlTableModel(this,(QSqlDatabase&)(*_db));
    const QString& username=UsersManager::current();
    qDebug()<<UsersManager::current();
    if(_db->tables().contains(username)){
        model->setTable(username);
        model->select();
    }
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    QHeaderView* headerView = ui->tableView->horizontalHeader();
    headerView->setMinimumHeight(60);
    headerView->setMaximumHeight(100);

//    _setTableWidgetHeader(header,"Oximetry","Temperature","Time");
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Divide cells equally and spread them all over

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Set table not editable
    ui->tableView->setSelectionBehavior ( QAbstractItemView::SelectRows); // Set selected row
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Set selected line
}

void DataWindow::_onBtnSearchClicked(){
    // Press the "Search" button
    const QString& where=ui->lineEditSearchCondition->text().trimmed(); // Get search condition 获取搜索条件
    model->setTable(UsersManager::current());
    model->setFilter(where);
    model->select();
}
