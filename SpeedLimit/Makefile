run: main mainFast
	echo "Without gcc -O3" > memo.txt
	main >> memo.txt
	echo "With gcc -O3" >> memo.txt
	mainFast >> memo.txt
	cat memo.txt

main: Makefile main.o
	gcc -o main main.o

main.o: Makefile main.c
	gcc -c main.c

mainFast: Makefile mainFast.o
	gcc -O3 -o mainFast mainFast.o

mainFast.o: Makefile main.c
	gcc -O3 -c main.c && mv main.o mainFast.o
