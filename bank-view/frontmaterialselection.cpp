#include "frontmaterialselection.h"
#include "ui_frontmaterialselection.h"
#include <QDebug>

frontMaterialSelection::frontMaterialSelection(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frontMaterialSelection)
{
    ui->setupUi(this);
    //! Prepair models
    //! Todo , made utilities to open table , output message to qDebug()
    QSqlTableModel* __tableKey = new QSqlTableModel(this,db);
    __tableKey->setTable(QVariant::fromValue(VIEW_MAT_TABLE_KEY).toString());
    __tableKey->select();
    QSqlTableModel* __tableHousing = new QSqlTableModel(this,db);
    __tableHousing->setTable(QVariant::fromValue(VIEW_MAT_TABLE_HOUSING).toString());
    __tableHousing->select();

    __tableMap[FIELD_L_KEY] = __tableKey;
    __tableMap[FIELD_R_KEY] = __tableKey;
    __tableMap[FIELD_L_HOUSING] = __tableHousing;
    __tableMap[FIELD_R_HOUSING] = __tableHousing;
    __tableMap[FIELD_WORKING_NUMBER] = nullptr;

    //! Pairs
    __fieldsMap[ui->comboBoxLKey] = FIELD_L_KEY;
    __fieldsMap[ui->comboBoxLHousing] =FIELD_L_HOUSING;
    __fieldsMap[ui->comboBoxRKey] =FIELD_R_KEY;
    __fieldsMap[ui->comboBoxRHousing] =FIELD_R_HOUSING;
    __fieldsMap[ui->doubleSpinBoxWorkingNumber] = FIELD_WORKING_NUMBER;

    foreach (QWidget* var, __fieldsMap.keys()) {
        QComboBox* __cb = qobject_cast<QComboBox*>(var);
        if(__cb != nullptr)
        {
            utilities::linkQComboBoxAndModel(__cb,__tableMap[__fieldsMap[var]],"ID");
            connect(__cb,SIGNAL(activated(int)),this,SLOT(onComboBoxActivated(int)));

            __cb->setCurrentIndex(0);
        }
        else
        {
            QDoubleSpinBox* __dsb = qobject_cast<QDoubleSpinBox*>(var);
            connect(__dsb,SIGNAL(editingFinished()),this,SLOT(onWorkingNumberInputed()));
        }
    }
}

frontMaterialSelection::~frontMaterialSelection()
{
    delete ui;
}

void frontMaterialSelection::onComboBoxActivated(int index)
{
    QComboBox* __cb = qobject_cast<QComboBox*>(sender());
    QSqlTableModel* __table = qobject_cast<QSqlTableModel*>(__cb->model());

    QSqlRecord __record = __table->record(__cb->currentIndex());
    int __polarization = __record.value(QVariant::fromValue(Polarization).toString()).toInt();

    emit fieldValueChanged(static_cast<int>(__fieldsMap[__cb]),__polarization);
}

void frontMaterialSelection::onWorkingNumberInputed()
{
    QDoubleSpinBox* __dsb = qobject_cast<QDoubleSpinBox*>(sender());
    int __workingNumber = static_cast<int>(ui->doubleSpinBoxWorkingNumber->value());
    emit fieldValueChanged(static_cast<int>(__fieldsMap[__dsb]),__workingNumber);
}

//!
//! \brief frontMaterialSelection::onInitialized
//! raise comboBox activated event
void frontMaterialSelection::onInitialized()
{
    foreach (QWidget* var, __fieldsMap.keys()) {
        QComboBox* __cb = qobject_cast<QComboBox*>(var);
        if(__cb != nullptr)
            emit __cb->activated(__cb->currentIndex());
    }
}
