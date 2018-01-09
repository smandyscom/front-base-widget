#include "blockmanual.h"
#include <QVariant>
BlockManual::BlockManual(QObject *parent) : QObject(parent)
{

}

QList<AddressValueBinding> BlockManual::getRegisterBinding()
{
    QList<AddressValueBinding> result;
    result << AddressValueBinding(ENGAGED_HMI,QVariant::fromValue<BIT>(false))
           << AddressValueBinding(DONE,QVariant::fromValue<BIT>(false))
           //<< ModbusBinding(AXIS_ADR,QVariant::fromValue<WORD>(0))
           //<< ModbusBinding(POS_COMMAND
           //<< ModbusBinding(POS_FEEDBACK
           //<< ModbusBinding(SPD_FEEDBACK
           //<< ModbusBinding(TOR_FEEDBACK
           << AddressValueBinding(ENGAGED_PLC,QVariant::fromValue<BIT>(false))
           << AddressValueBinding(RUN,QVariant::fromValue<BIT>(false))
           << AddressValueBinding(CANCEL,QVariant::fromValue<BIT>(false)
           << AddressValueBinding(MODE,QVariant::fromValue<WORD>(0))
           << AddressValueBinding(COMMIT_CONTENT,QVariant::fromValue<WORD>(0))
           << AddressValueBinding(COMMIT_INDEX,QVariant::fromValue<WORD>(0));

}
