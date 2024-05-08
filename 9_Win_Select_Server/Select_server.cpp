#include <iostream>
#include <WinSock2.h>

//需在工程设置链接器(linker)输入选项添加ws2_32.lib

using namespace std;

SOCKET server_handler(const SOCKET server)
{
    sockaddr_in addr{};
    int asize{ sizeof(addr) };
    return accept(server, reinterpret_cast<sockaddr*>(&addr), &asize);
}

int client_handler(const SOCKET client)
{
    char buf[32]{};

    int ret{ recv(client, buf, (sizeof(buf) - 1), 0) };

    if (ret > 0) {

        buf[ret] = 0;

        cout << "Receive :" << buf << endl;

        if (strcmp(buf, "quit")) {

            ret = send(client, buf, ret, 0);
        }
        else {
            ret = -1;
        }
    }

    return ret;
}

int main()
{
    WSADATA wd{};

    if (WSAStartup(MAKEWORD(2, 2), &wd)) {
        cout << "startup error\n";
        return -1;
    }

    const SOCKET server{ socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) };

    if (INVALID_SOCKET == server) {
        cout << "server socket error\n";
        return -1;
    }

    sockaddr_in saddr{};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl函数把小端转换成大端（网络字节序采用大端）
    saddr.sin_port = htons(8888);

    if (SOCKET_ERROR == bind(server, reinterpret_cast<const sockaddr*>(&saddr), sizeof(saddr))) {
        cout << "server bind error\n";
        return -1;
    }

    if (SOCKET_ERROR == listen(server, 1)) {
        cout << "server listen error\n";
        return -1;
    }

    cout << "server start success\n" << "server socket_fd :" << server << '\n';

    fd_set reads{};

    FD_ZERO(&reads);
    FD_SET(server, &reads);

    for (;;) {

        fd_set temps{ reads };

        timeval timeout{ 0, 10000 };

        int num{ select(0 ,&temps, nullptr, nullptr, &timeout) };

        if (num > 0) {

            for (uint32_t i{ }; i < reads.fd_count; i++) {

                const SOCKET sock{ reads.fd_array[i] };

                if (FD_ISSET(sock, &temps)) {

                    if (sock == server) {

                        const SOCKET client{ server_handler(sock) };

                        if (INVALID_SOCKET != client) {

                            FD_SET(client, &reads);
                            cout << "accept client:" << client << endl;
                        }
                    }
                    else {

                        const int r{ client_handler(sock) };

                        if (-1 == r) {

                            FD_CLR(sock, &reads);
                            cout << "client(" << sock << ") quit\n";
                            closesocket(sock);
                        }
                    }
                }
            }
        }
    }
    closesocket(server);
    WSACleanup();
    return 0;
}