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
	m_widgetsPolish.append(ui->labelIsValid);
	m_widgetsPolish.append(ui->labelOKNG);
}

FrontSlot::~FrontSlot()
{
    delete ui;
}

void FrontSlot::Setup(QSqlTableModel* slot,
	QSqlTableModel* header)
{
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
	//! TODO , load title
}

void FrontSlot::dynamicPropertyChanged(int key, QVariant value)
{
	switch (key)
	{
	case SlotDataBlock::BIT1_ACT:
		break;
	case SlotDataBlock::BIT2_VALID:
		//! use QSS to control
		break;
	case SlotDataBlock::MATERIAL_ID:
		ui->labelID->setText(QString::number(value.toUInt()));
		//! Simple static
		if (m_lastId != value.toUInt())
		{
			m_totalCounter += 1;

			//query database to get whether OK/NG
			QSqlRecord record = utilities::getSqlTableSelectedRecord(m_model,
				QVariant::fromValue(HEADER_STRUCTURE::ID),
				value);
			SlotBlock::Grade grade = 
				record.value(QVariant::fromValue(SlotBlock::DATA_0).toString().toStdString().c_str())
				.value<SlotBlock::Grade>();

			switch (grade)
			{
			case SlotBlock::OK:
			case SlotBlock::BYPASS:
				m_okCounter += 1;
				break;
			case SlotBlock::NG:
				m_ngCounter += 1;
				break;
			default:
				break;
			}

			//rate
			if (m_totalCounter != 0) {
				ui->lcdNumberOKRate->display(m_okCounter / m_totalCounter);
				ui->lcdNumberNGRate->display(m_ngCounter / m_totalCounter);
			}
			else
			{
				ui->lcdNumberOKRate->display(0);
				ui->lcdNumberNGRate->display(0);
			}
		}
		m_lastId = value.toUInt();
		break;
	default:
		break;
	}
	ui->pushButtonMaterialOverrideOff->setEnabled(property(QVariant::fromValue(SlotDataBlock::BIT2_VALID)).toBool());
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
