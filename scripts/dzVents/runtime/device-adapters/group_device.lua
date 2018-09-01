local TimedCommand = require('TimedCommand')

return {

	baseType = 'group',

	name = 'Group device adapter',

	matches = function (device, adapterManager)
		local res = (device.baseType == 'group')
		if (not res) then
			adapterManager.addDummyMethod(device, 'switchOn')
			adapterManager.addDummyMethod(device, 'switchOff')
			adapterManager.addDummyMethod(device, 'toggleGroup')
		end
		return res
	end,

	process = function (group, data, domoticz, utils, adapterManager)

		group.isGroup = true

		function group.toggleGroup()
			local current, inv
			if (group.state ~= nil) then
				current = adapterManager.states[string.lower(group.state)]
				if (current ~= nil) then
					inv = current.inv
					if (inv ~= nil) then
<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/group_device.lua
						return domoticz.switchGroup(device.name, inv)
=======
						return TimedCommand(domoticz, 'Group:' .. group.name, inv, 'device')
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/group_device.lua
					end
				end
			end
			return nil
		end

		function group.setState(newState)
			-- generic state update method
<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/group_device.lua
			return domoticz.switchGroup(device.name, newState)
		end

		function device.switchOn()
			return domoticz.switchGroup(device.name, 'On')
		end

		function device.switchOff()
			return domoticz.switchGroup(device.name, 'Off')
=======
			return TimedCommand(domoticz, 'Group:' .. group.name, newState, 'device', group.state)
		end

		function group.switchOn()
			return TimedCommand(domoticz, 'Group:' .. group.name, 'On', 'device', group.state)
		end

		function group.switchOff()
			return TimedCommand(domoticz, 'Group:' .. group.name, 'Off', 'device', group.state)
		end

		function group.devices()
			local subData = {}
			local ids = data.deviceIDs ~= nil and data.deviceIDs or {}

			for i, id in pairs(ids) do
				subData[i] = domoticz._getItemFromData('device', id)
			end

			return domoticz._setIterators({}, true, 'device', false , subData)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/group_device.lua
		end

	end

}
