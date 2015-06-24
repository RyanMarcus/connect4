
# Copyright 2014 Ryan Marcus
#  
# This file is part of Connect4AI.
#  
# Connect4AI is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#  
# Connect4AI is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#  
# You should have received a copy of the GNU General Public License
# along with Connect4AI.  If not, see <http://www.gnu.org/licenses/>.



ui/conn4.js: conn4.c
	emcc -Wall -O1 -s TOTAL_MEMORY=67108864 -s EXPORTED_FUNCTIONS="['_startNewGame', '_playerMove', '_computerMove', '_isGameWon', '_isGameDraw', '_isEmpty', '_pieceAt']" conn4.c -o ui/conn4.js

.phony: clean

clean:
	rm -f ui/conn4.js


