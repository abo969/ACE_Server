/************************************************************************
 Description:
 Client: send data to port (6540) of localhost (ip = 127.0.0.1), and receive 
         them back from Server
 Server: receive data from port 6540 on the same pc, and send the received 
         data back
 Client and server are running on the same pc.

 running: 
 # ./client   //send for once
 ************************************************************************/

#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>
#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_string.h>
#include <ace/Log_Msg.h>

static const u_short UDP_PORT = 6540;
static const u_short BUF_LEN  = 100;
static const char*   IP_ADDR  = "127.0.0.1";

int main(int argc, char *argv[]) 
{
    char sbuf[BUF_LEN] = {"a ACE_SOCK_Dgram test"};
    ACE_INET_Addr remoteAddr(UDP_PORT, IP_ADDR);
    ACE_INET_Addr localAddr;
    ACE_SOCK_Dgram peer(localAddr);

    ACE_DEBUG((LM_INFO, "send: %s\n", sbuf));
    peer.send(sbuf, ACE_OS::strlen(sbuf), remoteAddr);        

    char buf[BUF_LEN] = {0};
    int bc = peer.recv(buf, BUF_LEN, remoteAddr);
    if( bc != -1)
    {
        ACE_DEBUG((LM_INFO, "recv: %s\n\n", buf));
    }

    return 0; 
} 
