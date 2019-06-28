let app = angular.module('DCS', []).run(function ($rootScope) {
	$rootScope.event_log = new Event_log();
});

// remote server
//app.value("host", "http://lukboz.000webhostapp.com/");
// localhost
app.value("host", "");
