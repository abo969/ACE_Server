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
 1. the target is to verify whether it will call notify method when registering
    or removing user-defined handlers with given ACE_HANDLE value in another task
    when the current task handle_events has no timeout, that is, timeout is NULL.
    In task1, it will spawn task2 before entering svc-loop, and in the new task, 
    it will register a new handler and remove it after sleep for 1 second. the new
    task does them 2 times.
    Besides, the given ACE_HANDLE value handler will be bound with the user-defined
    ACE_Event_Handler, in this example, that is Handler; it will be registered/
    removed with whatever ACE_Reactor_Mask, for example, ACE_Event_Handler::READ_MASK
    /WRITE_MASK/EXCEPT_MASK, because when it is registered/removed, it will invoke 
    the notify method with default value parameters by ...->sleep_hook->notify.
    If task1 receives 2 data packets from socket (client),it will notify the given 
    ACE_HANDLE value handler with EXCEPT_MASK. So Handler will handle the exception
    event by handle_exception. And in handle_exception, exit flag will be set, and
    in the forever-loop handle_events, the exit flag will be checked, if it is 1, 
    then it will exit from the forever-loop handle_events and server will end.    
 2. provide menu for user on server, to allow user select q (quit from program) 
    and s (restart server)
************************************************************************/

//#define ACE_NTRACE 0
#include <ace/Log_Msg.h>

#include <ace/OS_NS_time.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Reactor.h>
#include <ace/Select_Reactor.h>
#include <ace/Task.h>

#include <stdio.h>
#include <string.h>

static const char*    IP_ADDR  = "127.0.0.1";
static const u_short UDP_PORT = 6540;
static const int         REGISTER_COUNT = 2;
static const int         RECV_COUNT = 2;
static const int         MY_EXIT_HANDLER = 1023;

//////////////////////////////////////////////////////////////////////////

class MyTask: public ACE_Task<ACE_NULL_SYNCH>
{
public:
    static const int MAX_USER_THREAD = 1;
    MyTask();
    ~MyTask();

    int open(void *);
    virtual int svc();

private:
    static int exit_flag;
    static ACE_Select_Reactor *selector;
};

int MyTask::exit_flag = 0;
ACE_Select_Reactor* MyTask::selector = 0;

typedef ACE_Singleton <MyTask, ACE_SYNCH_MUTEX> mytask;

//////////////////////////////////////////////////////////////////////////

class Handler: public ACE_Event_Handler
{
public:
    Handler(u_short udp_port);
    ~Handler(void);

    virtual int handle_input(ACE_HANDLE);
    virtual int handle_exception (ACE_HANDLE);

    virtual ACE_HANDLE get_handle(void) const
    {
        return this->dgramt_.get_handle();
    }

    int get_count(void) const
    {
        return recv_count;
    }

    int get_flag(void) const
    {
        return exit_flag;
    }

private:
    ACE_SOCK_Dgram dgramt_;
    ACE_INET_Addr  addr_;

    static int recv_count;
    static int exit_flag;
};

int Handler::recv_count = 0;
int Handler::exit_flag = 0;

//////////////////////////////////////////////////////////////////////////

class MyTask2:public ACE_Task<ACE_NULL_SYNCH>
{
public:
    static const int MAX_USER_THREAD = 1;

    int open(void *);
    virtual int svc();

private:
    Handler* handler_;
};

typedef ACE_Singleton <MyTask2, ACE_SYNCH_MUTEX> mytask2;

//////////////////////////////////////////////////////////////////////////

MyTask:: MyTask()
{
    ACE_DEBUG ((LM_INFO, "(%t) in MyTask:: MyTask()\n"));
    exit_flag = 0;
    selector = NULL;
}

MyTask::~MyTask()
{
    ACE_DEBUG ((LM_INFO, "(%t) in MyTask::~MyTask() 1\n"));

    delete selector;
    selector = NULL;
    ACE_DEBUG ((LM_INFO, "(%t) in MyTask::~MyTask() 2\n"));
}

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

    //initialization
    selector = new ACE_Select_Reactor;
    ACE_Reactor* reactor = new ACE_Reactor(selector);
    ACE_Reactor::instance(reactor);  //then, ACE_Reactor::instance() is reactor
    this->reactor(reactor);  //this reactor_ of ACE_Event_Handler can not be set

    //register socket handler
    Handler handler(UDP_PORT);
    if (reactor->register_handler(&handler, ACE_Event_Handler::READ_MASK) == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "cant't register with Reactor in MyTask::svc()\n"));
        return -1;
    }

    //spawn mytask2
    mytask2::instance()->open(&handler);

    //handle_events in forever-loop until receive two data packets from socket, then, it will notify the MY_EXIT_HANDLER
    while (exit_flag == 0)
    {
        int result = reactor->handle_events();  //timeout will not occur at all
        if (result)
        {
            ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) handle_events() succeed, result = %d\n\n"), result));
            if (RECV_COUNT == handler.get_count())
            {
                ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) MyTask::svc() notify the exit handler\n")));
                ACE_Reactor::instance()->notify(&handler, ACE_Event_Handler::EXCEPT_MASK);
            }

            if (handler.get_flag())
                exit_flag = 1;
        }
        else
        {
            ACE_ERROR((LM_ERROR, "%p\n", "handle_events() failed\n"));
            return -1;
        }
    }

    if (reactor->remove_handler(&handler, ACE_Event_Handler::READ_MASK | 
        ACE_Event_Handler::DONT_CALL) == -1)
    {
        ACE_ERROR((LM_ERROR, "%p\n", "cant't remove handler from Reactor\n"));
        return -1;
    }

    delete reactor;
    reactor = NULL;
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) exit MyTask::svc()\n\n")));
    return 0;
}

//////////////////////////////////////////////////////////////////////////

int MyTask2::open(void *p)
{
    ACE_DEBUG ((LM_INFO, "(%t) MyTask2 start\n"));

    handler_ = (Handler*)p;
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
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) MyTask2::svc() register exit handler\n")));
        if (ACE_Reactor::instance()->register_handler(MY_EXIT_HANDLER, handler_, ACE_Event_Handler::EXCEPT_MASK) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "(%t) MyTask2::svc() register exit handler failed\n"));
            return -1;
        }
        ACE_OS::sleep(1);

        ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) MyTask2::svc() remove exit handler\n\n")));
        if (ACE_Reactor::instance()->remove_handler(MY_EXIT_HANDLER, ACE_Event_Handler::EXCEPT_MASK) == -1)
        {
            ACE_ERROR((LM_ERROR, "%p\n", "(%t) MyTask2::svc() remove exit handler failed\n"));
            return -1;
        }
        ACE_OS::sleep(1);
    }

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) exit MyTask2::svc()\n\n")));
    return 0;
}

//////////////////////////////////////////////////////////////////////////

Handler::Handler(u_short udp_port): addr_(udp_port)
{
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) in Handler()\n\n")));
    recv_count = 0;
    exit_flag = 0;
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

int Handler::handle_exception (ACE_HANDLE h)
{
    if (ACE_INVALID_HANDLE == h)
    {
        ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%t) handle_exception: handle the exit handler\n")));
        exit_flag = 1;
        recv_count = 0;  //to avoid the second calling notify, that is, to guarantee send notification only once
    }

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

