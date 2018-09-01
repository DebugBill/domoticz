<<<<<<< HEAD:scripts/dzVents/runtime/dzVents.lua
local currentPath = globalvariables['script_path']
local triggerReason = globalvariables['script_reason']
=======
local TESTMODE = false
globalvariables['testmode'] = false
--globalvariables['dzVents_log_level'] = 4 --debug

if (_G.TESTMODE) then
	TESTMODE = false
	globalvariables['testmode'] = false
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/dzVents.lua

end

local scriptPath = globalvariables['script_path'] -- should be ${szUserDataFolder}/scripts/dzVents/
local runtimePath = globalvariables['runtime_path'] -- should be ${szStartupFolder}/dzVents/runtime/

_G.scriptsFolderPath = scriptPath .. 'scripts' -- global
_G.generatedScriptsFolderPath = scriptPath .. 'generated_scripts' -- global
_G.dataFolderPath = scriptPath .. 'data' -- global

<<<<<<< HEAD:scripts/dzVents/runtime/dzVents.lua
package.path = package.path .. ';' .. currentPath .. '?.lua'
package.path = package.path .. ';' .. currentPath .. 'runtime/?.lua'
package.path = package.path .. ';' .. currentPath .. 'runtime/device-adapters/?.lua'
package.path = package.path .. ';' .. currentPath .. 'dzVents/?.lua'
package.path = package.path .. ';' .. currentPath .. 'scripts/?.lua'
package.path = package.path .. ';' .. currentPath .. 'generated_scripts/?.lua'
package.path = package.path .. ';' .. currentPath .. 'data/?.lua'
=======
package.path = package.path .. ';' .. scriptPath .. '?.lua'
package.path = package.path .. ';' .. runtimePath .. '?.lua'
package.path = package.path .. ';' .. runtimePath .. 'device-adapters/?.lua'
package.path = package.path .. ';' .. scriptPath .. 'dzVents/?.lua'
package.path = package.path .. ';' .. scriptPath .. 'scripts/?.lua'
package.path = package.path .. ';' .. scriptPath .. '../lua/?.lua'
package.path = package.path .. ';' .. scriptPath .. 'scripts/modules/?.lua'
package.path = package.path .. ';' .. scriptPath .. '?.lua'
package.path = package.path .. ';' .. scriptPath .. 'generated_scripts/?.lua'
package.path = package.path .. ';' .. scriptPath .. 'data/?.lua'
package.path = package.path .. ';' .. scriptPath .. 'modules/?.lua'
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/dzVents.lua

local EventHelpers = require('EventHelpers')
local helpers = EventHelpers()
local utils = require('Utils')

<<<<<<< HEAD:scripts/dzVents/runtime/dzVents.lua
if (tonumber(globalvariables['dzVents_log_level']) == utils.LOG_DEBUG) then
	utils.log('Dumping domoticz data to ' .. currentPath .. '/domoticzData.lua', utils.LOG_DEBUG)
	local persistence = require('persistence')
	persistence.store(currentPath .. '/domoticzData.lua', domoticzData)
=======

if (tonumber(globalvariables['dzVents_log_level']) == utils.LOG_DEBUG or TESTMODE) then
	print('Debug: Dumping domoticz data to ' .. scriptPath .. 'domoticzData.lua')
	local persistence = require('persistence')
	persistence.store(scriptPath .. 'domoticzData.lua', domoticzData)

	local events, length = helpers.getEventSummary()
	if (length > 0) then
		print('Debug: dzVents version: 2.4.6')

		print('Debug: Event triggers:')
		for i, event in pairs(events) do
			print('Debug: ' .. event)
		end
	end

	if (globalvariables['isTimeEvent']) then
		print('Debug: Event triggers:')
		print('Debug: - Timer')
	end

>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/dzVents.lua
end
--

commandArray = {}

<<<<<<< HEAD:scripts/dzVents/runtime/dzVents.lua
utils.log('Event trigger type: ' .. triggerReason, utils.LOG_DEBUG)
=======
local isTimeEvent = globalvariables['isTimeEvent']
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/dzVents.lua

if (isTimeEvent) then
	commandArray = helpers.dispatchTimerEventsToScripts()
<<<<<<< HEAD:scripts/dzVents/runtime/dzVents.lua
elseif triggerReason == "device" then
	commandArray = helpers.dispatchDeviceEventsToScripts()
elseif triggerReason == "uservariable" then
	commandArray = helpers.dispatchVariableEventsToScripts()
elseif triggerReason == 'security' then
	commandArray = helpers.dispatchSecurityEventsToScripts()
else
	utils.log("Unknown trigger: ", triggerReason, utils.LOG_DEBUG)
end
=======
end

helpers.dispatchDeviceEventsToScripts()
helpers.dispatchVariableEventsToScripts()
helpers.dispatchSecurityEventsToScripts()
helpers.dispatchSceneGroupEventsToScripts()
helpers.dispatchHTTPResponseEventsToScripts()
commandArray = helpers.domoticz.commandArray

>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/dzVents.lua

return commandArray

