#pragma once


#include "message_def.h"

enum ManagerMsgType
{
    ManagerMsgTypeNull      = 0,
    ManagerMsgTypeHeartBeat = 1,


};

#pragma pack(1)

namespace Cmd {

    namespace Manager {

        struct ManagerMsg : public xMsg
        {
            ManagerMsg(unsigned char u8MsgType_) :xMsg(MsgTypeManager), u8MgrMsgType(u8MsgType_) {}
            uint8   u8MgrMsgType;
            uint32  u32SrcProcessId;
            uint32  u32TargetProcessId;
        };

        struct HeartBeatMsg : public ManagerMsg
        {
            HeartBeatMsg() :ManagerMsg(ManagerMsgTypeHeartBeat) {}
            uint64   u64Tick;
        };
    }

}


#pragma pack()

