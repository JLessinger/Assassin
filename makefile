OBJECTS= assassin.o shuffle.o

all: $(OBJECTS) test.cgi
	gcc -Wall -o assassin.cgi $(OBJECTS)

assassin.o: assassin.c assassin.h shuffle.h
	gcc -c -Wall assassin.c

shuffle.o: shuffle.c shuffle.h
	gcc -c -Wall shuffle.c

test.cgi: cgi-bin/test.c
	gcc -o test.cgi test.c
	mv test.cgi cgi-bin/test.cgi
clean:
	rm *.o *~