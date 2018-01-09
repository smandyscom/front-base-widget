#include "blockmanualcontroller.h"

BlockManualController::BlockManualController(QObject *parent) : QObject(parent)
{

}

void BlockManualController::updateProcessing(int clusterId)
{
    //check all address which defined in ManualBlock

    //once in its cluster , prepare event then send
}
