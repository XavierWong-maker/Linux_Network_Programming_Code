#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int sock = 0;
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(-1 == sock){
        cout << "sock error" << endl;
        return -1;
    }

    const char* ipAddrStr = "47.99.217.175";
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    
    /*inet_addr() 将有效字符串转换为32位二进制网络字节序的IPV4地址*/
    //addr.sin_addr.s_addr = inet_addr("47.99.217.175");

    if(!inet_aton(ipAddrStr,&addr.sin_addr)){
        cout << "changer error" << endl;
        return -1;
    }
    /*htons() 把本机字节序转换成网络字节序*/
    addr.sin_port = htons(80);

    if(-1 == connect(sock, reinterpret_cast<sockaddr*> (&addr), sizeof(addr))){
        cout << "connect error" << endl;
        return -1;
    }

    cout << "connect success" << endl;
    close(sock);

    return 0;
}
