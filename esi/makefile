CFGLAGS=-Wall -g
LD = ld

LIBS = -lcommons -lreadline -lparsi 

INCLUDEDIR    =include
UTILSDIR = ../utils

INC_PATH := 
INC_PATH += -I$(INCLUDEDIR)
INC_PATH += -I$(UTILSDIR)

PROGNAME=esi

.PHONY: install

default:       
	make install --silent
install:
	$(CC) $(PROGNAME).c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c  $(LIBS) -o $(PROGNAME) $(CFLAGS) $(INC_PATH) -g

run:
	./$(PROGNAME)
clean:
	rm -f $(PROGNAME)
	rm -f tests
test:
	$(CC) $(CFLAGS) -o tests tests.c $(INCLUDEDIR)/*.c $(UTILSDIR)/*.c $(LIBS) -lcspecs -I$(INCLUDEDIR)/
	./tests
	rm tests




