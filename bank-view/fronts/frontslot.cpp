#include "frontslot.h"
#include "ui_frontslot.h"

#include <QDebug>

FrontSlot::FrontSlot(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontSlot),
	m_lastId(0),
	m_totalCounter(0),
	m_okCounter(0),
	m_ngCounter(0)
{
    ui->setupUi(this);
	for each (QPushButton* var in findChildren<QPushButton*>())
	{
		connect(var, &QPushButton::clicked, this, &FrontSlot::onButtonClicked);
	}
	connect(ui->toolButtonDialog, &QToolButton::clicked, this, &FrontSlot::onButtonClicked);
	//!
	m_widgetsPolish.append(ui->labelIsValid);
	m_widgetsPolish.append(ui->labelOKNG);
}

FrontSlot::~FrontSlot()
{
    delete ui;
}

void FrontSlot::Setup(QSqlTableModel* slot,
	QSqlTableModel* header,
	QString title)
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
	m_model = slot;
	//!load title
	ui->labelName->setText(title);
}

void FrontSlot::dynamicPropertyChanged(int key, QVariant value)
{
	switch (key)
	{
	case SlotDataBlock::BIT2_VALID:
		//! use QSS to control
		break;
	case SlotDataBlock::ROLE_QUERY:
	case SlotDataBlock::ROLE_QUERY_PASSIVE:
	case SlotDataBlock::ROLE_UPDATE_BLOCK:
		
		//! Simple static
		if (m_lastId != value.toUInt())
		{
			m_totalCounter += 1;

			//query database to get whether OK/NG
			QSqlRecord record = utilities::getSqlTableSelectedRecord(m_model,
				QVariant::fromValue(HEADER_STRUCTURE::ID),
				value);
			SlotBlock::Grade grade =
				record.value(QVariant::fromValue(SlotBlock::GRADE).toString().toStdString().c_str())
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
			ui->labelOKNG->setText(QVariant::fromValue(grade).toString());
			setProperty(QVariant::fromValue(SlotBlock::GRADE).toString().toStdString().c_str(),QVariant::fromValue(grade));

			//!
			ui->lcdNumberTotalCounter->display(m_totalCounter);
			ui->lcdNumberNGCounter->display(m_ngCounter);
			ui->lcdNumberOKCounter->display(m_okCounter);
			//rate
			if (m_totalCounter != 0) {
				ui->lcdNumberOKRate->display(((double)m_okCounter / (double)m_totalCounter) * 100);
				ui->lcdNumberNGRate->display(((double)m_ngCounter / (double)m_totalCounter) * 100);
			}
			else
			{
				ui->lcdNumberOKRate->display(0);
				ui->lcdNumberNGRate->display(0);
			}
		}
		m_lastId = value.toUInt();
	case SlotDataBlock::ROLE_CREATE:
		ui->lcdNumberID->display(QString::number(value.toUInt()));
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
		m_port->setProperty(QVariant::fromValue(SlotDataBlock::BIT2_VALID).toString().toStdString().c_str(),
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

	}
}
