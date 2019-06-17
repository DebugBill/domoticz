define(['app'], function (app) {
	app.controller('UtilityController', ['$scope', '$rootScope', '$location', '$http', '$interval', 'permissions', function ($scope, $rootScope, $location, $http, $interval, permissions) {

		$scope.HasInitializedEditCustomSensorDialog = false;

		$.strPad = function (i, l, s) {
			var o = i.toString();
			if (!s) { s = '0'; }
			while (o.length < l) {
				o = s + o;
			}
			return o;
		};

		MakeFavorite = function (id, isfavorite) {
			if (!permissions.hasPermission("Admin")) {
				HideNotify();
				ShowNotify($.t('You do not have permission to do that!'), 2500, true);
				return;
			}

			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$.ajax({
				url: "json.htm?type=command&param=makefavorite&idx=" + id + "&isfavorite=" + isfavorite,
				async: false,
				dataType: 'json',
				success: function (data) {
					ShowUtilities();
				}
			});
		}

		ConfigureEditCustomSensorDialog = function () {
			if ($scope.HasInitializedEditCustomSensorDialog == true) {
				return;
			}
			$scope.HasInitializedEditCustomSensorDialog = true;
			$.ddData = [];
			//Get Custom icons
			$.ajax({
				url: "json.htm?type=custom_light_icons",
				async: false,
				dataType: 'json',
				success: function (data) {
					if (typeof data.result != 'undefined') {
						var totalItems = data.result.length;
						$.each(data.result, function (i, item) {
							var bSelected = false;
							if (i == 0) {
								bSelected = true;
							}
							var itext = item.text;
							var idescription = item.description;

							var img = "images/";
							if (item.idx == 0) {
								img += "Custom";
								itext = "Custom";
								idescription = "Custom Sensor";
							}
							else {
								img += item.imageSrc;
							}
							img += "48_On.png";
							$.ddData.push({ text: itext, value: item.idx, selected: bSelected, description: idescription, imageSrc: img });
						});
					}
				}
			});
		}

		EditUtilityDevice = function (idx, name, description) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$.devIdx = idx;
			$("#dialog-editutilitydevice #devicename").val(unescape(name));
			$("#dialog-editutilitydevice #devicedescription").val(unescape(description));
			$("#dialog-editutilitydevice").i18n();
			$("#dialog-editutilitydevice").dialog("open");
		}

		EditCustomSensorDevice = function (idx, name, description, customimage, sensortype, axislabel) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			ConfigureEditCustomSensorDialog();
			$.devIdx = idx;
			$.sensorType = sensortype;
			$("#dialog-editcustomsensordevice #devicename").val(unescape(name));
			$("#dialog-editcustomsensordevice #sensoraxis").val(unescape(axislabel));

			$("#dialog-editcustomsensordevice #devicedescription").val(unescape(description));

			$('#dialog-editcustomsensordevice #combosensoricon').ddslick({
				data: $.ddData,
				width: 260,
				height: 390,
				selectText: "Sensor Icon",
				imagePosition: "left"
			});
			//find our custom image index and select it
			$.each($.ddData, function (i, item) {
				if (item.value == customimage) {
					$('#dialog-editcustomsensordevice #combosensoricon').ddslick('select', { index: i });
				}
			});

			$("#dialog-editcustomsensordevice").i18n();
			$("#dialog-editcustomsensordevice").dialog("open");
		}

		EditDistanceDevice = function (idx, name, description, switchtype) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$.devIdx = idx;
			$("#dialog-editdistancedevice #devicename").val(unescape(name));
			$("#dialog-editdistancedevice #devicedescription").val(unescape(description));
			$("#dialog-editdistancedevice #combometertype").val(switchtype);
			$("#dialog-editdistancedevice").i18n();
			$("#dialog-editdistancedevice").dialog("open");
		}

		EditMeterDevice = function (idx, name, description, switchtype, meteroffset, valuequantity, valueunits) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$.devIdx = idx;
			$("#dialog-editmeterdevice #devicename").val(unescape(name));
			$("#dialog-editmeterdevice #devicedescription").val(unescape(description));
			$("#dialog-editmeterdevice #combometertype").val(switchtype);
			$("#dialog-editmeterdevice #meteroffset").val(meteroffset);
			$("#dialog-editmeterdevice #valuequantity").val(unescape(valuequantity));
			$("#dialog-editmeterdevice #valueunits").val(unescape(valueunits));
			$("#dialog-editmeterdevice #metertable #customcounter").hide();
			if (switchtype == 3) { //Counter
				$("#dialog-editmeterdevice #metertable #customcounter").show();
			}

			$("#dialog-editmeterdevice #combometertype").change(function () {
				$("#dialog-editmeterdevice #metertable #customcounter").hide();
				var meterType = $("#dialog-editmeterdevice #combometertype").val();
				if (meterType == 3) { //Counter
					if (($("#dialog-editmeterdevice #valuequantity").val() == "")
						&& ($("#dialog-editmeterdevice #valueunits").val() == "")) {
						$("#dialog-editmeterdevice #valuequantity").val("Count");
					}
					$("#dialog-editmeterdevice #metertable #customcounter").show();
				}
			});

			$("#dialog-editmeterdevice").i18n();
			$("#dialog-editmeterdevice").dialog("open");
		}

		EditEnergyDevice = function (idx, name, description, switchtype, devoptions) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$.devIdx = idx;
			$("#dialog-editenergydevice #devicename").val(unescape(name));
			$("#dialog-editenergydevice #devicedescription").val(unescape(description));
			$("#dialog-editenergydevice #combometertype").val(switchtype);

			$('#dialog-editenergydevice input:radio[name=devoptions][value="' + devoptions + '"]').attr('checked', true);
			$('#dialog-editenergydevice input:radio[name=devoptions][value="' + devoptions + '"]').prop('checked', true);
			$('#dialog-editenergydevice input:radio[name=devoptions][value="' + devoptions + '"]').trigger('change');
			$("#dialog-editenergydevice").i18n();
			$("#dialog-editenergydevice").dialog("open");
		}

		EditSetPoint = function (idx, name, description, setpoint, isprotected) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			HandleProtection(isprotected, function () {
				$.devIdx = idx;
				$("#dialog-editsetpointdevice #devicename").val(unescape(name));
				$("#dialog-editsetpointdevice #devicedescription").val(unescape(description));
				$('#dialog-editsetpointdevice #protected').prop('checked', (isprotected == true));
				$("#dialog-editsetpointdevice #setpoint").val(setpoint);
				$("#dialog-editsetpointdevice #tempunit").html($scope.config.TempSign);
				$("#dialog-editsetpointdevice").i18n();
				$("#dialog-editsetpointdevice").dialog("open");
			});
		}

		EditThermostatClock = function (idx, name, description, daytime, isprotected) {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			HandleProtection(isprotected, function () {
				var sarray = daytime.split(";");
				$.devIdx = idx;
				$("#dialog-editthermostatclockdevice #devicename").val(unescape(name));
				$("#dialog-editthermostatclockdevice #devicedescription").val(unescape(description));
				$('#dialog-editthermostatclockdevice #protected').prop('checked', (isprotected == true));
				$("#dialog-editthermostatclockdevice #comboclockday").val(parseInt(sarray[0]));
				$("#dialog-editthermostatclockdevice #clockhour").val(sarray[1]);
				$("#dialog-editthermostatclockdevice #clockminute").val(sarray[2]);
				$("#dialog-editthermostatclockdevice").i18n();
				$("#dialog-editthermostatclockdevice").dialog("open");
			});
		}

		EditThermostatMode = function (idx, name, description, actmode, modes, isprotected) {
			HandleProtection(isprotected, function () {
				var sarray = modes.split(";");
				$.devIdx = idx;
				$.isFan = false;
				$("#dialog-editthermostatmode #devicename").val(unescape(name));
				$("#dialog-editthermostatmode #devicedescription").val(unescape(description));
				$('#dialog-editthermostatmode #protected').prop('checked', (isprotected == true));
				//populate mode combo
				$("#dialog-editthermostatmode #combomode").html("");
				var ii = 0;
				while (ii < sarray.length - 1) {
					var option = $('<option />');
					option.attr('value', sarray[ii]).text(sarray[ii + 1]);
					$("#dialog-editthermostatmode #combomode").append(option);
					ii += 2;
				}

				$("#dialog-editthermostatmode #combomode").val(parseInt(actmode));
				$("#dialog-editthermostatmode").i18n();
				$("#dialog-editthermostatmode").dialog("open");
			});
		}
		EditThermostatFanMode = function (idx, name, description, actmode, modes, isprotected) {
			HandleProtection(isprotected, function () {
				var sarray = modes.split(";");
				$.devIdx = idx;
				$.isFan = true;
				$("#dialog-editthermostatmode #devicename").val(unescape(name));
				$("#dialog-editthermostatmode #devicedescription").val(unescape(description));
				$('#dialog-editthermostatmode #protected').prop('checked', (isprotected == true));
				//populate mode combo
				$("#dialog-editthermostatmode #combomode").html("");
				var ii = 0;
				while (ii < sarray.length - 1) {
					var option = $('<option />');
					option.attr('value', sarray[ii]).text(sarray[ii + 1]);
					$("#dialog-editthermostatmode #combomode").append(option);
					ii += 2;
				}

				$("#dialog-editthermostatmode #combomode").val(parseInt(actmode));
				$("#dialog-editthermostatmode").i18n();
				$("#dialog-editthermostatmode").dialog("open");
			});
		}

		AddUtilityDevice = function () {
			bootbox.alert($.t('Please use the devices tab for this.'));
		}

		RefreshUtilities = function () {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			var id = "";

			$.ajax({
				url: "json.htm?type=devices&filter=utility&used=true&order=[Order]&lastupdate=" + $.LastUpdateTime + "&plan=" + window.myglobals.LastPlanSelected,
				async: false,
				dataType: 'json',
				success: function (data) {
					if (typeof data.ServerTime != 'undefined') {
						$rootScope.SetTimeAndSun(data.Sunrise, data.Sunset, data.ServerTime);
					}

					if (typeof data.result != 'undefined') {
						if (typeof data.ActTime != 'undefined') {
							$.LastUpdateTime = parseInt(data.ActTime);
						}

						$.each(data.result, function (i, item) {
							id = "#utilitycontent #" + item.idx;
							var obj = $(id);
							if (typeof obj != 'undefined') {
								if ($(id + " #name").html() != item.Name) {
									$(id + " #name").html(item.Name);
								}
								var img = "";
								var status = "";
								var bigtext = "";

								if ((typeof item.Usage != 'undefined') && (typeof item.UsageDeliv == 'undefined')) {
									bigtext = item.Usage;
								}

								if (typeof item.Counter != 'undefined') {
<<<<<<< HEAD
									if ((item.SubType == "Gas") || (item.SubType == "RFXMeter counter") || (item.SubType == "Counter Incremental")) {
										status = item.Counter;
										bigtext = item.CounterToday;
									}
									else {
										status = item.Counter + ', ' + $.t("Today") + ': ' + item.CounterToday;
=======
									if (
										(item.SubType == "Gas") ||
										(item.SubType == "RFXMeter counter") ||
										(item.SubType == "Counter Incremental")
									) {
										bigtext = item.CounterToday;
									}
									else if (item.SubType == "Managed Counter") {
										bigtext = item.Counter;
										status = "";
									}
									if (
										(item.SubType == "RFXMeter counter") ||
										(item.SubType == "Counter Incremental")
									) {
										status = item.Counter;
									} else {
										status = $.t("Today") + ': ' + item.CounterToday + ', ' + item.Counter;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
									}
								}
								else if (item.Type == "Current") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if ((item.Type == "Energy") || (item.Type == "Current/Energy") || (item.Type == "Power") || (item.SubType == "kWh")) {
									status = item.Data;
									if (typeof item.CounterToday != 'undefined') {
										status += ', ' + $.t("Today") + ': ' + item.CounterToday;
									}
								}
								else if (item.SubType == "Percentage") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.Type == "Fan") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.Type == "Air Quality") {
									status = item.Data + " (" + item.Quality + ")";
									bigtext = item.Data;
								}
								else if (item.SubType == "Soil Moisture") {
									status = item.Data + " (" + item.Desc + ")";
									bigtext = item.Data;
								}
								else if (item.SubType == "Custom Sensor") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.SubType == "Leaf Wetness") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if ((item.SubType == "Voltage") || (item.SubType == "Current") || (item.SubType == "Distance") || (item.SubType == "A/D") || (item.SubType == "Pressure") || (item.SubType == "Sound Level")) {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.SubType == "Text") {
									status = item.Data;
								}
								else if (item.SubType == "Alert") {
									status = item.Data;
									var aLevel = item.Level;
									if (aLevel > 4) aLevel = 4;
									img = '<img src="images/Alert48_' + aLevel + '.png" height="48" width="48">';
								}
								else if (item.Type == "Lux") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.Type == "Weight") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if (item.Type == "Usage") {
									status = item.Data;
									bigtext = item.Data;
								}
								else if ((item.Type == "Thermostat") && (item.SubType == "SetPoint")) {
									status = item.Data + '\u00B0 ' + $scope.config.TempSign;
									bigtext = item.Data + '\u00B0 ' + $scope.config.TempSign;
								}
								else if (item.Type == "Radiator 1") {
									status = item.Data + '\u00B0 ' + $scope.config.TempSign;
									bigtext = item.Data + '\u00B0 ' + $scope.config.TempSign;
								}
								else if (item.SubType == "Thermostat Clock") {
									status = item.Data;
								}
								else if (item.SubType == "Thermostat Mode") {
									status = item.Data;
								}
								else if (item.SubType == "Thermostat Fan Mode") {
									status = item.Data;
								}
								else if (item.SubType == "Waterflow") {
									status = item.Data;
									bigtext = item.Data;
								}

								if (typeof item.Usage != 'undefined') {
									bigtext = item.Usage;
								}
								if (typeof item.CounterDeliv != 'undefined') {
									if (item.CounterDeliv != 0) {
										status += '<br>' + $.t("Return") + ': ' + item.CounterDeliv + ', ' + $.t("Today") + ': ' + item.CounterDelivToday;
										if (item.UsageDeliv.charAt(0) != 0) {
											if (parseInt(item.Usage) != 0) {
												bigtext += ', -' + item.UsageDeliv;
											}
											else {
												bigtext = '-' + item.UsageDeliv;
											}
										}
									}
								}

								var nbackcolor = "#D4E1EE";
								if (item.Protected == true) {
									nbackcolor = "#A4B1EE";
								}
								if (item.HaveTimeout == true) {
									nbackcolor = "#DF2D3A";
								}
								else {
									var BatteryLevel = parseInt(item.BatteryLevel);
									if (BatteryLevel != 255) {
										if (BatteryLevel <= 10) {
											nbackcolor = "#DDDF2D";
										}
									}
								}
								var obackcolor = rgb2hex($(id + " #name").css("background-color"));
								if (obackcolor != nbackcolor) {
									$(id + " #name").css("background-color", nbackcolor);
								}

								if ($(id + " #status").html() != status) {
									$(id + " #bigtext").html(bigtext);
									$(id + " #status").html(status);
								}
								if ($(id + " #bigtext").html() != bigtext) {
									$(id + " #bigtext").html(bigtext);
								}
								if ($(id + " #lastupdate").html() != item.LastUpdate) {
									$(id + " #lastupdate").html(item.LastUpdate);
								}
								if (img != "") {
									if ($(id + " #img").html() != img) {
										$(id + " #img").html(img);
									}
								}
								if ($scope.config.ShowUpdatedEffect == true) {
									$(id + " #name").effect("highlight", { color: '#EEFFEE' }, 1000);
								}
							}
						});
					}
				}
			});
			$scope.mytimer = $interval(function () {
				RefreshUtilities();
			}, 10000);
		}

		ShowUtilities = function () {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			$('#modal').show();

			var htmlcontent = '';
			var bShowRoomplan = false;
			$.RoomPlans = [];
			$.ajax({
				url: "json.htm?type=plans",
				async: false,
				dataType: 'json',
				success: function (data) {
					if (typeof data.result != 'undefined') {
						var totalItems = data.result.length;
						if (totalItems > 0) {
							bShowRoomplan = true;
							//				if (window.myglobals.ismobile==true) {
							//				bShowRoomplan=false;
							//		}
							if (bShowRoomplan == true) {
								$.each(data.result, function (i, item) {
									$.RoomPlans.push({
										idx: item.idx,
										name: item.Name
									});
								});
							}
						}
					}
				}
			});

			var bHaveAddedDevider = false;

			var tophtm = "";
			if ($.RoomPlans.length == 0) {
				tophtm +=
					'\t<table border="0" cellpadding="0" cellspacing="0" width="100%">\n' +
					'\t<tr>\n' +
					'\t  <td align="left" valign="top" id="timesun"></td>\n' +
					'\t</tr>\n' +
					'\t</table>\n';
			}
			else {
				tophtm +=
					'\t<table border="0" cellpadding="0" cellspacing="0" width="100%">\n' +
					'\t<tr>\n' +
					'\t  <td align="left" valign="top" id="timesun"></td>\n' +
					'<td align="right" valign="top">' +
					'<span data-i18n="Room">Room</span>:&nbsp;<select id="comboroom" style="width:160px" class="combobox ui-corner-all">' +
					'<option value="0" data-i18n="All">All</option>' +
					'</select>' +
					'</td>' +
					'\t</tr>\n' +
					'\t</table>\n';
			}

			var i = 0;
			$.ajax({
				url: "json.htm?type=devices&filter=utility&used=true&order=[Order]&plan=" + window.myglobals.LastPlanSelected,
				async: false,
				dataType: 'json',
				success: function (data) {
					if (typeof data.result != 'undefined') {
						if (typeof data.ActTime != 'undefined') {
							$.LastUpdateTime = parseInt(data.ActTime);
						}
						$.each(data.result, function (i, item) {
							if (i % 3 == 0) {
								//add devider
								if (bHaveAddedDevider == true) {
									//close previous devider
									htmlcontent += '</div>\n';
								}
								htmlcontent += '<div class="row divider">\n';
								bHaveAddedDevider = true;
							}

							var xhtm =
								'\t<div class="span4" id="' + item.idx + '">\n' +
								'\t  <section>\n' +
								'\t    <table id="itemtable" border="0" cellpadding="0" cellspacing="0">\n' +
								'\t    <tr>\n';
							var nbackcolor = "#D4E1EE";
							if (item.Protected == true) {
								nbackcolor = "#A4B1EE";
							}
							if (item.HaveTimeout == true) {
								nbackcolor = "#DF2D3A";
							}
							else {
								var BatteryLevel = parseInt(item.BatteryLevel);
								if (BatteryLevel != 255) {
									if (BatteryLevel <= 10) {
										nbackcolor = "#DDDF2D";
									}
								}
							}
							xhtm += '\t      <td id="name" style="background-color: ' + nbackcolor + ';">' + item.Name + '</td>\n';
							xhtm += '\t      <td id="bigtext">';
							if ((typeof item.Usage != 'undefined') && (typeof item.UsageDeliv == 'undefined')) {
								xhtm += item.Usage;
							}
							else if ((typeof item.Usage != 'undefined') && (typeof item.UsageDeliv != 'undefined')) {
								if ((item.UsageDeliv.charAt(0) == 0) || (parseInt(item.Usage) != 0)) {
									xhtm += item.Usage;
								}
								if (item.UsageDeliv.charAt(0) != 0) {
									xhtm += '-' + item.UsageDeliv;
								}
							}
							else if ((item.SubType == "Gas") || (item.SubType == "RFXMeter counter") || (item.SubType == "Counter Incremental")) {
								xhtm += item.CounterToday;
							}
							else if (item.SubType == "Managed Counter") {
								xhtm += item.Counter;
							}
							else if (item.Type == "Air Quality") {
								xhtm += item.Data;
							}
							else if (item.SubType == "Custom Sensor") {
								xhtm += item.Data;
							}
							else if (item.Type == "Current") {
								xhtm += item.Data;
							}
							else if (item.SubType == "Percentage") {
								xhtm += item.Data;
							}
							else if (item.Type == "Fan") {
								xhtm += item.Data;
							}
							else if (item.SubType == "Soil Moisture") {
								xhtm += item.Data;
							}
							else if (item.SubType == "Leaf Wetness") {
								xhtm += item.Data;
							}
							else if ((item.SubType == "Voltage") || (item.SubType == "Current") || (item.SubType == "Distance") || (item.SubType == "A/D") || (item.SubType == "Pressure") || (item.SubType == "Sound Level")) {
								xhtm += item.Data;
							}
							else if (item.Type == "Lux") {
								xhtm += item.Data;
							}
							else if (item.Type == "Weight") {
								xhtm += item.Data;
							}
							else if (item.Type == "Usage") {
								xhtm += item.Data;
							}
							else if (item.Type == "Thermostat") {
								xhtm += item.Data + '\u00B0 ' + $scope.config.TempSign;
							}
							else if (item.SubType == "Waterflow") {
								xhtm += item.Data;
							}
							xhtm += '</td>\n';
							xhtm += '\t      <td id="img"><img src="images/';
							var status = "";
							if (typeof item.Counter != 'undefined') {
								if ((item.Type == "RFXMeter") || (item.Type == "YouLess Meter") || (item.SubType == "Counter Incremental") || (item.SubType == "Managed Counter")) {
									if (item.SwitchTypeVal == 1) {
										xhtm += 'Gas48.png" height="48" width="48"></td>\n';
									}
									else if (item.SwitchTypeVal == 2) {
										xhtm += 'Water48.png" height="48" width="48"></td>\n';
									}
									else if (item.SwitchTypeVal == 3) {
										xhtm += 'Counter48.png" height="48" width="48"></td>\n';
									}
									else if (item.SwitchTypeVal == 4) {
										xhtm += 'PV48.png" height="48" width="48"></td>\n';
									}
									else {
										xhtm += 'Counter48.png" height="48" width="48"></td>\n';
									}
								}
								else {
									if (item.SubType == "Gas") {
										xhtm += 'Gas48.png" height="48" width="48"></td>\n';
									}
									else {
										xhtm += 'Counter48.png" height="48" width="48"></td>\n';
									}
								}
								if (
									(item.SubType == "Gas") ||
									(item.SubType == "RFXMeter counter") ||
									(item.SubType == "Counter Incremental")
								) {
									status = item.Counter;
								}
<<<<<<< HEAD
								else {
									status = item.Counter + ', ' + $.t("Today") + ': ' + item.CounterToday;
=======
								else if (item.SubType != "Managed Counter") {
									status = $.t("Today") + ': ' + item.CounterToday + ', ' + item.Counter;
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
								}
							}
							else if (item.Type == "Current") {
								xhtm += 'current48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if ((item.Type == "Energy") || (item.Type == "Current/Energy") || (item.Type == "Power") || (item.SubType == "kWh")) {
								if (((item.Type == "Energy") || (item.SubType == "kWh")) && (item.SwitchTypeVal == 4)) {
									xhtm += 'PV48.png" height="48" width="48"></td>\n';
								}
								else {
									xhtm += 'current48.png" height="48" width="48"></td>\n';
								}
								status = item.Data;
								if (typeof item.CounterToday != 'undefined') {
									status += ', ' + $.t("Today") + ': ' + item.CounterToday;
								}
							}
							else if (item.Type == "Air Quality") {
								xhtm += 'air48.png" height="48" width="48"></td>\n';
								status = item.Data + " (" + item.Quality + ")";
							}
							else if (item.SubType == "Custom Sensor") {
								xhtm += item.Image + '48_On.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Soil Moisture") {
								xhtm += 'moisture48.png" height="48" width="48"></td>\n';
								status = item.Data + " (" + item.Desc + ")";
							}
							else if (item.SubType == "Percentage") {
								xhtm += 'Percentage48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Fan") {
								xhtm += 'Fan48_On.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Leaf Wetness") {
								xhtm += 'leaf48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Distance") {
								xhtm += 'visibility48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if ((item.SubType == "Voltage") || (item.SubType == "Current") || (item.SubType == "A/D")) {
								xhtm += 'current48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Text") {
								xhtm += 'text48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Alert") {
								xhtm += 'Alert48_' + item.Level + '.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Pressure") {
								xhtm += 'gauge48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.Type == "Lux") {
								xhtm += 'lux48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.Type == "Weight") {
								xhtm += 'scale48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.Type == "Usage") {
								xhtm += 'current48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (((item.Type == "Thermostat") && (item.SubType == "SetPoint")) || (item.Type == "Radiator 1")) {
								xhtm += 'override.png" class="lcursor" onclick="ShowSetpointPopup(event, ' + item.idx + ', RefreshUtilities, ' + item.Protected + ', ' + item.Data + ');" height="48" width="48" ></td>\n';
								status = item.Data + '\u00B0 ' + $scope.config.TempSign;
							}
							else if (item.SubType == "Thermostat Clock") {
								xhtm += 'clock48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Thermostat Mode") {
								xhtm += 'mode48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Thermostat Fan Mode") {
								xhtm += 'mode48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Sound Level") {
								xhtm += 'Speaker48_On.png" class="lcursor" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'' + item.SubType + '\');" height="48" width="48"></td>\n';
								status = item.Data;
							}
							else if (item.SubType == "Waterflow") {
								xhtm += 'moisture48.png" height="48" width="48"></td>\n';
								status = item.Data;
							}
							if (typeof item.CounterDeliv != 'undefined') {
								if (item.CounterDeliv != 0) {
									status += '<br>' + $.t("Return") + ': ' + item.CounterDeliv + ', ' + $.t("Today") + ': ' + item.CounterDelivToday;
								}
							}
							xhtm +=
								'\t      <td id="status">' + status + '</td>\n' +
								'\t      <td id="lastupdate">' + item.LastUpdate + '</td>\n' +
								'\t      <td id="type">' + item.Type + ', ' + item.SubType + '</td>\n' +
								'\t      <td class="options">';
							if (item.Favorite == 0) {
								xhtm +=
									'<img src="images/nofavorite.png" title="' + $.t('Add to Dashboard') + '" onclick="MakeFavorite(' + item.idx + ',1);" class="lcursor">&nbsp;&nbsp;&nbsp;&nbsp;';
							}
							else {
								xhtm +=
									'<img src="images/favorite.png" title="' + $.t('Remove from Dashboard') + '" onclick="MakeFavorite(' + item.idx + ',0);" class="lcursor">&nbsp;&nbsp;&nbsp;&nbsp;';
							}

							if (typeof item.Counter != 'undefined') {
								if ((item.Type == "P1 Smart Meter") && (item.SubType == "Energy")) {
									xhtm += '<a class="btnsmall" onclick="ShowSmartLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\', ' + item.SwitchTypeVal + ');" data-i18n="Log">Log</a> ';
								}
								else if ((item.Type == "YouLess Meter") && (item.SwitchTypeVal == 0 || item.SwitchTypeVal == 4)) {
									xhtm += '<a class="btnsmall" onclick="ShowCounterLogSpline(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\', ' + item.SwitchTypeVal + ');" data-i18n="Log">Log</a> ';
								}
								else {
									xhtm += '<a class="btnsmall" onclick="ShowCounterLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\', ' + item.SwitchTypeVal + ');" data-i18n="Log">Log</a> ';
								}
								if (permissions.hasPermission("Admin")) {
									if (item.Type == "P1 Smart Meter") {
										xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
									}
									else {
										xhtm += '<a class="btnsmall" onclick="EditMeterDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', ' + item.SwitchTypeVal + ',' + item.AddjValue + ',\'' + escape(item.ValueQuantity) + '\',\'' + escape(item.ValueUnits) + '\');" data-i18n="Edit">Edit</a> ';
									}
								}
							}
							else if (item.Type == "Air Quality") {
								xhtm += '<a class="btnsmall" onclick="ShowAirQualityLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.SubType == "Custom Sensor") {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.SensorUnit) + '\', \'' + item.SubType + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditCustomSensorDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', ' + item.CustomImage + ', ' + item.SensorType + ', \'' + escape(item.SensorUnit) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.SubType == "Percentage") {
								xhtm += '<a class="btnsmall" onclick="ShowPercentageLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.Type == "Fan") {
								xhtm += '<a class="btnsmall" onclick="ShowFanLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.SubType == "Soil Moisture") || (item.SubType == "Leaf Wetness") || (item.SubType == "Waterflow")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'' + item.SubType + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.Type == "Lux") {
								xhtm += '<a class="btnsmall" onclick="ShowLuxLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.Type == "Weight") {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',\'' + item.Type + '\', \'' + item.SubType + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.Type == "Usage") {
								xhtm += '<a class="btnsmall" onclick="ShowUsageLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "Energy") || (item.SubType == "kWh") || (item.Type == "Power")) {
								xhtm += '<a class="btnsmall" onclick="ShowCounterLogSpline(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\', ' + item.SwitchTypeVal + ');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									if ((item.Type == "Energy") || (item.SubType == "kWh")) {
										if (item.Options == "") { item.Options = "0" }
										xhtm += '<a class="btnsmall" onclick="EditEnergyDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', '
										xhtm += item.SwitchTypeVal + ',' + item.Options + ');" data-i18n="Edit">Edit</a> ';
									} else {
										xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
									}
								}
							}
							else if ((item.Type == "Current") || (item.Type == "Current/Energy")) {
								xhtm += '<a class="btnsmall" onclick="ShowCurrentLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\', ' + item.displaytype + ');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "Thermostat") && (item.SubType == "SetPoint")) {
								if (permissions.hasPermission("Admin")) {
<<<<<<< HEAD
									xhtm += '<a class="btnsmall" onclick="ShowTempLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
=======
									var timerLink = '#/Devices/'+item.idx+'/Timers';
									var logLink = '#/Devices/'+item.idx+'/TemperatureLog';

									xhtm += '<a class="btnsmall" href="' + logLink +'" data-i18n="Log">Log</a> ';
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
									xhtm += '<a class="btnsmall" onclick="EditSetPoint(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', ' + item.SetPoint + ',' + item.Protected + ');" data-i18n="Edit">Edit</a> ';
									if (item.Timers == "true") {
										xhtm += '<a class="btnsmall-sel" href="' + timerLink + '" data-i18n="Timers">Timers</a> ';
									}
									else {
										xhtm += '<a class="btnsmall" href="' + timerLink + '" data-i18n="Timers">Timers</a> ';
									}
								}
							}
							else if (item.Type == "Radiator 1") {
								if (permissions.hasPermission("Admin")) {
<<<<<<< HEAD
									xhtm += '<a class="btnsmall" onclick="ShowTempLog(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\');" data-i18n="Log">Log</a> ';
=======
									var timerLink = '#/Devices/'+item.idx+'/Timers';
									var logLink = '#/Devices/'+item.idx+'/TemperatureLog';

									xhtm += '<a class="btnsmall" href="' + logLink +'" data-i18n="Log">Log</a> ';
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1
									xhtm += '<a class="btnsmall" onclick="EditSetPoint(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', ' + item.SetPoint + ',' + item.Protected + ');" data-i18n="Edit">Edit</a> ';
									if (item.Timers == "true") {
										xhtm += '<a class="btnsmall-sel" href="' + timerLink + '" data-i18n="Timers">Timers</a> ';
									}
									else {
										xhtm += '<a class="btnsmall" href="' + timerLink + '" data-i18n="Timers">Timers</a> ';
									}
								}
							}
							else if (item.SubType == "Text") {
                                var logLink = '#/Devices/'+item.idx+'/TextLog';

								xhtm += '<a class="btnsmall" href="' + logLink + '" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.SubType == "Thermostat Clock") {
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditThermostatClock(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', \'' + item.DayTime + '\',' + item.Protected + ');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.SubType == "Thermostat Mode") {
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditThermostatMode(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', \'' + item.Mode + '\', \'' + item.Modes + '\',' + item.Protected + ');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if (item.SubType == "Thermostat Fan Mode") {
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditThermostatFanMode(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\', \'' + item.Mode + '\', \'' + item.Modes + '\',' + item.Protected + ');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Voltage")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'VoltageGeneral\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Distance")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'DistanceGeneral\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditDistanceDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\',' + item.SwitchTypeVal + ');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Current")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'CurrentGeneral\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Pressure")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'Pressure\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.SubType == "Voltage") || (item.SubType == "Current") || (item.SubType == "A/D")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'' + item.SubType + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Sound Level")) {
								xhtm += '<a class="btnsmall" onclick="ShowGeneralGraph(\'#utilitycontent\',\'ShowUtilities\',' + item.idx + ',\'' + escape(item.Name) + '\',' + item.SwitchTypeVal + ', \'' + item.SubType + '\');" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else if ((item.Type == "General") && (item.SubType == "Alert")) {
                                var logLink = '#/Devices/'+item.idx+'/TextLog';

								xhtm += '<a class="btnsmall" href="' + logLink + '" data-i18n="Log">Log</a> ';
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							else {
								if (permissions.hasPermission("Admin")) {
									xhtm += '<a class="btnsmall" onclick="EditUtilityDevice(' + item.idx + ',\'' + escape(item.Name) + '\',\'' + escape(item.Description) + '\');" data-i18n="Edit">Edit</a> ';
								}
							}
							if (item.ShowNotifications == true) {
								if (permissions.hasPermission("Admin")) {
                                    var notificationLink = '#/Devices/'+item.idx+'/Notifications';

									if (item.Notifications == "true")
										xhtm += '<a class="btnsmall-sel" href="' + notificationLink + '" data-i18n="Notifications">Notifications</a>';
									else
										xhtm += '<a class="btnsmall" href="' + notificationLink + '" data-i18n="Notifications">Notifications</a>';
								}
							}
							xhtm +=
								'</td>\n' +
								'\t    </tr>\n' +
								'\t    </table>\n' +
								'\t  </section>\n' +
								'\t</div>\n';
							htmlcontent += xhtm;
						});
					}
				}
			});
			if (bHaveAddedDevider == true) {
				//close previous devider
				htmlcontent += '</div>\n';
			}
			if (htmlcontent == '') {
				htmlcontent = '<h2>' + $.t('No Utility sensors found or added in the system...') + '</h2>';
			}
			$('#modal').hide();
			$('#utilitycontent').html(tophtm + htmlcontent);
			$('#utilitycontent').i18n();
			if (bShowRoomplan == true) {
				$.each($.RoomPlans, function (i, item) {
					var option = $('<option />');
					option.attr('value', item.idx).text(item.name);
					$("#utilitycontent #comboroom").append(option);
				});
				if (typeof window.myglobals.LastPlanSelected != 'undefined') {
					$("#utilitycontent #comboroom").val(window.myglobals.LastPlanSelected);
				}
				$("#utilitycontent #comboroom").change(function () {
					var idx = $("#utilitycontent #comboroom option:selected").val();
					window.myglobals.LastPlanSelected = idx;
					ShowUtilities();
				});
			}
			if ($scope.config.AllowWidgetOrdering == true) {
				if (permissions.hasPermission("Admin")) {
					if (window.myglobals.ismobileint == false) {
						$("#utilitycontent .span4").draggable({
							drag: function () {
								if (typeof $scope.mytimer != 'undefined') {
									$interval.cancel($scope.mytimer);
									$scope.mytimer = undefined;
								}
								$.devIdx = $(this).attr("id");
								$(this).css("z-index", 2);
							},
							revert: true
						});
						$("#utilitycontent .span4").droppable({
							drop: function () {
								var myid = $(this).attr("id");
								var roomid = $("#utilitycontent #comboroom option:selected").val();
								if (typeof roomid == 'undefined') {
									roomid = 0;
								}
								$.ajax({
									url: "json.htm?type=command&param=switchdeviceorder&idx1=" + myid + "&idx2=" + $.devIdx + "&roomid=" + roomid,
									async: false,
									dataType: 'json',
									success: function (data) {
										ShowUtilities();
									}
								});
							}
						});
					}
				}
			}
			$rootScope.RefreshTimeAndSun();
			$scope.mytimer = $interval(function () {
				RefreshUtilities();
			}, 10000);
			return false;
		}

		init();

		function init() {
			//global var
			$.devIdx = 0;
			$.LastUpdateTime = parseInt(0);

			$.myglobals = {
				TimerTypesStr: [],
				OccurenceStr: [],
				MonthStr: [],
				WeekdayStr: [],
				SelectedTimerIdx: 0
			};

			$scope.MakeGlobalConfig();

			$('#timerparamstable #combotype > option').each(function () {
				$.myglobals.TimerTypesStr.push($(this).text());
			});
			$('#timerparamstable #occurence > option').each(function () {
				$.myglobals.OccurenceStr.push($(this).text());
			});
			$('#timerparamstable #months > option').each(function () {
				$.myglobals.MonthStr.push($(this).text());
			});
			$('#timerparamstable #weekdays > option').each(function () {
				$.myglobals.WeekdayStr.push($(this).text());
			});

			var dialog_editutilitydevice_buttons = {};

			dialog_editutilitydevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editutilitydevice #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editutilitydevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editutilitydevice #devicedescription").val()) +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editutilitydevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editutilitydevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editutilitydevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editutilitydevice_buttons[$.t("Replace")] = function () {
				$(this).dialog("close");
				ReplaceDevice($.devIdx, ShowUtilities);
			};
			dialog_editutilitydevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editutilitydevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editutilitydevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editcustomsensordevice_buttons = {};

			dialog_editcustomsensordevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editcustomsensordevice #devicename"), 2, 100);
				bValid = bValid && checkLength($("#dialog-editcustomsensordevice #sensoraxis"), 1, 100);
				if (bValid) {
					$(this).dialog("close");
					var soptions = $.sensorType + ";" + encodeURIComponent($("#dialog-editcustomsensordevice #sensoraxis").val());
					var cval = $('#dialog-editcustomsensordevice #combosensoricon').data('ddslick').selectedIndex;
					var CustomImage = $.ddData[cval].value;

					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editcustomsensordevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editcustomsensordevice #devicedescription").val()) +
						'&switchtype=0' +
						'&customimage=' + CustomImage +
						'&devoptions=' + encodeURIComponent(soptions) +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editcustomsensordevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editcustomsensordevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editcustomsensordevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editcustomsensordevice_buttons[$.t("Replace")] = function () {
				$(this).dialog("close");
				ReplaceDevice($.devIdx, ShowUtilities);
			};
			dialog_editcustomsensordevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editcustomsensordevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editcustomsensordevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editdistancedevice_buttons = {};
			dialog_editdistancedevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editdistancedevice #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editdistancedevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editdistancedevice #devicedescription").val()) +
						'&switchtype=' + $("#dialog-editdistancedevice #combometertype").val() +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editdistancedevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editdistancedevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editdistancedevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editdistancedevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editdistancedevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editdistancedevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			}).i18n();

			var dialog_editmeterdevice_buttons = {};
			dialog_editmeterdevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				var devOptionsParam = [], devOptions = [];
				var meterType = $("#dialog-editmeterdevice #combometertype").val();
				bValid = bValid && checkLength($("#dialog-editmeterdevice #devicename"), 2, 100);
				if (bValid) {
					var meteroffset = $("#dialog-editmeterdevice #meteroffset").val();
					if (meterType == 3) //Counter
					{
						devOptions.push("ValueQuantity:");
						devOptions.push($("#dialog-editmeterdevice #valuequantity").val());
						devOptions.push(";");
						devOptions.push("ValueUnits:");
						devOptions.push($("#dialog-editmeterdevice #valueunits").val());
						devOptions.push(";");
						devOptionsParam.push(devOptions.join(''));
					}
					$(this).dialog("close");
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editmeterdevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editmeterdevice #devicedescription").val()) +
						'&switchtype=' + meterType +
						'&addjvalue=' + meteroffset +
						'&used=true' +
						'&options=' + b64EncodeUnicode(devOptionsParam.join('')),
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editmeterdevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editmeterdevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editmeterdevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editmeterdevice_buttons[$.t("Replace")] = function () {
				$(this).dialog("close");
				ReplaceDevice($.devIdx, ShowUtilities);
			};
			dialog_editmeterdevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editmeterdevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editmeterdevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editenergydevice_buttons = {};
			dialog_editenergydevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editenergydevice #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editenergydevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editenergydevice #devicedescription").val()) +
						'&switchtype=' + $("#dialog-editenergydevice #combometertype").val() + '&devoptions=' + $("#dialog-editenergydevice input:radio[name=devoptions]:checked").val() +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editenergydevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editenergydevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editenergydevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editenergydevice_buttons[$.t("Replace")] = function () {
				$(this).dialog("close");
				ReplaceDevice($.devIdx, ShowUtilities);
			};

			dialog_editenergydevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editenergydevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editenergydevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editsetpointdevice_buttons = {};

			dialog_editsetpointdevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editsetpointdevice #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editsetpointdevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editsetpointdevice #devicedescription").val()) +
						'&setpoint=' + $("#dialog-editsetpointdevice #setpoint").val() +
						'&protected=' + $('#dialog-editsetpointdevice #protected').is(":checked") +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});

				}
			};
			dialog_editsetpointdevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editsetpointdevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editsetpointdevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editsetpointdevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editsetpointdevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editsetpointdevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editthermostatclockdevice_buttons = {};

			dialog_editthermostatclockdevice_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editthermostatclockdevice #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					bootbox.alert($.t('Setting the Clock is not finished yet!'));
					var daytimestr = $("#dialog-editthermostatclockdevice #comboclockday").val() + ";" + $("#dialog-editthermostatclockdevice #clockhour").val() + ";" + $("#dialog-editthermostatclockdevice #clockminute").val();
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editthermostatclockdevice #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editthermostatclockdevice #devicedescription").val()) +
						'&clock=' + encodeURIComponent(daytimestr) +
						'&protected=' + $('#dialog-editthermostatclockdevice #protected').is(":checked") +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});
				}
			};
			dialog_editthermostatclockdevice_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editthermostatclockdevice #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editthermostatclockdevice #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editthermostatclockdevice_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editthermostatclockdevice").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editthermostatclockdevice_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			var dialog_editthermostatmode_buttons = {};

			dialog_editthermostatmode_buttons[$.t("Update")] = function () {
				var bValid = true;
				bValid = bValid && checkLength($("#dialog-editthermostatmode #devicename"), 2, 100);
				if (bValid) {
					$(this).dialog("close");
					var modestr = "";
					if ($.isFan == false) {
						modestr = "&tmode=" + $("#dialog-editthermostatmode #combomode").val();
					}
					else {
						modestr = "&fmode=" + $("#dialog-editthermostatmode #combomode").val();
					}
					$.ajax({
						url: "json.htm?type=setused&idx=" + $.devIdx +
						'&name=' + encodeURIComponent($("#dialog-editthermostatmode #devicename").val()) +
						'&description=' + encodeURIComponent($("#dialog-editthermostatmode #devicedescription").val()) +
						modestr +
						'&protected=' + $('#dialog-editthermostatmode #protected').is(":checked") +
						'&used=true',
						async: false,
						dataType: 'json',
						success: function (data) {
							ShowUtilities();
						}
					});
				}
			};
			dialog_editthermostatmode_buttons[$.t("Remove Device")] = function () {
				$(this).dialog("close");
				bootbox.confirm($.t("Are you sure to remove this Device?"), function (result) {
					if (result == true) {
						$.ajax({
							url: "json.htm?type=setused&idx=" + $.devIdx +
							'&name=' + encodeURIComponent($("#dialog-editthermostatmode #devicename").val()) +
							'&description=' + encodeURIComponent($("#dialog-editthermostatmode #devicedescription").val()) +
							'&used=false',
							async: false,
							dataType: 'json',
							success: function (data) {
								ShowUtilities();
							}
						});
					}
				});
			};
			dialog_editthermostatmode_buttons[$.t("Cancel")] = function () {
				$(this).dialog("close");
			};

			$("#dialog-editthermostatmode").dialog({
				autoOpen: false,
				width: 'auto',
				height: 'auto',
				modal: true,
				resizable: false,
				title: $.t("Edit Device"),
				buttons: dialog_editthermostatmode_buttons,
				close: function () {
					$(this).dialog("close");
				}
			});

			ShowUtilities();

			$("#dialog-editutilitydevice").keydown(function (event) {
				if (event.keyCode == 13) {
					$(this).siblings('.ui-dialog-buttonpane').find('button:eq(0)').trigger("click");
					return false;
				}
			});
			$("#dialog-editcustomsensordevice").keydown(function (event) {
				if (event.keyCode == 13) {
					$(this).siblings('.ui-dialog-buttonpane').find('button:eq(0)').trigger("click");
					return false;
				}
			});
		};
		$scope.$on('$destroy', function () {
			if (typeof $scope.mytimer != 'undefined') {
				$interval.cancel($scope.mytimer);
				$scope.mytimer = undefined;
			}
			var popup = $("#setpoint_popup");
			if (typeof popup != 'undefined') {
				popup.hide();
			}
		});
	}]);
});
