function ConnFourCtrl($scope) {

	$scope.worker = new Worker('conn4worker.js');


	$scope.board = [];
	$scope.look_ahead = 5;
	$scope.winner = false;
	$scope.draw = false;
	$scope.thinking = false;
	$scope.isOver = {};

	$scope.move = function(i) {
		if ($scope.thinking)
			return;

		if ($scope.winner)
			return;
		$scope.thinking = true;
		$scope.worker.postMessage({'cmd': 'move', 'val': i});	
	};


	$scope.$watch(function() { return $scope.look_ahead; }, function() {
		$scope.worker.postMessage({'cmd': "ahead", 'val': $scope.look_ahead});
	});

	$scope.worker.postMessage({'cmd': 'new'});
	$scope.worker.addEventListener('message', function(data) {
		var m = data.data;
		$scope.board = m['board'];

		$scope.winner = m['win'];
		$scope.draw = m['draw'];
		$scope.thinking = m['thinking'];
		$scope.$apply();
	});

	$scope.over = function(idx) {
		$scope.isOver[idx] = true;
	};

	$scope.leave = function(idx) {
		$scope.isOver[idx] = false;
	};

}
