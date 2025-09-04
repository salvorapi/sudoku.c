build: sudoku.c
	cc sudoku.c -O2 -Wall -W -o sudoku
clean: sudoku
	rm sudoku
