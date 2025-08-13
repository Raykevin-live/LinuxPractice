#include <iostream>
#include <jsoncpp/json/json.h>


void Test1(){
    Json::Value root;

    root["x"] = 100;
    root["y"] = 200;
    root["op"] = '+';
    root["desc"] = "this is a + oper";

    Json::FastWriter writer;
    // Json::StyledWriter writer;
    std::string res = writer.write(root);

    std::cout<<res;

    //反序列化
    Json::Value v;
    Json::Reader r;
    r.parse(res, v);
    int x = v["x"].asInt();
    int y = v["y"].asInt();
    char op = v["op"].asInt();
    std::string desc = v["desc"].asString();

    std::cout<<x<<op<<y<<" desc: "<<desc<<std::endl;
}
int main(){
    Test1();

    return 0;
}