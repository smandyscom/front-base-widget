#ifndef ENUMCOMBOBOXDYNAMIC_H
#define ENUMCOMBOBOXDYNAMIC_H

#include <QWidget>
#include <QComboBox>
#include <QMetaEnum>

typedef QVariant (*CastingFunction)(int i);
//typedef castingFunction CastingFunction;

class EnumComboBoxDynamic : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ readValue WRITE setValue USER true)
public:
    QVariant readValue() const;
    void setValue(QVariant value);
    explicit EnumComboBoxDynamic(QMetaEnum target,CastingFunction cast,QWidget *parent = nullptr);

signals:

public slots:

protected:
    QVariant __seed; // stored the enum object
};

#endif // ENUMCOMBOBOXDYNAMIC_H
