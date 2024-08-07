#pragma once

#include "platform_def.h"




struct SessionInfo {
    //
    uint8   u8SessionType;
    uint32  u32SrcServiceType;
    uint32  u32SrcgetServiceId;
    uint32  u32TargetServiceType;
    uint32  u32TargetServiceId;
    //
    uint8   szIp[64];
    uint16  u16Port;
};
























