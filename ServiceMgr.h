#pragma once



#include "Service.h"




class ServiceMgr : public xSingleton<ServiceMgr>
{
public:
    ServiceMgr() {}
    ~ServiceMgr() {}

private:
    // 总索引
    ServiceMap m_mapServiceIndex;
    // 不同状态索引
    ServiceStatePool m_stStatePool[ServiceStateTypeMax];

public:
    Service* GetService(uint32 u32State, uint64 u32ServiceId) {
        //std::shared_lock<std::shared_mutex> lock(m_lock);
        //auto it = m_mapActiveService.find(u32Type);
        //if (it == m_mapActiveService.end()) {
        //    return nullptr;
        //}
        //else {
        //    auto it_t = it->second->find(u32ServiceId);
        //    if (it_t == it->second->end()) {
        //        return nullptr;
        //    }
        //    //
        //    auto *pService = it_t->second;
        //    return pService;
        //}
    }

    void OnLogic(Cmd::Service::ServiceMsg *pMsg) {
        switch (pMsg->u8MsgType) {
        case ServiceMsgTypeRegist: {

            }break;
        case ServiceMsgTypeUnRegist: {

            }break;
        case ServiceMsgTypeLogic: {

            }break;
        defualt:
            break;
        }
    }
};




class ServiceRunMgr {
public:
    ServiceRunMgr(){}
    ~ServiceRunMgr() {}

private:
    SafeOneWriteOneReadObjPool<Service*, ObjPoolCapInitSizeServiceStatePool>* m_pRunning;
    SafeOneWriteOneReadObjPool<Service*, ObjPoolCapInitSizeServiceStatePool>* m_pSuspend;
    SafeOneWriteOneReadObjPool<Service*, ObjPoolCapInitSizeServiceStatePool>* m_pFree;

public:




};










