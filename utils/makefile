.PHONY: test

test:
	gcc *.c tests/tests.c -o test -lcommons -lcspecs
	./test
	rm test

debug:
	gcc *.c tests/tests.c -o test -lcommons -lcspecs -g

test-valgrind:
	gcc *.c tests/tests.c -o test -lcommons -lcspecs -g
	valgrind --leak-check=full ./test
	rm test	
