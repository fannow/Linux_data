.PHONY:all
all:sever client
sever:sever.cc
	g++ -o $@ $^
client:client.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f sever client
