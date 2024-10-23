
ALL:
	cc -std=c99 01hv.c -o 01hv -lhv
	c++ -std=c++11 hmain_test.cpp -o hmain_test -I/usr/local/include/hv -lhv
	cc -std=c99 hvloop.c -o hvloop -lhv
	cc -std=c99 tcp_server.c -o tcp_server -lhv
	c++ -std=c++11 http_server_test.cpp -o http_server_test -I/usr/local/include/hv -lhv


