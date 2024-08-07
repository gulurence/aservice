#pragma once


#include "Session.h"
#include "xSingleton.h"

#include "obj_pool.h"

/*

1.不同线程不同管理器
2.

*/



class SessionMgr :public xSingleton <SessionMgr> {
public:
    SessionMgr(){}
    ~SessionMgr(){}

 public:
    //void OnHeartBeat(Session* pSession);
    //void OnMsgLogic(Session* pSession);

private:
    //std::unordered_map<Session*,int> m_mapSession;
    //SafeOneWriteOneReadObjPool<Session*, 500> m_pSession[SessionStateMax];
};
