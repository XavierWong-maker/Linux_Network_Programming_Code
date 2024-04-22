#include <sys/select.h>
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int iofd {0};
    char s[] {"hello world\r\n"};
    fd_set reads{};

    FD_ZERO(&reads);
    FD_SET(iofd, &reads);// 將指定文件描述符添加到集合中

    int counter{};

    while (true)
    {
        fd_set temps{reads}; //select函数会影响temps的值,所以每次我们都需要拷贝一次以确保不会出错
        timeval timeout{.tv_sec = 0, .tv_usec = 50000};

        int r {select(1, &temps, nullptr, nullptr, &timeout)};

        if(r > 0){

            cout << "r : " << r << endl;

            int len (read(iofd, &s, (sizeof(s)-1)));

            s[len] = 0;

            cout << "Input : " << s << endl;

        }else if (0 == r){
            usleep(1000);
            if(++counter > 100){
                counter = 0;
                cout << "do something else\n";
            }

        }else{
            break;
        }
    }
    
    return 0;
}
