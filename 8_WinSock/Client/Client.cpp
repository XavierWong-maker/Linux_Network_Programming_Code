#include <iostream>
#include <WinSock2.h>

//需在工程设置链接器(linker)输入选项添加ws2_32.lib

using namespace std;

int main(int argc, char* argv[])
{
    WSADATA wd{};

    if (WSAStartup(MAKEWORD(2, 2), &wd)) {
        cout << "startup error\n";
        return -1;
    }

    SOCKET sock{ socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) };

    if (INVALID_SOCKET == sock) {
        cout << "socket error\n";
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);

    if (SOCKET_ERROR == connect(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr))) {
        cout << "connect error\n";
        return -1;
    }

    cout << "connect success sock :" << sock << '\n';

    for (;;) {

        char input[32]{}, buf[128]{};

        cout << "Input: \n";

        cin >> input;

        int len(send(sock, input, strlen(input) + 1, 0));

        int r(recv(sock, buf, sizeof(buf), 0));

        if (r > 0) {
            cout << "Receive :" << buf << '\n';
        }
        else {
            cout << "client quit r :" << r << '\n';
            break;
        }
    }

    closesocket(sock);

    WSACleanup();

    system("pause");

    return 0;
}
