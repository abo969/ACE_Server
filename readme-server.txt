1. server1
    MyTask, Handler
    exit when timeout, by calling ACE_Reactor::notify, user-defined notify, or wakeup_all_threads to exit
    provide menu for user
	
    超时即调用ACE_Reactor::notify()或者自定义的notify，或者wakeup_all_threads退出

2. server2
    MyTask, MyTask2, Handler
    MyTask timeout, then spawn MyTask2
    MyTask2 register/remove ACE_STDIN handler 2 times
    exit according to the number of timeout (2 times)
    provide menu for user
	
    超时时注册/删除ACE_STDIN的handler各2次，第2次超时时退出（但第2次超时时注册/删除ACE_STDIN的handler依然进行，只是在MyTask退出后不再处理注册/删除的事件了）

3. server21
    MyTask, MyTask2, Handler
    MyTask timeout, then spawn MyTask2
    MyTask2 register ACE_STDIN handler 3 times
    exit according to the number of timeout (2 times)
    provide menu for user

    超时时注册ACE_STDIN的handler 2次，第2次超时时退出（但第2次超时时注册ACE_STDIN的handler依然进行，只是在MyTask退出后不再处理注册的事件了）

4. server22
    MyTask, MyTask2, Handler
    MyTask timeout, set the exit flag, then spawn MyTask2
    MyTask2 register ACE_STDIN handler 1 times, then the registeration will wakeup the wait-state thread
    then MyTask exit according to the eixt flag
    provide menu for user

    超时时设置退出标志，并注册ACE_STDIN的handler，该注册动作唤醒handle_events，根据标志退出
 
5. server3
    MyTask, Handler, timeout, wakeup_all_threads to exit
	
    超时时设置退出标志，并直接调用wakeup_all_threads唤醒handle_events，根据标志退出

6. server4
    MyTask, Handler, timeout, wakeup_all_threads to exit
    provide menu for user
	
    超时时设置退出标志，并直接调用wakeup_all_threads唤醒handle_events，根据标志退出

7. server5
    MyTask, no Handler, in svc, receive data directly by ACE_SOCK_Dgram, timeout to exit
    provide menu for user

    没有Handler，不使用ACE_Reactor的handle_events，直接使用ACE_SOCK_Dgram发送/接受数据，且超时时直接break退出

8. server6
    MyTask, no Handler, in svc, use select (Linux system call) to wait event, receive data directly by ACE_SOCK_Dgram, timeout to exit
    provide menu for user

    没有Handler，不使用ACE_Reactor的handle_events，直接调用Linux系统调用select监测端口状态，若有数据到来，则使用ACE_SOCK_Dgram接受数据，且超时时设置退出标志，根据标志退出

9. server7
    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, Handler, timeout, wakeup_all_threads to exit
    provide menu for user

    直接使用ACE_Select_Reactor，超时时设置退出标志，并直接调用wakeup_all_threads唤醒handle_events，根据标志退出

10. server8
    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove user-defined handler 2 times
    exit according to the number of receive data package
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask中的handle_events没有超时。MyTask2注册/删除自定义的handler 2次。MyTask收到client的2个数据包退出。

11. server81
    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove ACE_STDIN handler 2 times
    exit according to the number of receive data package
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask中的handle_events没有超时。MyTask2注册/删除ACE_STDIN的handler 2次。MyTask收到client的2个数据包退出。

12. server82
    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove signal handler 2 times
    exit according to the number of receive data package
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask中的handle_events没有超时。MyTask2注册/删除signal的handler 2次。MyTask收到client的2个数据包退出。

13. server99
    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove given ACE_HANDLE value handler such as 1023 2 times
    using notify the given ACE_HANDLE value handler to exit according to the number of receive data package
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask::svc中的handle_events没有超时。
    MyTask2注册/删除给定值的handler（例如1023）2次。MyTask收到client的2个数据包时调用ACE_Reactor::notify方法，且使用ACE_Event_Handler::EXCEPT_MASK参数，并在该mask对应的处理函数handle_exception中设置退出标志，并根据该标志退出MyTask。
    MyTask::svc中的ACE_Reactor和Handler对象都是局部对象，selector是MyTask的类对象。
    MyTask和MyTask2都是ACE_Singleton对象。

14. server98

    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove given ACE_HANDLE value handler such as 1023 2 times
    using notify the given ACE_HANDLE value handler to exit according to the number of receive data package
    ACE_singleton task is not used, Reactor, Selector is data member of MyTask
    mytask is a local object, the destruction of Reactor, Selector is done in ~MyTask
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask::svc中的handle_events没有超时。
    MyTask2注册/删除给定值的handler（例如1023）2次。MyTask收到client的2个数据包时调用ACE_Reactor::notify方法，且使用ACE_Event_Handler::EXCEPT_MASK参数，并在该mask对应的处理函数handle_exception中设置退出标志，并根据该标志退出MyTask。
    MyTask::svc中的Handler对象是局部对象，reactor和selector是MyTask的类对象。在~MyTask中释放这两个类对象。
    MyTask2对象是MyTask::svc方法中的局部对象。每次MyTask任务重新启动，都重新生成MyTask对象。

15. server97

    implement ACE_Reactor with ACE_Select_Reactor_T
    MyTask, MyTask2, Handler
    MyTask has no timeout
    MyTask spawn MyTask2 before the forever-loop handle_events
    MyTask2 register/remove given ACE_HANDLE value handler such as 1023 2 times
    using notify the given ACE_HANDLE value handler to exit according to the number of receive data package
    ACE_singleton task is not used, Reactor, Selector, Hanlder is data member of MyTask
    mytask corresponds to a global object, the destruction of Reactor, Selector is done in MyTask::close
    provide menu for user

    显示使用ACE_Select_Reactor实现ACE_Reactor，在MyTask的svc方法中的无限循环前启动MyTask2，且MyTask::svc中的handle_events没有超时。
    MyTask2注册/删除给定值的handler（例如1023）2次。MyTask收到client的2个数据包时调用ACE_Reactor::notify方法，且使用ACE_Event_Handler::EXCEPT_MASK参数，并在该mask对应的处理函数handle_exception中设置退出标志，并根据该标志退出MyTask。
    reactor, selector和handler都是MyTask的类对象，在MyTask::close中释放这三个类对象。MyTask2对象是MyTask::svc方法中的局部对象。
    在程序开始时，生成MyTask对象，以后每次任务重新启动，都使用这一个MyTask对象。