#pragma once

#include "message_def.h"


enum ServiceMsgType
{
    ServiceMsgTypeNull      = 0,
    ServiceMsgTypeRegist    = 1,
    ServiceMsgTypeUnRegist  = 2,
    ServiceMsgTypeLogic     = 3,

};


#pragma pack(1)

namespace Cmd {

    namespace Service {

        struct ServiceMsg : public xMsg
        {
            ServiceMsg(uint8 u8MsgType_) :xMsg(MsgTypeService), u8ServiceMsgType(u8MsgType_){}
            uint8   u8ServiceMsgType;
            uint32  u32SrcServiceType;
            uint32  u32SrcgetServiceId;
            uint32  u32TargetServiceType;
            uint32  u32TargetServiceId;
        };

        // 创建服务
        struct RegistServiceMsg : public ServiceMsg
        {
            RegistServiceMsg() :ServiceMsg(ServiceMsgTypeRegist){}
            //
            uint16 u16Len;
            uint8 u8Data[0];
        };

        // 注销服务
        struct UnRegistServiceMsg : public ServiceMsg
        {
            UnRegistServiceMsg() :ServiceMsg(ServiceMsgTypeUnRegist) {}
            //
            uint16 u16Len;
            uint8 u8Data[0];
        };


        // 注销服务
        struct UnRegistServiceMsg : public ServiceMsg
        {
            UnRegistServiceMsg() :ServiceMsg(ServiceMsgTypeUnRegist) {}
            //
            uint16 u16Len;
            uint8 u8Data[0];
        };



        ServiceStateTypeVerify,
            ServiceStateTypeRunning,
            ServiceStateTypeClose,


        // 服务消息
        struct ServiceLogicMsg : public ServiceMsg
        {
            ServiceLogicMsg() :ServiceMsg(ServiceMsgTypeLogic) {}
            //
            uint16 u16Len;
            uint8 u8Data[0];

        };
    }

}


#pragma pack()

