#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>
#include <ace/Time_Value.h> 

#include <string>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#pragma comment (lib,"aced.lib")
#else
#pragma comment (lib,"ace.lib")
#endif

int main(int argc, char *argv[]) 
{
    //port to bind
    ACE_INET_Addr addr(6540);

    //communication channel
    ACE_SOCK_Dgram peer(addr);

    char buf[100] = {0};
    while(true)
    {
        ACE_INET_Addr remoteAddr;

        //receive data and get the remote address
        int bc = peer.recv(buf,100,remoteAddr);
        if( bc != -1)
        {
            string s(buf,bc);
            cout<<endl<<"rev: "<<s<<endl;
        }

        //send to the remote address
        peer.send(buf,bc,remoteAddr);
    }

    return 0; 
} 
