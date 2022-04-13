all: utils tx rx

# TX

tx: tx.c txconnect.o utils.o mtwister.o
	gcc -g txconnect.o utils.o mtwister.o tx.c -lm -o tx

txconnect.o: txconnect.c
	gcc -g -c txconnect.c

# RX

rx: rx.c rxconnect.o utils.o mtwister.o
	gcc -g rxconnect.o utils.o mtwister.o rx.c -lm -o rx

rxconnect.o: rxconnect.c
	gcc -g -c rxconnect.c

# OTHERS

utils: utils.c mtwister/mtwister.c
	gcc -g -c utils.c
	gcc -g -c mtwister/mtwister.c

clean:
	rm -rf *.o tx rx