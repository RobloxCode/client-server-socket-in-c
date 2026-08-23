CC = gcc
CFLAGS = -Wall -Wextra

SRCSERVER = server.c
SRCCLIENT = client.c

OUTSERVER = server
OUTCLIENT = client

client-server-socket:
	$(CC) $(SRCSERVER) $(CFLAGS) -o $(OUTSERVER)
	$(CC) $(SRCCLIENT) $(CFLAGS) -o $(OUTCLIENT)

clean:
	rm -f $(OUTSERVER)
	rm -f $(OUTCLIENT)
