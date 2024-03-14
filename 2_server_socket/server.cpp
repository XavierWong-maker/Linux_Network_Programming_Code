#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int server {socket(PF_INET, SOCK_STREAM, 0)};

    if(-1 == server){
        cout << "server socket error" << endl;
        return -1;
    }

    sockaddr_in saddr {};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl函数把小端转换成大端（网络字节序采用大端）
    saddr.sin_port = htons(8899);

    if(-1 == bind(server, reinterpret_cast <sockaddr*> (&saddr), sizeof(saddr))){
        cout << "server error" << endl;
        return -1;
    }

    if(-1 == listen(server,1)){
        cout << "server listen error" << endl;
        return -1;
    }

    cout << "server start success" << endl;

    sockaddr_in caddr {};
    socklen_t asize {sizeof(caddr)};
    int client {accept(server,reinterpret_cast<sockaddr*> (&caddr), &asize)};

    if(-1 == client){
        cout << "client accept error\n";
       return -1;
    }

    cout << "client: " << client << endl;//client的数值表示系统资源的id

    int len {0};
    do
    {
        char buf[32]{};
        int r (recv(client, buf, sizeof(buf)/sizeof(*buf), 0));

        if(r > 0){
            len += r;
            for (int i = 0; i < r; i++)
            {
                cout << buf[i];
            }
        }
        
    } while (len < 64);

    cout << "\nrecv len = " << len << endl;
    constexpr char temp_str[] {"hello word!\n"};
    send(client, temp_str, sizeof(temp_str), 0);

    sleep(1);

    close(client);
    close(server);

    return 0;
}
