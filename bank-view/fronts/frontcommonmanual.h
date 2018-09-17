#ifndef FRONTCOMMONMANUAL_H
#define FRONTCOMMONMANUAL_H


#include <frontcommon.h>
#include <definitionmanualblock.h>

class FrontCommonManual :
        public FrontCommon
{
    Q_OBJECT
public:
    explicit FrontCommonManual(QWidget* parent=nullptr);

    //! Commons
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

protected:
    //! Commons
    ManualModeDataBlock::Categrories m_categrory;
    int m_index;
};

#endif // FRONTCOMMONMANUAL_H
