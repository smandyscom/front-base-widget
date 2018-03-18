#include "utilities.h"


utilities::utilities()
{

}

KeyValueCollection utilities::model2KeyValuePairs(const QAbstractItemModel &model)
{
    KeyValueCollection output;

    //takes first column as key , second as value
    for(int i=0;i<model.rowCount();i++)
        output.append(KeyValuePair(model.data(model.index(i,0)),
                                   model.data(model.index(i,1))));

    return output;
}

int utilities::sizeOf(QVariant value)
{
    return QMetaType::sizeOf(QMetaType::type(value.typeName()));
}

void utilities::colorChangeOver(QWidget *target, bool value, Qt::GlobalColor trueColor)
{
    QPalette pal = target->palette();

    target->setAutoFillBackground(true);
    if(value)
        pal.setColor(QPalette::Button,QColor(trueColor));
    else
        pal.setColor(QPalette::Button,QColor(Qt::gray));

    target->setPalette(pal);
    target->update();
}
