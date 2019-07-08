let app = angular.module('DCS', []).run(function ($rootScope) {
	$rootScope.event_log = new Event_log();
});

app.filter('replace_line_breaks', function () {
	return function (x) {
		return x.replace(new RegExp("<br/>", 'g'), "\n");
	};
});

// remote server
//app.value("host", "http://lukboz.000webhostapp.com/");
// localhost
app.value("host", "");
