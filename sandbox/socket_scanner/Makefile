CFLAGS = -Wall -Werror -Iinclude/
LFLAGS = -lm

sock_scan: sock_scan.o
	${CC} ${CFLAGS} -o sock_scan sock_scan.o ${LFLAGS}

clean:
	rm -f sock_scan *.o