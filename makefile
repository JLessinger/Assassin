OBJECTS= assassin.o shuffle.o

all: $(OBJECTS)
	gcc -Wall -o assassin.py $(OBJECTS)

assassin.o: assassin.c assassin.h shuffle.h
	gcc -c -Wall assassin.c

shuffle.o: shuffle.c shuffle.h
	gcc -c -Wall shuffle.c

clean:
	rm *.o *~