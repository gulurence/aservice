#pragma once

#include "message_def.h"

enum RouteMsgType
{
    RouteMsgTypeNull        = 0,
    RouteMsgTypeRequest     = 1,
    RouteMsgTypeResponse    = 2,
    RouteMsgTypePost        = 3,
};

#pragma pack(1)

namespace Cmd {

    struct RouteMsg  {
        uint64  u8Tag;
        uint32  u32SrcProcessId;
        uint32  u32TargetProcessId;
        uint16  u16Len;
        int8    i8Data[0];
    };

}


#pragma pack()
