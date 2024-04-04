#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int sock = 0;
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(-1 == sock){
        cout << "Socket error" << endl;
        return -1;
    }

    const char* ipAddrStr = "127.0.0.1";
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    
    if(!inet_pton(AF_INET, ipAddrStr, &addr.sin_addr)){
        cout << "Invalid address" << endl;
        return -1;
    }

    addr.sin_port = htons(8888);

    if(-1 == connect(sock, reinterpret_cast<sockaddr*> (&addr), sizeof(addr))){
        cout << "connect error" << endl;
        return -1;
    }

    cout << "connect success sock :" << sock << endl;

    for(;;){

        char input[32]{},buf[128]{};
        
        cout << "Input: \n";

        cin >> input;

        int len (send(sock, input, strlen(input) + 1, 0));

        int r (recv(sock, buf, sizeof(buf), 0));

        if(r > 0){
            // cout << "Receive :" << buf << endl;
        }else{ /* 与服务端断开连接 recv(...) 返回 0 */
            cout << "client quit r :" << r << '\n';
            break;
        }
    }

    close(sock);

    return 0;
}
