#! /usr/bin/expect

puts "start to run all servers\n"

puts "server1 running ..."
exec ./autorun-server1 > server1.txt
puts "    result is in server1.txt"

puts "server2 running ..."
exec ./autorun-server2 > server2.txt
puts "    result is in server2.txt"

puts "server21 running ..."
exec ./autorun-server21 > server21.txt
puts "    result is in server21.txt"

puts "server22 running ..."
exec ./autorun-server22 > server22.txt
puts "    result is in server22.txt"

puts "server3 running ..."
exec ./autorun-server3 > server3.txt
puts "    result is in server3.txt"

puts "server4 running ..."
exec ./autorun-server4 > server4.txt
puts "    result is in server4.txt"

puts "server5 running ..."
exec ./autorun-server5 > server5.txt
puts "    result is in server5.txt"

puts "server6 running ..."
exec ./autorun-server6 > server6.txt
puts "    result is in server6.txt"

puts "server7 running ..."
exec ./autorun-server7 > server7.txt
puts "    result is in server7.txt"

puts "server8 running ..."
exec ./autorun-server8 > server8.txt
puts "    result is in server8.txt"

puts "server81 running ..."
exec ./autorun-server81 > server81.txt
puts "    result is in server81.txt"

puts "server82 running ..."
exec ./autorun-server82 > server82.txt
puts "    result is in server82.txt"

puts "server99 running ..."
exec ./autorun-server99 > server99.txt
puts "    result is in server99.txt"

puts "server98 running ..."
exec ./autorun-server98 > server98.txt
puts "    result is in server98.txt"

puts "server97 running ..."
exec ./autorun-server97 > server97.txt
puts "    result is in server97.txt\n"

puts "done. bye."