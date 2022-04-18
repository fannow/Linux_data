.PHONY:all
all:tcpsever tcpclient
tcpsever:tcpsever.cc
	g++ -o $@ $^
tcpclient:tcpclient.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f tcpsever tcpclient
