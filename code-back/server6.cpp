/************************************************************************
 Description:
 Client: send data to port (6540) of localhost (ip = 127.0.0.1), and receive
         them back from Server
 Server: receive data from port 6540 on the same pc, and send the received 
         data back
 Client and server are running on the same pc.

 Execution flow:
 1. Server wait for data on the port, if timeout for waiting, server will end
 2. Client send data to the port
 3. Server receive the data from the port and show them, and send the data back, 
    and go on waiting бн
 4. Client receive the back data and show them, then end
 Notation: client will be run manually when sending data.

 Target:
 1. server call select to tell kernel to wait socket event, if timeout for select,
    it will exit from svc and server will end, else, recv data from the port.
 2. provide menu for user on server, to allow user select q (quit from program) 
    and s (restart server)
 ************************************************************************/

//#define ACE_NTRACE 0
#include <ace/Log_Msg.h>

#include <ace/SOCK_Dgram.h>
#include <ace/Task.h>

#include <stdio.h>
#include <string.h>

static const u_short UDP_PORT = 6540;
static const int     TIMEOUT = 10;

class MyTask: public ACE_Task<ACE_NULL_SYNCH>
{
public:
    static const int MAX_USER_THREAD = 1;

    MyTask(u_short udp_port);
    ~MyTask();

    int open(void *);
    virtual int svc();    

private:
    ACE_SOCK_Dgram dgramt_;
    ACE_INET_Addr  addr_;

    static int exit_flags;
    static int recv_count;
};

int MyTask::exit_flags = 0;
int MyTask::recv_count = 0;

MyTask::MyTask(u_short udp_port): addr_(udp_port)
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) in MyTask()\n\n")));
    exit_flags = 0;
    recv_count = 0;
    dgramt_ = ACE_SOCK_Dgram(addr_);
}

MyTask::~MyTask(void)
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) in ~MyTask()\n")));
    if (this->dgramt_.close() == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "ACE_SOCK_Dgram close failed"));
    }
}

int MyTask::open(void *)
{
    ACE_DEBUG ((LM_INFO, "(%t) MyTask start\n"));    

    if(this->activate(THR_NEW_LWP, MAX_USER_THREAD) == -1)
    {
        ACE_DEBUG((LM_ERROR, "activate MyTask failed"));
        return -1;
    }

    return 0;
}

int MyTask::svc()
{
    char buf[BUFSIZ] = {0};
    ACE_INET_Addr  remote_addr;
    int timeout = TIMEOUT;
    ACE_Time_Value tv(timeout);

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) enter MyTask::svc()\n\n")));

    ACE_HANDLE handle = dgramt_.get_handle();
    
    while (exit_flags == 0)
    {
        fd_set set;
        struct timeval timeout;

        //Initialize the file descriptor set
        FD_ZERO (&set);
        FD_SET (handle, &set);

        //Initialize the timeout data structure
        timeout.tv_sec = TIMEOUT;
        timeout.tv_usec = 0;

        int res = select (handle + 1, &set, NULL, NULL, &timeout);
        switch(res)
        {
        case -1:    //error
            ACE_ERROR((LM_ERROR, "%p\n", "select failed"));
            break;
        case 0:
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) select timeout (%d seconds), result = %d\n\n"), 
                timeout, res));
            exit_flags = 1;
            break;
        default:
            ssize_t result = this->dgramt_.recv(buf, sizeof buf, remote_addr, 0, &tv);
    	    if (result == -1)
    	        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "MyTask::svc(): recv failed, will exit"), -1);

            ACE_DEBUG((LM_DEBUG, "recv: No. = %d, host = %s, IP = %x, port = %d, bytes = %d\n",
                ++recv_count, remote_addr.get_host_name(), remote_addr.get_ip_address(), 
                remote_addr.get_port_number(), result));
            ACE_DEBUG((LM_INFO, "      data = %s\n\n", buf));

            this->dgramt_.send(buf, result, remote_addr);  //send them back
        }
    }

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) exit MyTask::svc()\n\n")));
    return 0;
}

//////////////////////////////////////////////////////////////////////////
void show_menu() 
{ 
    printf("---------------------------------------------\n"); 
    printf("input command to test the program\n"); 
    printf(" s or S : start the talker (ACE Task)\n"); 
    printf(" q or Q : quit\n"); 
    printf("---------------------------------------------\n"); 
    printf("$ input command >"); 
}

void start_task()
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) main::start\n")));

    MyTask *mytask = new MyTask(UDP_PORT);
    mytask->open(0);

    ACE_Thread_Manager::instance ()->wait ();

    delete mytask;
    mytask = NULL;
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) main::end\n\n")));
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
    char sinput[10]; 

    show_menu(); 

    scanf("%s",sinput); 
    while(strcmp(sinput,"q")!=0) 
    { 
        if(strcmp(sinput,"s")==0) 
        { 
            start_task();
        } 

        //input command 
        printf("$ input command >"); 
        scanf("%s",sinput); 
    } 

    return 0;
}

