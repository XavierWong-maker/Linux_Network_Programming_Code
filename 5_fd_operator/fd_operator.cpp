#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    int iofd {0};

    char s[] {"D.T.Software\n"};

    write(iofd, s, sizeof(s));

    int len (read(iofd, s, 5));

    s[len] = 0;

    cout << "read: " << s << endl;

    return 0;
}
