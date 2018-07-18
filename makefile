install:
	make -C coordinador install
	make -C esi install
	make -C instancia install
	make -C planificador install
		

clean:
	make -C coordinador clean --silent
	make -C esi clean --silent
	make -C instancia clean --silent
	make -C planificador clean --silent
