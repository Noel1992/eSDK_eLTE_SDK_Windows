/********************************************************************
Copyright 2012,TD-Tech. Co., Ltd.
Filename:    uioperation.h
Author:      Wu yu wei
Created:     2012/07/26 16:00
Description:
*********************************************************************/

#ifndef UIOPERATION_H
#define UIOPERATION_H

#include "resourceinfo.h"
#include "operationmanager.h"

class UIOperation
{
public:

    virtual ~UIOperation();
    virtual ACE_INT32 invoke(ResourceInfo * resInfo);
};

class UIOperationMultiPara : public UIOperation
{
public:
    virtual ~UIOperationMultiPara();
    //DC mode operations
    virtual ACE_INT32 invoke(ResourceInfo * resInfo,void * MultiPara);

    //GW mode operations
    virtual ACE_INT32 invokeOp_gw1(const char* from, const char* to,void * gw_para);
    virtual ACE_INT32 invokeOp_gw2(unsigned long cid, void * gw_para);
};

#endif // UIOPERATION_H
