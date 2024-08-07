#include "Service.h"
#include "corutin.h"




//
//
//SegmentedTask<int> ProcessCmd() {
//    for (int i{ 0 }; i < 10; ++i) {
//        std::cout << "ProcessCmd thread ID: " << std::this_thread::get_id() << '\n';
//        std::cout << "Processing step " << i << std::endl;
//        int a = test_run();
//        co_yield std::nullopt;
//        printf("%d \n", a);
//    }
//    co_return 0;
//}
//
//void ProcessMsg()
//{
//    std::cout << "ProcessMsg thread ID: " << std::this_thread::get_id() << '\n';
//    std::cout << "Processing Message" << std::endl;
//}
//
//void ProcessAlm()
//{
//    std::cout << "ProcessAlm thread ID: " << std::this_thread::get_id() << '\n';
//    std::cout << "Processing Alarm" << std::endl;
//}
//
//
//void ServiceMgr::OnLogic() {
//
//    for (auto event : g_events) {
//        switch (event) {
//        case EventType::COMMAND:
//            suspended = ProcessCmd();
//            break;
//        case EventType::MESSAGE:
//            ProcessMsg();
//            break;
//        case EventType::ALARM:
//            ProcessAlm();
//            break;
//        }
//    }
//    while (suspended.has_value() && !suspended->IsFinished()) {
//        suspended->Resume();
//    }
//    if (suspended.has_value()) {
//        std::cout << "Final return: " << suspended->GetReturnValue() << std::endl;
//    }
//    return 0;
//}
//
//
