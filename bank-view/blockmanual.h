#ifndef BLOCKMANUAL_H
#define BLOCKMANUAL_H

#include <QObject>
#include <baselayerdefinitions.h>
//link to

using namespace BaseLayer;

struct ManualContext
{
    WORD wordIn;//PLC<-
    WORD axisAdr;
    LONG posCommand;
    LONG posFeedback;
    LONG spdFeedback;
    LONG torFeedback;
    WORD wordOut;//PLC->
    WORD mode;
    WORD commitContent;
    WORD commitIndex;
};


//addressing mode,  including bit access
enum ManualAddress : quint64
{
    ENGAGED_HMI	=	0,
    DONE	=	0,
    AXIS_ADR	=	1,
    POS_COMMAND	=	2,
    POS_FEEDBACK	=	4,
    SPD_FEEDBACK	=	6,
    TOR_FEEDBACK	=	8,
    ENGAGED_PLC	=	16,
    RUN	=	16,
    CANCEL	=	16,
    MODE	=	17,
    COMMIT_CONTENT	=	18,
    COMMIT_INDEX	=	19,
    BLOCK_HEAD	=	32,
};



class BlockManual : public QObject
{
    Q_OBJECT
public:
    explicit BlockManual(QObject *parent = nullptr);

    static QList<AddressValueBinding> getRegisterBinding(); //type infor embedded

signals:

public slots:
};

#endif // BLOCKMANUAL_H
