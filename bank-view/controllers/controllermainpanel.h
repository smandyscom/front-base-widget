#ifndef CONTROLLERMAINPANEL_H
#define CONTROLLERMAINPANEL_H

#include <QObject>
#include <QVariant>
#include <QMetaEnum>

#include <definitionsbaselayer.h>
#include <definitionunitblocks.h>
#include <definitionauxiliarykeys.h>
#include <definitionmaincontrolblock.h>

#include <controllerbase.h>

//!
//! \brief The ControllerMainPanel class
//! Used to fetch out Pause/Error/Error codes
//!  Control Pause/Error Reset
//!  TODOS
//!  need data base to translate error messages
using namespace HEADER_STRUCTURE;

class ControllerMainPanel :
        public ControllerBase
{
    Q_OBJECT
public:
    explicit ControllerMainPanel(quint8 clientId,quint16 baseOffset,int interval,QObject *parent = nullptr);
protected:
    //void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERMAINPANEL_H
