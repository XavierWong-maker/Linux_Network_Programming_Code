#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main() 
{
   int server {socket(PF_INET,SOCK_STREAM,0)};

   if (-1 == server){
       cout << "server socket error\n";
       return -1;
   }

   sockaddr_in saddr {};

   saddr.sin_family = AF_INET;
   saddr.sin_addr.s_addr = htonl(INADDR_ANY);//htonl函数把小端转换成大端（网络字节序采用大端）
   saddr.sin_port = htons(8888);

   if ( -1 == bind( server,reinterpret_cast<const sockaddr *>(&saddr),sizeof(saddr) ) ){
       cout << "server bind error\n";
       return -1;
   }

   if ( -1 == listen(server,1) ){
       cout << "server listen error\n";
       return -1;
   }

   cout << "server start success\n";

   while (true){

       sockaddr_in caddr {};
       socklen_t asize {sizeof(caddr)};

       const int client {accept(server,reinterpret_cast<sockaddr *>(&caddr),&asize)};

       if (-1 == client){
           cout << "client accept error\n";
           return -1;
       }

       cout << "client :" << client << '\n'; //client的数值表示系统资源的id

       int r{};

       do{
           char buf[32]{};

           r = recv(client,buf,(sizeof(buf)/sizeof(*buf)),0);

           if (r > 0){
               cout << "Recv: " << buf << '\n';
           }

       } while (r > 0);

       close(client);
   }

   close(server);

   return 0;
}