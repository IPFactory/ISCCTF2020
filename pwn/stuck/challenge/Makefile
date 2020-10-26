all: chall

chall: src.c
	gcc -fno-stack-protector -no-pie src.c -o chall
	cp chall ../rawdistfiles

clean: chall
	rm chall
