#! /usr/bin/expect

puts "start to run all servers\n"

puts "server1d running ..."
exec ./autorun-server1d > server1d.txt
puts "    result is in server1d.txt"

puts "server2d running ..."
exec ./autorun-server2d > server2d.txt
puts "    result is in server2d.txt"

puts "server21d running ..."
exec ./autorun-server21d > server21d.txt
puts "    result is in server21d.txt"

puts "server22d running ..."
exec ./autorun-server22d > server22d.txt
puts "    result is in server22d.txt"

puts "server3d running ..."
exec ./autorun-server3d > server3d.txt
puts "    result is in server3d.txt"

puts "server4d running ..."
exec ./autorun-server4d > server4d.txt
puts "    result is in server4d.txt"

puts "server5d running ..."
exec ./autorun-server5d > server5d.txt
puts "    result is in server5d.txt"

puts "server6d running ..."
exec ./autorun-server6d > server6d.txt
puts "    result is in server6d.txt"

puts "server7d running ..."
exec ./autorun-server7d > server7d.txt
puts "    result is in server7d.txt"

puts "server8d running ..."
exec ./autorun-server8d > server8d.txt
puts "    result is in server8d.txt"

puts "server81d running ..."
exec ./autorun-server81d > server81d.txt
puts "    result is in server81d.txt"

puts "server82d running ..."
exec ./autorun-server82d > server82d.txt
puts "    result is in server82d.txt"

puts "server99d running ..."
exec ./autorun-server99d > server99d.txt
puts "    result is in server99d.txt"

puts "server98d running ..."
exec ./autorun-server98d > server98d.txt
puts "    result is in server98d.txt"

puts "server97d running ..."
exec ./autorun-server97d > server97d.txt
puts "    result is in server97d.txt\n"

puts "done. bye."