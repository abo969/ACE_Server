1. for each autorun-server? script

    each autorun-server? script is written with Expect and TCL.

1.1 right running way

    fox example, autorun-server1 and autorun-server1d, it must be run like the following.    

(1) from linux shell

    # ./autorun-server1
    # ./autorun-server1d

(2) in tclsh command

    # tclsh
    % ./autorun-server1
    spawn ./server1
    ---------------------------------------------
    input command to test the program
     s or S : start the talker (ACE Task)
     q or Q : quit
    ---------------------------------------------
    $ input command >s
    (3079489232) main::start
    (3079489232) MyTask start
    (3079486320) enter MyTask::svc()
    
    (3079486320) in Handler()
    
    recv: No. = 1, host = localhost, IP = 7f000001, port = 40688, bytes = 21
          data = a ACE_SOCK_Dgram test
    (3079486320) handle_events(10) succeed, result = 1
    
    recv: No. = 2, host = localhost, IP = 7f000001, port = 33576, bytes = 21
          data = a ACE_SOCK_Dgram test
    (3079486320) handle_events(10) succeed, result = 1
    
    (3079486320) handle_events(10) timeout, result = 0
    
    (3079486320) handle_input: notification data
    (3079486320) handle_events(10) succeed, result = 1
    
    (3079486320) exit MyTask::svc()
    
    (3079486320) in ~Handler()
    
    (3079489232) main::end
    
    $ input command >% 
    #    

1.2 wrong running way

    they cant not be run like the following.

    # tclsh autorun-server1
    invalid command name "spawn"
        while executing
    "spawn ./server1"
        (file "autorun-server1" line 3)

    # tclsh ./autorun-server1
    invalid command name "spawn"
        while executing
    "spawn ./server1"
        (file "./autorun-server1" line 3)


2. for autorun-client scirpt

    it is a Linux Shell Script file, so, it can be run from Linux shell directly like this.

    # ./autorun-client


3. for autorun.tcl & autorund.tcl

    they are scripts with Expect and TCL.

3.1 right running way

    it must be run by using tclsh like the following.

    # tclsh ./autorun.tcl
    start to run all servers
    
    server1 running ...
        result is in server1.txt
    server2 running ...
        result is in server2.txt
    server21 running ...
        result is in server21.txt
    server22 running ...
        result is in server22.txt
    server3 running ...
        result is in server3.txt
    server4 running ...
        result is in server4.txt
    server5 running ...
        result is in server5.txt
    server6 running ...
        result is in server6.txt
    server7 running ...
        result is in server7.txt
    server8 running ...
        result is in server8.txt
    server81 running ...
        result is in server81.txt
    server82 running ...
        result is in server82.txt
    server99 running ...
        result is in server99.txt
    server98 running ...
        result is in server98.txt
    server97 running ...
        result is in server97.txt
    
    done. bye.

3.2 wrong running way

    tyey can not be run like the following.

(1) from linux shell 

    # ./autorun.tcl 
    -bash: ./autorun.tcl: /usr/bin/expect^M: bad interpreter: No such file or directory

(2) from tclsh command line

    # tclsh
    % ./autorun.tcl
    couldn't execute "./autorun.tcl": no such file or directory
    % autorun.tcl
    invalid command name "autorun.tcl"
    % exit


4. for autorun.sh & autorund.sh

    it is Linux Shell Script file. so run it directly from Linux shell like this.    

    # ./autorun.sh
    # ./autorund.sh