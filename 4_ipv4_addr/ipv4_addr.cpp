#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc,char* argv[]) 
{
    {
    #if 1
        const unsigned int addr {inet_addr("1.2.3.4")};
    #else
        const unsigned int addr {inet_addr("D.T.Software")};
    #endif
    
    cout << "addr = 0x" << hex << addr << endl;
    }

    {
    #define YES 1
        in_addr addr1 {0x09080706};
        in_addr addr2 {0x05040302};

    #if YES
        char * s1 {strcpy(new char[32], inet_ntoa(addr1))};
        char * s2 {strcpy(new char[32], inet_ntoa(addr2))};
        
        cout << "addr1 = 0x" << hex << addr1.s_addr << '\n';
        cout << "addr2 = 0x" << hex << addr2.s_addr << '\n';
        cout << "s1 = " << s1 << '\n';
        cout << "s2 = " << s2 << '\n';
        
        delete[] s1;
        delete[] s2;

    #else
        char * s1 {inet_ntoa(addr1)};
        char * s2 {inet_ntoa(addr2)};
        
        cout << "addr1 = 0x" << hex << addr1.s_addr << '\n';
        cout << "addr2 = 0x" << hex << addr2.s_addr << '\n';
        cout << "s1 = " << s1 << '\n';
        cout << "s2 = " << s2 << '\n';
        
    #endif
}

    in_addr addr3 {};

    #if 1
        if(inet_aton("1.2.3.4", &addr3)){
            cout << "addr3 = 0x" << hex << addr3.s_addr << '\n';
        }

    #else
    if (inet_aton("D.T.Software",&addr3)){
        cout << "addr3 = 0x" << hex << addr3.s_addr << '\n';
    }

    #endif

    return 0;
}