let app = angular.module('DCS', []).run(function ($rootScope, logger) {
	$rootScope.event_log = logger.get_log();
});

app.config([
	'$compileProvider',
	function ($compileProvider) {
		$compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|ftp|mailto|data):/);
	}
]);

app.filter('replace_line_breaks', function () {
	return function (x) {
		return x.replace(new RegExp("<br/>", 'g'), "\n");
	};
});

// remote server
//app.value("host", "http://lukboz.000webhostapp.com/");
// localhost
app.value("host", "");

app.service('sender', function ($http, host) {
	this.send = function (destination, parameter_object) {
		return $http({
			url: host + destination,
			method: "POST",
			headers: { 'Content-Type': 'application/json; charset=UTF-8' },
			data: {
				date: new Date().getTime(),
				content: JSON.stringify(parameter_object)
			}
		});
	}
});

app.service('logger', function () {
	this.log = new Event_log();
	this.console = new Console_logger();

	this.get_console = function() {
		return this.console;
	}

	this.get_log = function() {
		return this.log;
	}
});
