message Message {
    // 设置保留项 【废弃字段】
    reserved 100, 101, 200 to 299;
    reserved "field3", "field4";
    // 注意：不要在⼀⾏ reserved 声明中同时声明字段编号和名称。
    // reserved 102, "field5";
    // 设置保留项之后，下⾯代码会告警
    int32 field1 = 100; //告警：Field 'field1' uses reserved number 100
    int32 field2 = 101; //告警：Field 'field2' uses reserved number 101
    int32 field3 = 102; //告警：Field name 'field3' is reserved
    int32 field4 = 103; //告警：Field name 'field4' is reserved
}
















