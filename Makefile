all: client server

client:
	cc client.c -o client

server:
	cc server.c -o server

clean:
	rm client server