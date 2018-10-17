#include "frontslot.h"
#include "ui_frontslot.h"

#include <QDebug>

FrontSlot::FrontSlot(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontSlot)
{
    ui->setupUi(this);
    //!
//    ui->lcdNumberSlot->display(__controller->Index());
    //ui->frameInspection->setVisible(isShowCounters);
    //! Prepare dialog

       
        //! Set title
       /* __reference = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION);
        __reference->setTable("DEF_REGION");
        __reference->setFilter(QString("ID=%1").arg(__controller->Index()));
        __reference->select();
        ui->labelName->setText(__reference->record(0).value("zh_Tw").toString());
        __reference->deleteLater();*/
        //!
	/*	for each (QPushButton* var in findChildren<QPushButton*>())
		{
			connect(var, QPushButton::click, this, &FrontSlot::onButtonClicked);
			m_widgetsPolish.append(var);
		}*/
}

FrontSlot::~FrontSlot()
{
    delete ui;
}

void FrontSlot::Setup(QSqlTableModel* slot,
	QSqlTableModel* header)
{
	connect(slot, &QSqlTableModel::dataChanged, this, &FrontSlot::onDataChanged);
	//Initialize detail view
	m_dialog = new QDialog(this);
	m_dialog->setGeometry(m_dialog->geometry().x(),
		m_dialog->geometry().y() + 120,
		1024,
		240);
	m_dialog->setSizePolicy(QSizePolicy::Expanding,
		QSizePolicy::Expanding);
	auto layout = new QFormLayout(m_dialog);
	layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	m_dialog->setLayout(layout);

	m_view = new QTableView(m_dialog);
	m_view->setSizePolicy(QSizePolicy::Expanding,
		QSizePolicy::Expanding);
	m_view->verticalHeader()->setVisible(false);
	m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_view->setModel(slot);

	layout->addWidget(m_view);
	//! Fetch headers
	HEADER_STRUCTURE::HeaderRender::renderViewHeader(header, m_view);
}

void FrontSlot::dynamicPropertyChanged(int key, QVariant value)
{
	switch (key)
	{
	case SlotDataBlock::BIT1_ACT:
	case SlotDataBlock::BIT1_DONE:
		break;
	default:
		break;
	}
	ui->pushButtonMaterialOverrideOff->setEnabled(property(QVariant::fromValue(SlotDataBlock::BIT2_VALID)).toBool());
}

//! Show last record
void FrontSlot::onDataChanged(const QModelIndex &topLeft,
	const QModelIndex &bottomRight,
	const QVector<int> &roles)
{
	auto model = qobject_cast<QSqlTableModel*>(sender());

	QSqlRecord record = utilities::getSqlTableSelectedRecord(model,
		QVariant::fromValue(HEADER_STRUCTURE::ID),
		topLeft.data());

	m_currentId = record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toUInt();
	//!
	if (m_currentId != m_lastId)
		m_totalCounter += 1;
	//!
	update();
}

void FrontSlot::onButtonClicked()
{
	if (sender() == ui->pushButtonMaterialOverrideOff)
	{
		m_controller->setProperty(QVariant::fromValue(SlotDataBlock::BIT2_VALID).toString().toStdString().c_str(),
			false);
	}
	else if (sender() == ui->toolButtonDialog)
	{
		//! Show current data , model had been selected when dataChanged
		m_dialog->exec();
	}
	else if (sender() == ui->pushButtonClear)
	{
		m_totalCounter = 0;
		m_okCounter = 0;
		m_ngCounter = 0;

		update();
	}
}

void FrontSlot::update()
{
	//!
	/*ui->lcdNumberID->display(m_currentId);
	ui->lcdNumberTotalCounter->display(m_totalCounter);
	ui->lcdNumberOKCounter->display(m_okCounter);
	ui->lcdNumberNGCounter->display(m_ngCounter);
	if (m_totalCounter != 0) {
		ui->lcdNumberOKRate->display(m_okCounter / m_totalCounter);
		ui->lcdNumberNGRate->display(m_ngCounter / m_totalCounter);
	}
	else
	{
		ui->lcdNumberOKRate->display(0);
		ui->lcdNumberNGRate->display(0);
	}*/
	//ui->labelOKNG->setText(QVariant::fromValue(__controller->CurrentGrade()).toString());
}