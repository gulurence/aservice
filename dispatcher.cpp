#include "def.h"

#include "ServerDef.pb.h"







class MsgDispatcher {
public:
    MsgDispatcher() {}
    ~MsgDispatcher() {}

private:

public:
    // ��һ�����
    int OnRouteMsg(uint8 *pData,uint16 u16Len){

        return 0;
    }

public:
    // �ڶ������
    int OnMgrMsg(uint8* pData, uint16 u16Len) {

        return 0;
    }
    int OnServiceMsg(uint8* pData, uint16 u16Len) {

        return 0;
    }
    int OnLogMsg(uint8* pData, uint16 u16Len) {

        return 0;
    }
};






//int main() {
//
//    google::protobuf::Message* pMsg;
//
//
//    //std::thread tw(thread_write);
//    //std::thread tr(thread_read);
//
//    //tw.detach();
//    //tr.detach();
//
//    //// д�����ݿ�
//    //const char data[] = "Hello, world!";
//    //buffer.write(data, strlen(data) + 1); // +1 ���� null ��β��
//
//    //// ��ȡ���ݿ�
//    //std::vector<char> outputData;
//    //if (buffer.read(outputData)) {
//    //    std::cout << "Read data: " << outputData.data() << std::endl;
//    //}
//    //else {
//    //    std::cout << "Failed to read data." << std::endl;
//    //}
//
//    getchar();
//
//    return 0;
//}

