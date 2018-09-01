local TimedCommand = require('TimedCommand')

return {

	baseType = 'scene',

	name = 'Scene device adapter',

	matches = function(device, adapterManager)
		local res = (device.baseType == 'scene')
		if (not res) then
			adapterManager.addDummyMethod(device, 'switchOn')
			adapterManager.addDummyMethod(device, 'switchOff')
		end
		return res
	end,

	process = function(scene, data, domoticz, utils, adapterManager)

		scene.isScene = true

		function scene.setState(newState)
			-- generic state update method
			return domoticz.setScene(scene.name, newState)
		end

		function scene.switchOn()
			return TimedCommand(domoticz, 'Scene:' .. scene.name, 'On', 'device', scene.state)
		end

<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/scene_device.lua
		function device.switchOn()
			return domoticz.setScene(device.name, 'On')
		end

		function device.switchOff()
			return domoticz.setScene(device.name, 'Off')
=======
		function scene.switchOff()
			return TimedCommand(domoticz, 'Scene:' .. scene.name, 'Off', 'device', scene.state)
		end

		function scene.devices()
			local subData = {}
			local ids = data.deviceIDs ~= nil and data.deviceIDs or {}

			for i, id in pairs(ids) do
				subData[i] = domoticz._getItemFromData('device', id)
			end

			return domoticz._setIterators({}, true, 'device', false , subData)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/scene_device.lua
		end
	end

}
