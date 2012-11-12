all:
	g++ -g -o server1 server1.cpp -lACE -lpthread
	g++ -g -o server2 server2.cpp -lACE -lpthread
	g++ -g -o server21 server21.cpp -lACE -lpthread
	g++ -g -o server22 server22.cpp -lACE -lpthread
	g++ -g -o server3 server3.cpp -lACE -lpthread
	g++ -g -o server4 server4.cpp -lACE -lpthread
	g++ -g -o server5 server5.cpp -lACE -lpthread
	g++ -g -o server6 server6.cpp -lACE -lpthread
	g++ -g -o server7 server7.cpp -lACE -lpthread
	g++ -g -o server8 server8.cpp -lACE -lpthread
	g++ -g -o server81 server81.cpp -lACE -lpthread
	g++ -g -o server82 server82.cpp -lACE -lpthread
	g++ -g -o server99 server99.cpp -lACE -lpthread
	g++ -g -o server98 server98.cpp -lACE -lpthread
	g++ -g -o server97 server97.cpp -lACE -lpthread

trace:
	g++ -DACE_NTRACE=0 -g -o server1d server1.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server2d server2.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server21d server21.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server22d server22.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server3d server3.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server4d server4.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server5d server5.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server6d server6.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server7d server7.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server8d server8.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server81d server81.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server82d server82.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server99d server99.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server98d server98.cpp -lACE -lpthread
	g++ -DACE_NTRACE=0 -g -o server97d server97.cpp -lACE -lpthread

server1: server1.cpp 
	g++ -g -o server1 server1.cpp -lACE -lpthread
server2: server2.cpp
	g++ -g -o server2 server2.cpp -lACE -lpthread
server21: server21.cpp
	g++ -g -o server21 server21.cpp -lACE -lpthread
server22: server22.cpp
	g++ -g -o server22 server22.cpp -lACE -lpthread
server3: server3.cpp
	g++ -g -o server3 server3.cpp -lACE -lpthread
server4: server4.cpp
	g++ -g -o server4 server4.cpp -lACE -lpthread
server5: server5.cpp
	g++ -g -o server5 server5.cpp -lACE -lpthread
server6: server6.cpp
	g++ -g -o server6 server6.cpp -lACE -lpthread
server7: server7.cpp
	g++ -g -o server7 server7.cpp -lACE -lpthread
server8: server8.cpp
	g++ -g -o server8 server8.cpp -lACE -lpthread
server81: server81.cpp
	g++ -g -o server81 server81.cpp -lACE -lpthread
server82: server82.cpp
	g++ -g -o server82 server82.cpp -lACE -lpthread
server99: server99.cpp
	g++ -g -o server99 server99.cpp -lACE -lpthread
server98: server98.cpp
	g++ -g -o server98 server98.cpp -lACE -lpthread
server97: server97.cpp
	g++ -g -o server97 server97.cpp -lACE -lpthread
	
server1d: server1.cpp 
	g++ -DACE_NTRACE=0 -g -o server1d server1.cpp -lACE -lpthread
server2d: server2.cpp
	g++ -DACE_NTRACE=0 -g -o server2d server2.cpp -lACE -lpthread
server21d: server21.cpp
	g++ -DACE_NTRACE=0 -g -o server21d server21.cpp -lACE -lpthread
server22d: server22.cpp
	g++ -DACE_NTRACE=0 -g -o server22d server22.cpp -lACE -lpthread
server3d: server3.cpp
	g++ -DACE_NTRACE=0 -g -o server3d server3.cpp -lACE -lpthread
server4d: server4.cpp
	g++ -DACE_NTRACE=0 -g -o server4d server4.cpp -lACE -lpthread
server5d: server5.cpp
	g++ -DACE_NTRACE=0 -g -o server5d server5.cpp -lACE -lpthread
server6d: server6.cpp
	g++ -DACE_NTRACE=0 -g -o server6d server6.cpp -lACE -lpthread
server7d: server7.cpp
	g++ -DACE_NTRACE=0 -g -o server7d server7.cpp -lACE -lpthread
server8d: server8.cpp
	g++ -DACE_NTRACE=0 -g -o server8d server8.cpp -lACE -lpthread
server81d: server81.cpp
	g++ -DACE_NTRACE=0 -g -o server81d server81.cpp -lACE -lpthread
server82d: server82.cpp
	g++ -DACE_NTRACE=0 -g -o server82d server82.cpp -lACE -lpthread
server99d: server99.cpp
	g++ -DACE_NTRACE=0 -g -o server99d server99.cpp -lACE -lpthread
server98d: server98.cpp
	g++ -DACE_NTRACE=0 -g -o server98d server98.cpp -lACE -lpthread
server97d: server97.cpp
	g++ -DACE_NTRACE=0 -g -o server97d server97.cpp -lACE -lpthread

cleanall:
	rm server1
	rm server2
	rm server21
	rm server22
	rm server3
	rm server4
	rm server5
	rm server6
	rm server7
	rm server8
	rm server81
	rm server82
	rm server99
	rm server98
	rm server97

cleantrace:
	rm server1d
	rm server2d
	rm server21d
	rm server22d
	rm server3d
	rm server4d
	rm server5d
	rm server6d
	rm server7d
	rm server8d
	rm server81d
	rm server82d
	rm server99d
	rm server98d
	rm server97d

cleanserver1:
	rm server1
cleanserver2:
	rm server2
cleanserver21:
	rm server21
cleanserver22:
	rm server22
cleanserver3:
	rm server3
cleanserver4:
	rm server4
cleanserver5:
	rm server5
cleanserver6:
	rm server6
cleanserver7:
	rm server7
cleanserver8:
	rm server8
cleanserver81:
	rm server81
cleanserver82:
	rm server82
cleanserver99:
	rm server99
cleanserver98:
	rm server98
cleanserver97:
	rm server97

cleanserver1d:
	rm server1d
cleanserver2d:
	rm server2d
cleanserver21d:
	rm server21d
cleanserver22d:
	rm server22d
cleanserver3d:
	rm server3d
cleanserver4d:
	rm server4d
cleanserver5d:
	rm server5d
cleanserver6d:
	rm server6d
cleanserver7d:
	rm server7d
cleanserver8d:
	rm server8d
cleanserver81d:
	rm server81d
cleanserver82d:
	rm server82d
cleanserver99d:
	rm server99d
cleanserver98d:
	rm server98d
cleanserver97d:
	rm server97d

client:
	g++ -g -o client client.cpp -lACE -lpthread
	g++ -g -o client1 client1.cpp -lACE -lpthread

cleanclient:
	rm client
	rm client1