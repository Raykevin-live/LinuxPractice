#include "comm.hpp"
#include "log.hpp"
using namespace std;

int main(){
    int fd = open(FIFO_FILE, O_WRONLY);
    Log log;
    if(fd < 0){
        perror("open");
        exit(FIFO_OPEN_ERR);
    }
    cout<<"client open file done\n";
    log(Fatal, "error string: %s, error_code: %d", strerror(errno), errno);

    string line;
    while(true){
        cout<<"Please Enter@ ";
        getline(std::cin, line);
        write(fd, line.c_str(), line.size());
    }

    close(fd);
    return 0;
}
