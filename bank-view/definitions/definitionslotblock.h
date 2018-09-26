#ifndef DEFINITIONSLOTBLOCK_H
#define DEFINITIONSLOTBLOCK_H

#include <definitionbasicblocks.h>

//!
//! \brief The SlotDataBlock class
//! 64Words
class SlotDataBlock :
        public AbstractDataBlock
{
public:
    SlotDataBlock(QObject* parent=nullptr) :
        AbstractDataBlock(parent)
    {

    }
    SlotDataBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AbstractDataBlock(anchor,parent)
    {

    }
};
//Q_DECLARE_METATYPE(SlotDataBlock)

namespace SlotBlock {
    Q_NAMESPACE

    enum DataBaseHeaders
    {
    ID=INVALID_INDEX-1,
    //!
        DATA_0=0,
        DATA_1=1,
        DATA_2=2,
        DATA_3=3,
        DATA_4=4,
        DATA_5=5,
        DATA_6=6,
        DATA_7=7,
        DATA_8=8,
        DATA_9=9,
        DATA_10=10,
        DATA_11=11,
        DATA_12=12,
        DATA_13=13,
        DATA_14=14,
        DATA_15=15,
        DATA_16=16,
        DATA_17=17,
        DATA_18=18,
        DATA_19=19,
        DATA_20=20,
        DATA_21=21,
        DATA_22=22,
        DATA_23=23,
        DATA_24=24,
        DATA_25=25,
        DATA_26=26,
        DATA_27=27,
        DATA_28=28,
        DATA_29=29,
        DATA_30=30,
        DATA_31=31,
        DATA_32=32,
        DATA_33=33,
        DATA_34=34,
        DATA_35=35,
        DATA_36=36,
        DATA_37=37,
        DATA_38=38,
        DATA_39=39,
        DATA_40=40,
        DATA_41=41,
        DATA_42=42,
        DATA_43=43,
        DATA_44=44,
        DATA_45=45,
        DATA_46=46,
        DATA_47=47,
        DATA_48=48,
        DATA_49=49,
        DATA_50=50,
        DATA_51=51,
        DATA_52=52,
        DATA_53=53,
        DATA_54=54,
        DATA_55=55,
        DATA_56=56,
        DATA_57=57,
        DATA_58=58,
        DATA_59=59,
        DATA_60=60,
        DATA_61=61,
        DATA_62=62,
        DATA_63=63,
    };
Q_ENUM_NS(DataBaseHeaders)


}


#endif // DEFINITIONSLOTBLOCK_H
