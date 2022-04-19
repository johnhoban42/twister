# Build directory
BD = build

all: tx rx

# TX

tx: utils tx/tx.c txconnect 
	gcc -g $(BD)/txconnect.o $(BD)/utils.o $(BD)/mtwister.o tx/tx.c -lm -o $(BD)/tx

txconnect: tx/txconnect.c
	gcc -g -c tx/txconnect.c -o $(BD)/txconnect.o

# RX

rx: utils rx/rx.c rxconnect
	gcc -g $(BD)/rxconnect.o $(BD)/utils.o $(BD)/mtwister.o rx/rx.c -lm -o $(BD)/rx

rxconnect: rx/rxconnect.c
	gcc -g -c rx/rxconnect.c -o $(BD)/rxconnect.o

# OTHERS

utils: utils/utils.c mtwister/mtwister.c
	gcc -g -c utils/utils.c -o $(BD)/utils.o
	gcc -g -c mtwister/mtwister.c -o $(BD)/mtwister.o

clean:
	rm -rf build/*.o build/rx build/tx

.PHONY: all clean