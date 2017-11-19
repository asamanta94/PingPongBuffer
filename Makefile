all: ping_pong

ping_pong: ping_pong.cpp
	g++ -std=c++11 -o ping_pong ping_pong.cpp test.cpp -lpthread
	
clean:
	rm -f ping_pong