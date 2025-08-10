#pragma once

#include <iostream>
#include <string>

const std::string blank_space_sep = " ";
const std::string protocol_sep = "\n";

inline std::string Encode(const std::string &content){
    // 封装报头
    std::string package = std::to_string(content.size());
    package += protocol_sep;
    package += content;
    package += protocol_sep;
    return package;
}

inline bool Decode(const std::string &package, std::string *content){
    std::size_t pos = package.find(protocol_sep);
    if(pos == std::string::npos) return false;
    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str);
    // "len'\n'x op y '\n'"
    // package = len_str + content_str + 2('\n)
    std::size_t total_len = len_str.size() + len + 2;
    if(package.size() < total_len) return false;
    *content = package.substr(pos+1, len);

    return true;
}
class Request{
public:
    Request(int data1, int data2, char oper):x(data1), y(data2), op(oper){}
    bool Serialize(std::string *out){
        // struct -> string
        // 构建报文有效载荷
        std::string s = std::to_string(x);
        s += blank_space_sep;
        s += op;
        s += blank_space_sep;
        s += std::to_string(y);
        
        *out = s;
        return true;
    }
    Request(){}
    bool Deserialize(const std::string &in){ // x + y 
        std::size_t left = in.find(blank_space_sep);
        if(left == std::string::npos) return false;
        std::string part_x = in.substr(0, left);

        std::size_t right = in.rfind(blank_space_sep);
        if(right == std::string::npos) return false;
        std::string part_y = in.substr(right+1);

        if(left + 2 != right) return false;
        op = in[left+1];
        x = std::stoi(part_x);
        y = std::stoi(part_y);
        return true;
    }
public:
    // x op y
    int x;
    int y;
    char op;
};

class Response{
public:
    Response(int res, int c):result(res), code(c){}
    bool Serialize(std::string *out){
        std::string s = std::to_string(result);
        s += blank_space_sep;
        s += std::to_string(code);

        *out = s;
        return true;
    }
    Response(){}
    
    bool Deserialize(const std::string &in){ //"result code"
        std::size_t left = in.find(blank_space_sep);
        if(left == std::string::npos) return false;
        std::string part_left = in.substr(0, left);
        std::string part_right = in.substr(left+1);
        
        result = std::stoi(part_left);
        code = std::stoi(part_right);
        return true;
    }

public:
    int result;
    int code;// 0:可信，!0表示不可信，具体是几表示对应的错误原因
};
