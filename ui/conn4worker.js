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
	for (var y = 0; y < 6; y++) {
		var toAdd = [];
		for (var x = 0; x < 7; x++) {
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
