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
