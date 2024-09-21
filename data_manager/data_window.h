#ifndef DATA_WINDOW_H
#define DATA_WINDOW_H

#include <QWidget>
#include <QButtonGroup>
#include <QSqlTableModel>
#include "manager_database.h"
#include "users_manager.h"

namespace Ui {
class DataWindow;
}

class DataWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = nullptr);
    ~DataWindow();

    void initTableWidget();

private:
    void _initDatabase(){
        _db=&ManagerDatabase::instance();
    }

    /// This function is used to modify the data name and number of columns represented
    /// by each column in the tableWidget in the ui.
    /// Author: Juanhua Zhang
    template<typename...Args>
    void _setTableWidgetHeader(QStringList& header,const Args&...args);

    void _onRadioButtonAllClicked();
    void _onRadioButtonOximetryClicked();
    void _onRadioButtonTemperatureClicked();
    void _onRadioButtonAllClickedWhere(const QString& where);
    void _onRadioButtonOximetryClickedWhere(const QString& where);
    void _onRadioButtonTemperatureClickedWhere(const QString& where);

    void _onBtnSearchClicked();

    Ui::DataWindow *ui;
    QSqlTableModel* model;
    ManagerDatabase* _db=nullptr;

    QButtonGroup* _btnGroup;
};

#endif // DATA_WINDOW_H
