#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <functional>

using namespace std;

static function<void(int, siginfo_t*, void*)> func_;

static void signal_handler(const int sig, siginfo_t* info, void*){

    constexpr char str[] {"\nexit\n"};
    write(0, str, sizeof(str));
    if(func_){
        func_(sig, info, nullptr);
    }

    exit(0);
}

int server_handler(int server){

    sockaddr_in addr{};
    socklen_t asize {sizeof(addr)};
    return accept(server, reinterpret_cast<sockaddr*>(&addr), &asize);
}

int client_handler(const int client){

    char buf[32]{};

    int ret ( read(client, buf, (sizeof(buf) - 1)) );

    if(ret > 0){

        buf[ret] = 0;

        cout << "Receive (" << client << ") : " << buf << endl;

        if (strcmp(buf,"quit")){
           
           ret = write(client,reinterpret_cast<const void *>(buf),ret);
       }else{
           ret = -1;
       }
    }

    return ret;
}

int main(int argc, char const *argv[])
{
    const int server {socket(PF_INET,SOCK_STREAM,0)};

    if(-1 == server){
        cout << "server socket error\n";
        return -1;
    }

    func_ = [&](const int sig, siginfo_t* info, void*){
        close(server);
    };

    struct sigaction act{};
    act.sa_flags = SA_RESTART | SA_SIGINFO;
    act.sa_sigaction = signal_handler;

    sigaction(SIGINT, &act, nullptr);

    sockaddr_in saddr{};
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(8888);

    if(-1 == bind(server, reinterpret_cast<const sockaddr*>(&saddr), sizeof(saddr))){
        cout << "server bind error\n";
        return -1;
    }

    if(-1 == listen(server, 1)){
        cout << "server listen error\n";
        return -1;
    }

    cout << "server start success\n" << "server scoket_fd : " << server << endl;

    int max{server};
    fd_set reads{};

    FD_ZERO(&reads);
    FD_SET(server, &reads);

    while (true)
    {
        fd_set temps {reads};

        timeval timeout{.tv_sec = 0, .tv_usec = 10000};

        int num{ select((max + 1), &temps, nullptr, nullptr, &timeout)};

        if(num > 0){
            for(int i {server}; i <= max; i++){
                if(FD_ISSET(i, &temps)){
                    if(i == server){

                        const int client {server_handler(server)};

                        if(client > -1){

                            FD_SET(client, &reads);

                            max = (client > max)? client : max;

                            cout << "accept client: " << client << 
                                    " max: " << max << 
                                    " server: " << server << endl;
                        }

                    }else{
                        
                        const int r {client_handler(i)};

                        if(-1 == r){
                            
                            FD_CLR(i, &reads);
                            close(i);
                            cout << "client(" << i << ") quit\n";
                        }
                    }
                }
            }
        }
    }
    
    close(server);

    return 0;
}
