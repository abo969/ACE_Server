there are 5 kinds of scripts, and all kinds of scripts can be executed.

1. autorun-server*

    * represents a digit, such as 4, then the script will be autorun-server4.

    this kind of script is the automatical executable script of a server, for example, server4. of course, when it is running, if it needs communication with client, then, you must manually execute client program like this.
    # ./client
    
    or
    
    # ./autorun-client

2. autorun-serverd*

    * has the same meaning with above.

    this kind of script is the automatical executable script of a server with ACE trace information. as above, when it is running, if it needs communication with cient, you must manually execute client program too.

3. autorun-client

    the automatically executable script of client program.

4. autorun & autorund

    the automatically executable script of all autorun-server*s & autorun-serverd*s scripts. they are written with Expect and TCL.

5. autorun1 & autorund1

    the automatically executable script of all autorun-server*s & autorun-serverd*s scripts. they are Linux Shell Scripts.


summary

    autorun-client                Linux Shell
    autorun-server*               Expect/TCL
    autorun-serverd*              Expect/TCL
    autorun.tcl & autorund.tcl    Expect/TCL
    autorun.sh  & autorund.sh     Linux Shell    