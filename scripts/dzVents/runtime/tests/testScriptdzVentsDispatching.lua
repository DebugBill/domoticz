local _ = require 'lodash'
_G._ = require 'lodash'

local scriptPath = ''
--package.path = package.path .. ";../?.lua"
package.path = package.path .. ";../../?.lua;../../../scripts/lua/?.lua;"
package.path = package.path .. ";../?.lua;" .. scriptPath .. '/?.lua;../device-adapters/?.lua;'


describe('Event dispatching', function()

	setup(function()
		_G.TESTMODE = true
		_G.TESTTIME = 2017 .. '-' .. 6 .. '-' .. 13 .. ' ' .. 12 .. ':' .. 5 .. ':' .. 0

		_G.timeofday = {
			Daytime = 'dt',
			Nighttime = 'nt',
			SunriseInMinutes = 'sunrisemin',
			SunsetInMinutes = 'sunsetmin'
		}

<<<<<<< HEAD:scripts/dzVents/runtime/tests/testScriptdzVentsDispatching.lua
		_G.globalvariables = {
			['radix_separator'] = '.',
			['Security'] = 'sec',
			['domoticz_listening_port'] = '8080',
			['script_path'] = scriptPath
		}
=======
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/tests/testScriptdzVentsDispatching.lua

	end)

	before_each(function()
		_G.domoticzData = {
			[1] = {
				["deviceType"] = "Light/Switch",
				["deviceID"] = "00014051",
				["lastLevel"] = 61,
				["switchTypeValue"] = 0,
				["id"] = 1,
				["subType"] = "Switch",
				["timedOut"] = "false",
				["signalLevel"] = 255,
				["data"] = {
					["_state"] = "On",
					["hardwareType"] = "Dummy (Does nothing, use for virtual switches only)",
					["hardwareName"] = "dummy",
					["hardwareID"] = "2",
					["icon"] = "lightbulb",
					["maxDimLevel"] = "100",
				},
				["batteryLevel"] = 255,
				["description"] = "",
				["rawData"] = {
					[1] = "0",
				},
				["name"] = "onscript1",
				["baseType"] = "device",
				["changed"] = true,
				["lastUpdate"] = "2017-05-18 09:52:19",
				["switchType"] = "On/Off",
			},
			[2] = {
				["description"] = "",
				["name"] = "scene1",
				["id"] = 1,
				["data"] = {
					["_state"] = "On",
				},
				["lastUpdate"] = "2017-04-19 20:31:50",
				["baseType"] = "scene",
			},
			[3] = {
				["deviceType"] = "Light/Switch",
				["deviceID"] = "00014051",
				["lastLevel"] = 61,
				["switchTypeValue"] = 0,
				["id"] = 2,
				["subType"] = "Switch",
				["timedOut"] = "false",
				["signalLevel"] = 255,
				["data"] = {
					["_state"] = "On",
					["hardwareType"] = "Dummy (Does nothing, use for virtual switches only)",
					["hardwareName"] = "dummy",
					["hardwareID"] = "2",
					["icon"] = "lightbulb",
					["maxDimLevel"] = "100",
				},
				["batteryLevel"] = 255,
				["description"] = "",
				["rawData"] = {
					[1] = "0",
				},
				["name"] = "device1",
				["baseType"] = "device",
				["changed"] = false,
				["lastUpdate"] = "2017-05-18 09:52:19",
				["switchType"] = "On/Off",
			},
			[4] = {
				["id"] = 1;
				["lastUpdate"] = "2017-05-27 17:18:07";
				["variableType"] = "integer";
				["data"] = {
					["value"] = 3345;
				};
				["baseType"] = "uservariable";
				["changed"] = true;
				["name"] = "myVar1";
			}
		}
		_G.globalvariables = {
			['radix_separator'] = '.',
			['Security'] = 'sec',
			['domoticz_listening_port'] = '8080',
			['script_path'] = scriptPath,
			['runtime_path'] = '',
			['isTimeEvent'] = false,
			['currentTime'] = '2017-08-17 12:13:14.123'
		}
		_G.securityupdates = {
            'Armed Away'
        }

		_G.httpresponse = {{
			callback='trigger1',
			statusCode = 200
		}}

	end)

	after_each(function()
		_G.domoticzData = {}
		package.loaded['dzVents'] = nil
	end)


	it("should dispatch timer events", function()
		_G.commandArray = {}
		_G.globalvariables['isTimeEvent'] = true
		local main = require('dzVents')
		assert.is_same({
<<<<<<< HEAD:scripts/dzVents/runtime/tests/testScriptdzVentsDispatching.lua
			{ ["onscript1"] = "Off" },
			{ ["onscript1"] = "Set Level 10" },
			{ ["SendNotification"] = 'Yo##0#pushover##' }
		}, main)
	end)

=======
			{
				["onscript1"] = "Off"
			},
			{
				['Scene:scene 2'] = 'On'
			},
			{
				["SendNotification"] = 'Me#every minute timer every minute script_timer_single#0#pushover##'
		 	},
			{
				["Scene:scene 1"] = "On"
			},
			{
				["onscript1"]="Off"
			},
			{
				["onscript1"]="Set Level 10"
			},
			{
				["SendNotification"]="Yo##0#pushover##"
			},
			{
				["Variable"]={["value"]="10", ["idx"]=1, ["_trigger"]=true}
			},
			{
				["SendNotification"]="Me#Armed Away#0#pushover##"
			},
			{
				["Scene:myscene1"]="Off"
			},
			{
				["Group:mygroup1"]="On"
			},
			{
				["OpenURL"]={["URL"]="test", ["method"]="GET"}
			},


		}, main)
	end)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/tests/testScriptdzVentsDispatching.lua


	it('should dispatch non-timer events', function()
		_G.commandArray = {}

		local main = require('dzVents')
<<<<<<< HEAD:scripts/dzVents/runtime/tests/testScriptdzVentsDispatching.lua
		assert.is_same({
			{ ['OpenURL'] = 'http://127.0.0.1:8080/json.htm?type=command&param=updateuservariable&vname=myVar1&vtype=0&vvalue=10&idx=1' }
=======

		assert.is_same(
		{
			{
				["onscript1"]="Off"
			},
			{
				["onscript1"]="Set Level 10"
			},
			{
				["SendNotification"]="Yo##0#pushover##"
			},
			{
				["Variable"]={["value"]="10", ["idx"]=1, ["_trigger"]=true}
			},
			{
				["SendNotification"]="Me#Armed Away#0#pushover##"
			},
			{
				["Scene:myscene1"]="Off"
			},
			{
				["Group:mygroup1"]="On"
			},
			{
				["OpenURL"]={["URL"]="test", ["method"]="GET"}
			}
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/tests/testScriptdzVentsDispatching.lua
		}, main)
	end)

end)
