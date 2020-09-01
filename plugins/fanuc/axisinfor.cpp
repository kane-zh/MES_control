#include "axisinfor.h"


axisInfor::axisInfor()
{

}

QString axisInfor::getValue(unsigned short handle, DATASET dataSetInfor)
{
    return  dataSetInfor.name;
}

QString axisInfor::cnc_actf(unsigned short handle, DATASET dataSetInfor)
{

}
