#pragma once


#include "def.h"

/* *****************************************
 *
 *  SGuiD_uiNT32 为唯一标记一个服务器进程的标识id，且用于服务器间的消息路由定位
 *
 *
 *  <sid>为服务器种类id 分下面三种类型 【1开始】【参见 t_servername 定义】
 *  1.不带子节点的中心节点进程 比如 DataRecord/LoginGate  【nid必须为0】
 *  2.带子节点的中心节点进程 比如  LobbyCenter/HomeCenter  【nid必须为0】
 *  3.群组服务器的子节点进程 比如 LobbyGate/HomeLogic  【nid必须从1开始】
 *
 *  <gid>为sid下的群组服务器id，相同sid且相同gid代表一个群组服务器
 *
 *  <nid>为gid下的子节点id，相同sid且相同gid并且nid不同代表相同群组中的子节点服务器
 *
 */
 // 最大的【子节点服务器】 以及 【不带子节点的中心节点服务器】 sid
#define CENTER_SERVER_CHILD_MAX_SID 80
// 带子节点的中心节点sid必须大于下面定义
#define CENTER_SERVER_BASE_SID 100
struct SGuiD_uiNT32
{
private:

    struct _Data
    {
        uint8 sid;      // 1~255
        uint8 nid;      // 1~255
        uint16 gid;     // 1~65535
    };

    union
    {
        uint32 u32Data = 0;
        _Data _data;
    }ID;

public:
    SGuiD_uiNT32() {
        ID.u32Data = 0;
    }
    SGuiD_uiNT32(uint32 sguid) {
        ID.u32Data = sguid;
    }
    SGuiD_uiNT32(const uint8& _sid, uint16 _gid, uint8 _nid) {
        ID._data.sid = _sid;
        ID._data.gid = _gid;
        ID._data.nid = _nid;
    }
    SGuiD_uiNT32& operator=(const uint32& _data) {
        ID.u32Data = _data;
        return *this;
    }
    SGuiD_uiNT32& operator=(const SGuiD_uiNT32& _data) {
        if (this == &_data) {
            return *this;
        }
        ID.u32Data = _data.getSGuiD();
        return *this;
    }
    bool operator==(const uint32& _data) const {
        return ID.u32Data == _data;
    }

public:
    void setSGuiD(uint32 sguid) {
        ID.u32Data = sguid;
    }
    void setSid(uint8 sid) {
        ID._data.sid = sid;
    }
    void setGid(uint16 _id) {
        ID._data.gid = _id;
    }
    void setNid(uint8 _nid) {
        ID._data.nid = _nid;
    }

public:
    // 是否是服务器类型集群中间节点
    bool isSCenter() const {
        return ID._data.sid > CENTER_SERVER_BASE_SID || getNodeID() == 0;
    }
    uint32 getSGuiD() const {
        return ID.u32Data;
    }
    // 服务器类型 基础ID
    uint32 getServerID() const {
        return ID._data.sid;
    }
    // 集群ID
    uint32 getGroupID() const {
        return (uint16)ID._data.gid;
    }
    // 集群节点ID
    uint32 getNodeID() const {
        return (uint8)ID._data.nid;
    }

public:
    // 转换到 SGuiD_uiNT32
    static SGuiD_uiNT32 ToSGuiD(const int& guid) {
        SGuiD_uiNT32 stSGuiD(guid);
        return stSGuiD;
    }
    static uint32 GetCenterSGuiD(uint32 sguid) {
        SGuiD_uiNT32 stSGuiD(sguid);
        stSGuiD.setSid(stSGuiD.getServerID() >= CENTER_SERVER_BASE_SID ? (stSGuiD.getServerID()) : (stSGuiD.getServerID() + CENTER_SERVER_BASE_SID));
        stSGuiD.setNid(0);
        return stSGuiD.getSGuiD();
    }
};