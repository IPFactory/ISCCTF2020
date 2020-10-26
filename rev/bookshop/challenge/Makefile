all: chall

chall: src.c
	gcc src.c -o bookshop
	strip --strip-all bookshop
	cp bookshop ../rawdistfiles/

clean: bookshop
	rm bookshop
	rm ../rawdistfiles/bookshop
