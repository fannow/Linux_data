.PHONY:all
all:client sever
client:client.cc
	g++ -o $@ $^
sever:sever.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f sever client
