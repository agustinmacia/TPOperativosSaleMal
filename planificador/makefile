CFGLAGS=-Wall -g
LD = ld

LIBS = -lcommons -lreadline -lpthread -lparsi 

INCLUDEDIR    =include
UTILSDIR = ../utils

INC_PATH := 
INC_PATH += -I$(INCLUDEDIR)
INC_PATH += -I$(UTILSDIR)

PROGNAME=planificador

.PHONY: install

default:       
	make install --silent
install:
	$(CC) $(PROGNAME).c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c  $(LIBS) -o $(PROGNAME) $(CFLAGS) $(INC_PATH)
run:
	./$(PROGNAME)
valgrind:
	valgrind --leak-check=full ./$(PROGNAME)
clean:
	rm -f $(PROGNAME)
	rm -f tests
test:
	$(CC) $(CFLAGS) -o tests tests.c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c $(LIBS) -lcspecs -I$(INCLUDEDIR)/
	./tests
	rm tests
test-debug:
	$(CC) $(CFLAGS) tests.c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c -o tests $(LIBS) -lcspecs $(INC_PATH) -g
	gdb ./tests
	rm tests
test-valgrind:
	$(CC) $(CFLAGS) tests.c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c -o tests $(LIBS) -lcspecs $(INC_PATH) -g
	valgrind --leak-check=full ./tests
	rm tests