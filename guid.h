#pragma once


#include "def.h"

/* *****************************************
 *
 *  SGuiD_uiNT32 ΪΨһ���һ�����������̵ı�ʶid�������ڷ����������Ϣ·�ɶ�λ
 *
 *
 *  <sid>Ϊ����������id �������������� ��1��ʼ�����μ� t_servername ���塿
 *  1.�����ӽڵ�����Ľڵ���� ���� DataRecord/LoginGate  ��nid����Ϊ0��
 *  2.���ӽڵ�����Ľڵ���� ����  LobbyCenter/HomeCenter  ��nid����Ϊ0��
 *  3.Ⱥ����������ӽڵ���� ���� LobbyGate/HomeLogic  ��nid�����1��ʼ��
 *
 *  <gid>Ϊsid�µ�Ⱥ�������id����ͬsid����ͬgid����һ��Ⱥ�������
 *
 *  <nid>Ϊgid�µ��ӽڵ�id����ͬsid����ͬgid����nid��ͬ������ͬȺ���е��ӽڵ������
 *
 */
 // ���ġ��ӽڵ�������� �Լ� �������ӽڵ�����Ľڵ�������� sid
#define CENTER_SERVER_CHILD_MAX_SID 80
// ���ӽڵ�����Ľڵ�sid����������涨��
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
    // �Ƿ��Ƿ��������ͼ�Ⱥ�м�ڵ�
    bool isSCenter() const {
        return ID._data.sid > CENTER_SERVER_BASE_SID || getNodeID() == 0;
    }
    uint32 getSGuiD() const {
        return ID.u32Data;
    }
    // ���������� ����ID
    uint32 getServerID() const {
        return ID._data.sid;
    }
    // ��ȺID
    uint32 getGroupID() const {
        return (uint16)ID._data.gid;
    }
    // ��Ⱥ�ڵ�ID
    uint32 getNodeID() const {
        return (uint8)ID._data.nid;
    }

public:
    // ת���� SGuiD_uiNT32
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