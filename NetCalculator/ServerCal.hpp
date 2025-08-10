#pragma once
#include "Protocol.hpp"

class ServerCal{
public:
    ServerCal(){

    }
    Response CalculatorHelper(const Request& req){
        Response resp(0, 0);
        switch (req.op)
        {
        case '+':
            resp.result = req.x + req.y;
            break;
        default:
            break;
        }
        return resp;
    }
    std::string Calculator(std::string &package){
        std::string content;
        bool r = Decode(package, &content);
        if( !r) return;
        Request req;
        r = req.Deserialize(content);
        if(!r) return;

        content.clear();
        Response resp = CalculatorHelper(req);
        resp.Serialize(&content);
        content = Encode(content);

        return content;
    }
    ~ServerCal(){}
};