#include "frontslot.h"
#include "ui_frontslot.h"

#include <QDebug>

FrontSlot::FrontSlot(ControllerMaterialTransfer *controller,bool isShowCounters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSlot),
    __controller(controller)
{
    ui->setupUi(this);
    //!
//    ui->lcdNumberSlot->display(__controller->Index());
    ui->frameInspection->setVisible(isShowCounters);
    //! Prepare dialog

        __dialog = new QDialog(this);
        __dialog->setGeometry(__dialog->geometry().x(),
                              __dialog->geometry().y() + 120,
                              1024,
                              240);
        __dialog->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
        auto __layout = new QFormLayout(__dialog);
        __layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        __dialog->setLayout(__layout);

        __view = new QTableView(__dialog);
        __view->setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::Expanding);
        __view->verticalHeader()->setVisible(false);
        __view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        __view->setModel(__controller->Table());
        emit

        __layout->addWidget(__view);
        //! Fetch headers
        QSqlTableModel* __reference = new QSqlTableModel(this,__controller->DataBase());
        __reference->setTable(QString("MAT_HEADER_SLOT%1").arg(__controller->Index()));
        bool result = __reference->select();
        qDebug() << QString("%1:%2").arg(__reference->tableName()).arg(result);
        __reference->deleteLater();
        HEADER_STRUCTURE::HeaderRender::renderViewHeader(__reference,__view);
        //! Set title
        __reference = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION);
        __reference->setTable("DEF_REGION");
        __reference->setFilter(QString("ID=%1").arg(__controller->Index()));
        __reference->select();
        ui->labelName->setText(__reference->record(0).value("zh_Tw").toString());
        __reference->deleteLater();
        //!
        connect(ui->toolButtonDialog,&QToolButton::clicked,this,&FrontSlot::onDataRaise);
        connect(__controller,&ControllerMaterialTransfer::dataUpdated,this,&FrontSlot::onDataUpdated);
        connect(ui->pushButtonMaterialOverrideOff,&QPushButton::clicked,this,&FrontSlot::onMaterialOverrideOff);
        connect(ui->pushButtonClear,&QPushButton::clicked,this,&FrontSlot::onClear);


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

    //!
    ui->lcdNumberTotalCounter->display(__controller->TotalCount());
    ui->lcdNumberOKCounter->display(__controller->OKCount());
    ui->lcdNumberNGCounter->display(__controller->NGCount());
    ui->lcdNumberOKRate->display(__controller->OKRate());
    ui->lcdNumberNGRate->display(__controller->NGRate());
    ui->labelOKNG->setText(QVariant::fromValue(__controller->CurrentGrade()).toString());
    utilities::colorChangeOver(ui->labelOKNG,
                              __controller->CurrentGrade()==ControllerMaterialTransfer::OK ||
                               __controller->CurrentGrade()==ControllerMaterialTransfer::BYPASS,
                              Qt::green,
                              Qt::red);
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
void FrontSlot::onClear()
{
    __controller->CounterClear();
}
