#pragma once


#include "platform_def.h"


/*

1.·�ɲ�
2.Э���

*/

enum MsgType {
    MsgTypeNull     = 0,
    MsgTypeManager  = 1,
    MsgTypeService  = 2,
    MsgTypeLog      = 3,

};



#pragma pack(1)

namespace Cmd {

    struct xMsg
    {
        xMsg(uint8 u8MsgType_):u8MsgType(u8MsgType_){}
        uint8 u8MsgType;
    };

}


#pragma pack()
