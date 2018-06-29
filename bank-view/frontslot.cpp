#include "frontslot.h"
#include "ui_frontslot.h"

#include <QDebug>

FrontSlot::FrontSlot(ControllerMaterialTransfer *controller, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSlot),
    __controller(controller)
{
    ui->setupUi(this);
    //!
    ui->lcdNumberSlot->display(__controller->Index());
    //! Prepare dialog

        __dialog = new QDialog(this);
//        __dialog->setGeometry(__dialog->geometry().x(),
//                              __dialog->geometry().y(),
//                              600,
//                              480);
        __dialog->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);

        __view = new QTableView(__dialog);
        __view->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Expanding);
        __view->verticalHeader()->setVisible(false);
        __view->setModel(__controller->Table());
        //! Fetch headers
        QSqlTableModel* __reference = new QSqlTableModel(this,__controller->DataBase());
        __reference->setTable(QString("MAT_HEADER_SLOT%1").arg(__controller->Index()));
        bool result = __reference->select();
        qDebug() << QString("%1:%2").arg(__reference->tableName()).arg(result);

        HEADER_STRUCTURE::HeaderRender::renderViewHeader(__reference,__view);
        __reference->deleteLater();
        //!
        connect(ui->toolButtonDialog,&QToolButton::clicked,this,&FrontSlot::onDataRaise);
        connect(__controller,&ControllerMaterialTransfer::dataUpdated,this,&FrontSlot::onDataUpdated);
        connect(ui->pushButtonMaterialOverrideOff,&QPushButton::clicked,this,&FrontSlot::onMaterialOverrideOff);
}

FrontSlot::~FrontSlot()
{
    delete ui;
}

void FrontSlot::onDataUpdated()
{
    utilities::colorChangeOver(ui->labelIsValid,
                               __controller->IsValid());
    ui->lcdNumberID->display(__controller->MaterialId());

    //! enable button once valid
    ui->pushButtonMaterialOverrideOff->setEnabled(__controller->IsValid());
}

void FrontSlot::onDataRaise()
{
    //! Show current datas
    QSqlTableModel* __table = qobject_cast<QSqlTableModel*>(__view->model());
    __table->setFilter(utilities::generateFilterString(QVariant::fromValue(HEADER_STRUCTURE::ID),
                                                       QVariant::fromValue(__controller->MaterialId())));
    __table->select();
    __dialog->exec();
}

void FrontSlot::onMaterialOverrideOff()
{
    __controller->MaterialOverride(false);
    ui->pushButtonMaterialOverrideOff->setEnabled(false);
}
