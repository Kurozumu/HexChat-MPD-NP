CC=gcc
INC=/usr/include
LIB=/usr/lib
CFLAGS= -I$(INC) -L$(LIB)

all: hcmpdnp

hcmpdnp: hcmpdnp.c
	$(CC) $(CFLAGS) -c -fpic -o hcmpdnp.o hcmpdnp.c
	$(CC) $(CFLAGS) -shared -o ../hcmpdnp.so hcmpdnp.o

clean:
	rm ../hcmpdnp.so
	rm hcmpdnp.o

.PHONY: clean all
