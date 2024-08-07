#pragma once

#include "message_def.h"


enum LogMsgType
{
    LogMsgTypeNull              = 0,
    LogMsgTypeMgr               = 1,
    LogMsgTypeService           = 2,
    LogMsgTypeServiceLogic      = 3, 

};


#pragma pack(1)

namespace Cmd {

    namespace Log {

        struct LogMsg : public xMsg
        {
            LogMsg(uint8 u8MsgType_) :xMsg(MsgTypeService), u8LogMsgType(u8MsgType_){}
            uint8   u8LogMsgType;
        };

        // ��������
        struct LogMgrMsg : public LogMsg
        {
            LogMgrMsg() :LogMsg(LogMsgTypeMgr){}
            //
            uint16 u16Len;
            int8 u8Data[0];
        };

        // ע������
        struct LogServiceMsg : public LogMsg
        {
            LogServiceMsg() :LogMsg(LogMsgTypeService) {}
            //
            uint16 u16Len;
            int8 u8Data[0];
        };

        // ������Ϣ
        struct LogServiceLogicMsg : public LogMsg
        {
            LogServiceLogicMsg() :LogMsg(LogMsgTypeServiceLogic) {}
            //
            uint16 u16Len;
            int8 u8Data[0];

        };
    }

}


#pragma pack()

