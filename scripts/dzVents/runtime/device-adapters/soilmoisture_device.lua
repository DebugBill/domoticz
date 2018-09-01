return {

	baseType = 'device',

	name = 'Soil Moisture device',

	matches = function (device, adapterManager)
		local res = (device.deviceSubType == "Soil Moisture")
		if (not res) then
			adapterManager.addDummyMethod(device, 'updateSoilMoisture')
		end
		return res
	end,

	process = function (device, data, domoticz, utils, adapterManager)

		device.moisture = data.data._nValue --cB centibar

		device['updateSoilMoisture'] = function (moisture)
<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/soilmoisture_device.lua
			local url = domoticz.settings['Domoticz url'] .. '/json.htm?type=command&param=udevice&idx=' .. device.id .. '&nvalue=' .. tonumber(moisture)
			domoticz.openURL(url)
=======
			return device.update(moisture, 0)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/soilmoisture_device.lua
		end

	end

}
