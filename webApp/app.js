let app = angular.module('DCS', []).run(function ($rootScope) {
	$rootScope.event_log = new Event_log();
});

function read_error_msg(response){
	let error_msg = response.data.message;
	if (error_msg)
		return error_msg;
	else
		return "Connection failed: Server may be unavialiable.";
}
