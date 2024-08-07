#pragma once




#include "platform_def.h"




#include <unordered_map>
#include <functional>
#include <string>
#include <list>
#include <mutex>
#include <shared_mutex>


enum ObjPoolCapInitSize {
    // Service State Pool 
    ObjPoolCapInitSizeServiceStatePool = 1000,
};