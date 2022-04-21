.PHONY:all
all:tcpsever tcpclient
tcpsever:tcpsever.cc
	g++ -o $@ $^ -lpthread
tcpclient:tcpclient.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f tcpsever tcpclient
