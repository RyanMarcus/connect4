/*

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

*/

importScripts("conn4.js");

var startNewGame = Module.cwrap('startNewGame');
var playerMove = Module.cwrap('playerMove', '', ['number']);
var computerMove = Module.cwrap('computerMove', '', ['number']);
var isWin = Module.cwrap('isGameWon', 'number', []);
var isDraw = Module.cwrap('isGameDraw', 'number', []);
var isEmpty = Module.cwrap('isEmpty', 'number', ['number', 'number']);
var pieceAt = Module.cwrap('pieceAt', 'number', ['number', 'number']);


var movesAhead = 5;

function sendBoardUpdate(thinking) {
	var board = [];
	for (var y = 0; y < 8; y++) {
		var toAdd = [];
		for (var x = 0; x < 8; x++) {
			if (isEmpty(x, y)) {
				toAdd.push("empty");
			} else {
				toAdd.push("player" + pieceAt(x, y));
			}
		}
		board.unshift(toAdd);
	}
	
	var winner = isWin();
	var draw = isDraw();

	self.postMessage({'board': board, 'win': winner, 
			  'draw': draw, 'thinking': thinking});

}



self.addEventListener('message', function(data) {
	var m = data.data;
	if (m['cmd'] == "new") {
		startNewGame();
		sendBoardUpdate();
	} else if (m['cmd'] == "move") {
		playerMove(m['val']);
		sendBoardUpdate(true);
		computerMove(movesAhead);
		sendBoardUpdate(false);
	} else if (m['cmd'] == "ahead") {
		movesAhead = parseInt(m['val']);
	}
});
