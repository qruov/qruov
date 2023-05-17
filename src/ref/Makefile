CC=gcc
CFLAGS=@qruov_config.txt -O3 -fomit-frame-pointer -Wno-unused-result -I. -fopenmp # -DQRUOV_HASH_LEGACY -ggdb3 
LDFLAGS=-lcrypto -Wl,-Bstatic -lcrypto -Wl,-Bdynamic -lm
OBJS=Fql.o mgf.o PQCgenKAT_sign.o qruov.o rng.o sign.o matrix.o

.SUFFIXES:
.SUFFIXES: .rsp .req .diff .c .o .h

.PHONY: all clean

all: PQCgenKAT_sign
	./PQCgenKAT_sign

PQCgenKAT_sign: Makefile ${OBJS}
	${CC} ${OBJS} ${CFLAGS} ${LDFLAGS} -o $@

clean:
	rm -f PQCgenKAT_sign PQCsignKAT_*.req PQCsignKAT_*.rsp ${OBJS}
