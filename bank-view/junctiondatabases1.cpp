#include "junctiondatabases1.h"

#include <tablemodelcommandblock.h>
#include <tablemodelaxis.h>
#include <tablemodelcylinder.h>
#include <tablemodeliooverride.h>
#include <tablemodelsignals.h>
#include <tablemodelunits.h>
#include <tablemodelmatslotn.h>

JunctionDataBaseS1::JunctionDataBaseS1(QString databaseName, QObject *parent) :
    JunctionBankDatabase(databaseName,parent)
{

}

void JunctionDataBaseS1::decorateTable()
{
    __tableMap[WHOLE_AXIS] = new TableModelAxis(__tableMap[WHOLE_AXIS]);
    __tableMap[WHOLE_COMMAND_BLOCKS] = new TableModelCommandBlock(__tableMap[WHOLE_COMMAND_BLOCKS]);
    __tableMap[WHOLE_CYLINDERS] = new TableModelCylinder(__tableMap[WHOLE_CYLINDERS]);
    __tableMap[WHOLE_SIGNALS] = new TableModelSignals(__tableMap[WHOLE_SIGNALS]);
    __tableMap[WHOLE_UNITS] = new TableModelUnits(__tableMap[WHOLE_UNITS]);

    __tableMap[WHOLE_MAT_DATA_SLOTN] = new TableModelMatSlotN(__tableMap[WHOLE_MAT_DATA_SLOTN]);

    __tableMap[INPUT_ATTRIBUTES] = new TableModelIOOverride(__tableMap[INPUT_ATTRIBUTES]);
    __tableMap[OUTPUT_ATTRIBUTES] = new TableModelIOOverride(__tableMap[OUTPUT_ATTRIBUTES]);

}
