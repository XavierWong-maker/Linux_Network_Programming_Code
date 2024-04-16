#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    {
        in_addr addr0 {};
        if(inet_pton(AF_INET, "1.2.3.4", &addr0)){
            cout << "addr0 = 0x" << hex << addr0.s_addr << endl;
        }
    }

    {
        char buf[32]{};
        in_addr addr1 {0x0100007f};
        const char* r {inet_ntop(AF_INET, &addr1, buf, sizeof(buf))};
        cout << "addr1 = " << buf << endl;
        cout << "r = " << r << endl;
        cout << "buf addr: " << reinterpret_cast<int*>(buf) << endl;
        cout << "r point to addr: " << reinterpret_cast<const int*>(r) << endl;
    }

    return 0;
}
