client: server
	gcc client.c -o client

server:
	gcc server.c -o server -lpthread

clean:
	rm client server *.o
