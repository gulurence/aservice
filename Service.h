#pragma once

#include "def.h"
#include "error.h"
#include "xSingleton.h"
#include "obj_pool.h"

#include "ServiceDef.h"

#include "message_service.h"

enum ServiceRunType {
    ServiceRunTypeNull = 0,
    ServiceRunTypeRunning = 1,
    ServiceRunTypeSuspend = 2,
    ServiceRunTypeFree = 3,

    ServiceRunningTypeMax = 4,
};

class Service {
public:
    Service(ServiceId& ServiceId_):m_stServiceId(ServiceId_){}
    ~Service(){}

private:
    ServiceId &m_stServiceId;
    ServiceRunType m_stRunState;

public:
    ServiceId &GetServiceId() {
        return m_stServiceId;
    }
    void SetServiceId(const ServiceId &stServiceId) {
        m_stServiceId = stServiceId;
    }

public:
    virtual Service* Create() = 0;
    virtual uint32 Reset() = 0;
};
typedef std::unordered_map<uint64, Service*> ServiceMap;





class Player : public Service {
public:
    Player(ServiceId& ServiceId_):Service(ServiceId_){}
    ~Player(){}

private:
    uint32 m_u32ServiceId;

public:
    virtual Service *Create(){
        ServiceId stServiceId(1,1);
        return new Player(stServiceId);
    }
    virtual uint32 Reset(){
        m_u32ServiceId = 0;
    }
};


class ServiceStatePool
{
public:
    ServiceStatePool(){}
    ~ServiceStatePool(){}

private:
    SafeOneWriteOneReadObjPool<Service*, ObjPoolCapInitSizeServiceStatePool> m_stIn;

public:
    void HeartBeat() {

    }
};

