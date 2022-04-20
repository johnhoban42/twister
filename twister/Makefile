# Macros
BD = build
CC = gcc -g
CCO = gcc -g -c

# PRIMARY OPTIONS - make (all), make twister, make trace

all: trace twister

twister: tx rx

# TX

tx: utils tx/tx.c txconnect txcli
	$(CC) $(BD)/txconnect.o $(BD)/utils.o $(BD)/mtwister.o $(BD)/txcli.o tx/tx.c -lm -o $(BD)/tx

txconnect: tx/txconnect.c
	$(CCO) tx/txconnect.c -o $(BD)/txconnect.o

txcli: tx/cli.c
	$(CCO) tx/cli.c -o $(BD)/txcli.o

# RX

rx: utils rx/rx.c rxconnect rxcli
	$(CC) $(BD)/rxconnect.o $(BD)/utils.o $(BD)/mtwister.o $(BD)/rxcli.o rx/rx.c -lm -o $(BD)/rx

rxconnect: rx/rxconnect.c
	$(CCO) rx/rxconnect.c -o $(BD)/rxconnect.o

rxcli: rx/cli.c
	$(CCO) rx/cli.c -o $(BD)/rxcli.o

# TRACE

trace: utils trace/trace.c tracecli
	$(CC) $(BD)/mtwister.o $(BD)/utils.o $(BD)/tracecli.o trace/trace.c -o $(BD)/trace

tracecli: trace/cli.c
	$(CCO) trace/cli.c -o $(BD)/tracecli.o

# OTHERS

utils: utils/utils.c mtwister/mtwister.c
	$(CCO) utils/utils.c -o $(BD)/utils.o
	$(CCO) mtwister/mtwister.c -o $(BD)/mtwister.o

clean:
	rm -rf build/*.o build/rx build/tx

.PHONY: all clean