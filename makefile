OBJECTS= assassin.o shuffle.o

all: $(OBJECTS)
	gcc -o server $(OBJECTS)

assassin.o: assassin.c assassin.h shuffle.h
	gcc -c assassin.c

shuffle.o: shuffle.c shuffle.h
	gcc -c shuffle.c

clean:
	rm *.o *~