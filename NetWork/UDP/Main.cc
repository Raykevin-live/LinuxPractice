#include "UdpServer.hpp"
#include <memory>
#include <string>
#include <vector>

void Usage(std::string proc){
    std::cout<<"\n\rUsage: "<<proc<<" pror[1024+]\n"<<std::endl;
}

std::string Handler(const std::string &str, const std::string& clientip, uint16_t clientport){
    std::cout<<"["<<clientip<<":"<<clientport<<"]# "<<str<<std::endl;
    std::string res = "["+ clientip+ ":"+ std::to_string(clientport)+ "]# "+str;
    return res;
}

bool SafeCheck(const std::string &cmd){
    std::vector<std::string> key_word = {
        "rm",
        "mv",
        "kill",
        "sudo",
        "unlink",
        "uninstall",
        "yum",
        "top"
    };

    for(auto & word : key_word){
        auto pos = cmd.find(word);
        if(pos!=std::string::npos) return false;
    }
    return true;
}
std::string ExcuteCommand(const std::string &cmd){
    std::cout<<"get a request: "<<cmd<<std::endl;
    if(!SafeCheck(cmd)) return "Bad man";

    FILE* fp = popen(cmd.c_str(), "r");//popen 直接解析执行
    if(nullptr == fp){
        perror("popen");
        return "error";
    }
    std::string result;
    char buffer[4096];
    while(true){
        char *ok = fgets(buffer, sizeof(buffer), fp);
        if(ok==nullptr) break;
        result += buffer;
    }
    pclose(fp);
    return result;
}
int main(int argc, char* argv[]){
    if(argc != 2){
        Usage(argv[0]);
        exit(0);
    }
    
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->Init();
    svr->Run(Handler);

    return 0;
}