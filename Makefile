ui/conn4.js: conn4.c
	emcc -Wall -O3 -s TOTAL_MEMORY=67108864 -s EXPORTED_FUNCTIONS="['_startNewGame', '_playerMove', '_computerMove', '_isGameWon', '_isGameDraw', '_isEmpty', '_pieceAt']" conn4.c -o ui/conn4.js

.phony: clean

clean:
	rm -f ui/conn4.js


