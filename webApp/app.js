let app = angular.module('DCS', []);
let minDateInput = document.getElementById('minDateInput');
let maxDateInput = document.getElementById('maxDateInput');
let tagsInput = document.getElementById('tagsInput');

function getParams(){
	return {
		min_date: minDateInput.value,
		max_date: maxDateInput.value,
		tags: tagsInput.value
	};
}

app.controller('selectData', function($scope, $http){
	$scope.sendRequest = function(){
		
		$http({
			url: "http://lukboz.000webhostapp.com/dcs.php",
			method: "GET",
			params: {
				date: new Date().getTime(),
				content: JSON.stringify( getParams() )
			}
		}).then(function(response){
			$scope.myData = response.data;
			$scope.errorInfo = "";
		}, function(response){
			console.log("Can't load data. Server may be unavialiable.");
			$scope.errorInfo = "Can't load data. Server may be unavialiable.";
		});
	}
	
	$scope.sortOrder = false;
	
	$scope.orderByTh = function(th) {
		$scope.activeTh = th;
		$scope.sortOrder = !$scope.sortOrder;
	}

});
