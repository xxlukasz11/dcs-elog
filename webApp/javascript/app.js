const ATTACHMENT_NAME_LIMIT = 15;

const app = angular.module('DCS_ELOG', []).run(function ($rootScope, logger) {
	$rootScope.event_log = logger.get_log();
});

app.config([
	'$compileProvider',
	function ($compileProvider) {
		// to allow data URI in link address
		$compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|ftp|mailto|data):/);
	}
]);

app.filter('replace_line_breaks', function () {
	return function (x) {
		return x.replace(new RegExp("<br/>", 'g'), "\n");
	};
});

app.filter('limit_attachment_name', function () {
	return function (name) {
		if (name.length > ATTACHMENT_NAME_LIMIT) {
			return name.substr(0, ATTACHMENT_NAME_LIMIT) + "...";
		}
		else {
			return name;
		}
	}
});


// address and location of php handlers
app.value("host", "");
app.value("php_scripts_location", "php_scripts/");

app.service('sender', function ($http, host, php_scripts_location) {
	this.send = function (destination, parameter_object) {
		return $http({
			url: host + php_scripts_location + destination,
			method: "POST",
			headers: { 'Content-Type': 'application/json; charset=UTF-8' },
			data: {
				date: new Date().getTime(),
				content: parameter_object
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
