return {

	baseType = 'device',

	name = 'Leaf wetness device adapter',

	matches = function (device, adapterManager)
		local res = (device.deviceSubType == 'Leaf Wetness')
		if (not res) then
			adapterManager.addDummyMethod(device, 'updateWetness')
		end
		return res
	end,

	process = function (device, data, domoticz, utils, adapterManager)

		device.wetness = data.data._nValue

		function device.updateWetness(wetness)
<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/leafwetness_device.lua
			-- /json.htm?type=command&param=udevice&idx=14&nvalue=10&svalue=0
			local url = domoticz.settings['Domoticz url'] .. '/json.htm?type=command&param=udevice&idx=' .. device.id .. '&nvalue=' .. tonumber(wetness)
			domoticz.openURL(url)
=======
			return device.update(wetness)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/leafwetness_device.lua
		end

	end

}
