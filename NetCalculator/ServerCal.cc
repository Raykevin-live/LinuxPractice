#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "ServerCal.hpp"

// int main(){
//     Request req(123, 456, '+');
//     std::string s;
//     req.Serialize(&s);

//     s = Encode(s);
//     std::cout<<s;

//     std::string content;
//     bool r = Decode(s, &content);
//     std::cout<<content<<std::endl;
//     Request temp;
//     temp.Deserialize(content);
//     std::cout<<"x: "<<temp.x << " op: "<<temp.op<<" y: "<<temp.y<<std::endl;
//     return 0;
// }

int main(){
    ServerCal cal;
    TcpServer* tsvp = new TcpServer(8080, std::bind(&ServerCal::Calculator, &cal, std::placeholders::_1));

    return 0;
}