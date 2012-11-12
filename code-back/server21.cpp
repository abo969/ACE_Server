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
 1. if timeout for server waiting, it will start another task, and in the new task,
    it will register a new handler but not remove it after sleep for 1 second. the new
    task does them 2 times. If the timeout times reaches 2, it will exit from the 
    forever-loop handle_events and server will end.
 2. provide menu for user on server, to allow user select q (quit from program) 
    and s (restart server)
************************************************************************/

//#define ACE_NTRACE 0
#include <ace/Log_Msg.h>

#include <ace/OS_NS_unistd.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Reactor.h>
#include <ace/Task.h>

#include <stdio.h>
#include <string.h>

static const char*   IP_ADDR  = "127.0.0.1";
static const u_short UDP_PORT = 6540;
static const int     TIMEOUT_COUNT = 2;
static const int     REGISTER_COUNT = 2;
static const int     TIMEOUT = 10;

//////////////////////////////////////////////////////////////////////////

class MyTask: public ACE_Task<ACE_NULL_SYNCH>
{
public:
    static const int MAX_USER_THREAD = 1;

    int open(void *);
    virtual int svc();    

    static int timeout_count;
    static int exit_flag;
};

int MyTask::timeout_count = 0;
int MyTask::exit_flag = 0;

typedef ACE_Singleton <MyTask, ACE_SYNCH_MUTEX> mytask;

//////////////////////////////////////////////////////////////////////////

class MyTask2:public ACE_Task<ACE_NULL_SYNCH>
{
public:
    static const int MAX_USER_THREAD = 1;

    int open(void *);
    virtual int svc();    
};

typedef ACE_Singleton <MyTask2, ACE_SYNCH_MUTEX> mytask2;

//////////////////////////////////////////////////////////////////////////

class Handler: public ACE_Event_Handler
{
public:
    Handler(u_short udp_port);
    ~Handler(void);

    virtual int handle_input(ACE_HANDLE);
    virtual ACE_HANDLE get_handle(void) const;

private:
    ACE_SOCK_Dgram dgramt_;
    ACE_INET_Addr  addr_;

    static int recv_count;
};

int Handler::recv_count = 0;

//////////////////////////////////////////////////////////////////////////

int MyTask::open(void *)
{
    ACE_DEBUG ((LM_INFO, "(%t) MyTask start\n"));    

    exit_flag = 0;

    if(this->activate(THR_NEW_LWP, MAX_USER_THREAD) == -1)
    {
        ACE_DEBUG((LM_ERROR, "activate MyTask failed"));
        return -1;
    }

    return 0;
}

int MyTask::svc()
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) enter MyTask::svc()\n\n")));

    Handler handler(UDP_PORT);
    if (ACE_Reactor::instance()->register_handler(&handler, ACE_Event_Handler::READ_MASK) == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "cant't register with Reactor in MyTask::svc()\n"));
        return -1;
    }

    while (1)
    {
        int timeout = TIMEOUT;
        ACE_Time_Value tv(timeout);
        int result = ACE_Reactor::instance()->handle_events(tv);
        if (result == 0)    //timeout
        {
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) handle_events(%d) timeout, result = %d\n\n"), timeout, result));
            timeout_count++;
            if (TIMEOUT_COUNT == timeout_count)
            {
                exit_flag = 1;
            }
            mytask2::instance()->open(0);
        }
        else
        {
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) handle_events(%d) succeed, result = %d\n\n"), timeout, result));
            if (exit_flag)
            {                
                break;
            }
        }
    }

    if (ACE_Reactor::instance()->remove_handler(&handler, ACE_Event_Handler::READ_MASK | 
        ACE_Event_Handler::DONT_CALL) == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "cant't remove handler from Reactor\n"));
        return -1;
    }
    
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) exit MyTask::svc()\n\n")));
    return 0;
}

//////////////////////////////////////////////////////////////////////////

int MyTask2::open(void *)
{
    ACE_DEBUG ((LM_INFO, "(%t) MyTask2 start\n"));    

    if(this->activate(THR_NEW_LWP, MAX_USER_THREAD) == -1)
    {
        ACE_DEBUG((LM_ERROR, "activate MyTask2 failed"));
        return -1;
    }

    return 0;
}

int MyTask2::svc()
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) enter MyTask2::svc()\n\n")));

    for (int i = 0; i < REGISTER_COUNT; i++)
    {
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) MyTask2::svc() register ACE_STDIN handler\n")));
        if (ACE_Event_Handler::register_stdin_handler(this, ACE_Reactor::instance(), ACE_Thread_Manager::instance()) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "(%t) MyTask2::svc() register_stdin_handler failed\n"));
            return -1;
        }

        ACE_OS::sleep(1);

        //remove these codes, it will not wakeup the wait-state thread. 
        //but unfortunately, it will crash when destructing on Linux platform
        /*
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) MyTask2::svc() remove ACE_STDIN handler\n\n")));
        if (ACE_Reactor::instance()->remove_handler(ACE_STDIN, ACE_Event_Handler::READ_MASK) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "(%t) MyTask2::svc() remove_stdin_handler failed\n"));
            return -1;
        }

        ACE_OS::sleep(1);
        */
    }

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) exit MyTask2::svc()\n\n")));
    return 0;
}

//////////////////////////////////////////////////////////////////////////

ACE_HANDLE Handler::get_handle(void) const
{
    return this->dgramt_.get_handle();
}

int Handler::handle_input(ACE_HANDLE h)
{
    char buf[BUFSIZ] = {0};
    ACE_INET_Addr remote_addr;

    ssize_t result = this->dgramt_.recv(buf, sizeof buf, remote_addr);
    if (result == -1)
        ACE_ERROR_RETURN((LM_ERROR, "%p\n", "handle_input: recv failed"), -1);

    //receive successfully
    ACE_DEBUG((LM_DEBUG, "recv: No. = %d, host = %s, IP = %x, port = %d, bytes = %d\n",
        ++recv_count, remote_addr.get_host_name(), remote_addr.get_ip_address(), 
        remote_addr.get_port_number(), result));
    ACE_DEBUG((LM_INFO, "      data = %s\n", buf));

    this->dgramt_.send(buf, result, remote_addr);

    return 0;
}

Handler::Handler(u_short udp_port): addr_(udp_port)
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) in Handler()\n\n")));
    recv_count = 0;
    dgramt_ = ACE_SOCK_Dgram(addr_);
}

Handler::~Handler(void)
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) in ~Handler()\n\n")));
    if (this->dgramt_.close() == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "ACE_SOCK_Dgram close failed"));
    }
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

    mytask::instance()->open(0);
    
    ACE_Thread_Manager::instance ()->wait ();

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
