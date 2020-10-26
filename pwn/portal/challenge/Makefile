all: chall

chall: src.c
	gcc src.c -fno-stack-protector -no-pie -o chall
	cp chall ../rawdistfiles/

clean: chall
	rm chall
	rm ../rawdistfiles/chall
