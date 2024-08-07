#pragma once


#include "platform_def.h"


struct ServiceId {
    ServiceId(uint8 u8Type_, uint64 u56Id_){
        u8Type = u8Type_;
        u56Id = u56Id_;
    }
    uint32 u8Type : 8;
    uint32 u56Id : 56;
};