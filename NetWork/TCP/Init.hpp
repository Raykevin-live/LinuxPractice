#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "Log.hpp"

const std::string dictname = "./dict.txt";

static bool Split(std::string &s, std::string *part1, std::string *part2, const std::string sep = ":"){
    auto pos = s.find(sep);
    if(pos==std::string::npos) return false;
    *part1 = s.substr(0, pos);
    *part2 = s.substr(pos+1);
    return true;
}
class Init{
public:
    Init(){
        std::fstream in(dictname);
        if(!in.is_open()){
            lg(Fatal, "ifsream open %s error", dictname.c_str());
            exit(1);
        }

        std::string line;
        while(std::getline(in, line)){
            std::string part1, part2;
            Split(line, &part1, &part2);
            dict.insert({part1, part2});
        }

        in.close();
    }

    std::string translation(const std::string &key){
        auto iter = dict.find(key);
        if(iter == dict.end()){
            return "Unknow";
        }
        return iter->second;
    }
private:
    std::unordered_map<std::string, std::string> dict;
};

Init init;