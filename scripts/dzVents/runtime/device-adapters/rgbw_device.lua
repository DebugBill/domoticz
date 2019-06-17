return {

	baseType = 'device',

	name = 'RGB(W) device adapter',

	matches = function (device, adapterManager)
		local res = device.deviceType == 'Color Switch'
		adapterManager.addDummyMethod(device, 'setKelvin')
		adapterManager.addDummyMethod(device, 'setWhiteMode')
		adapterManager.addDummyMethod(device, 'increaseBrightness')
		adapterManager.addDummyMethod(device, 'decreaseBrightness')
		adapterManager.addDummyMethod(device, 'setNightMode')
		adapterManager.addDummyMethod(device, 'setRGB')
		adapterManager.addDummyMethod(device, 'setDiscoMode')

		return res
	end,

	process = function (device, data, domoticz, utils, adapterManager)

<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/rgbw_device.lua
		-- from data: maxDimLevel

		function device.toggleSwitch()
			local current, inv
			if (device.state ~= nil) then
				current = adapterManager.states[string.lower(device.state)]
				if (current ~= nil) then
					inv = current.inv
					if (inv ~= nil) then
						return TimedCommand(domoticz, device.name, inv)
					end
				end
=======
		if (device.deviceSubType == 'RGBWW') then
			function device.setKelvin(kelvin)
				local url
				url = domoticz.settings['Domoticz url'] ..
						'/json.htm?param=setkelvinlevel&type=command&idx=' .. device.id .. '&kelvin=' .. tonumber(kelvin)
				return domoticz.openURL(url)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/rgbw_device.lua
			end
		end

		function device.setWhiteMode()
			local url
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=whitelight&type=command&idx=' .. device.id
			return domoticz.openURL(url)
		end

<<<<<<< HEAD:scripts/dzVents/runtime/device-adapters/rgbw_device.lua
		function device.switchOn()
			return TimedCommand(domoticz, device.name, 'On')
		end

		function device.switchOff()
			return TimedCommand(domoticz, device.name, 'Off')
		end

		function device.dimTo(percentage)
			return TimedCommand(domoticz, device.name, 'Set Level ' .. tostring(percentage))
=======
		function device.increaseBrightness()
			local url
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=brightnessup&type=command&idx=' .. device.id
			return domoticz.openURL(url)
		end

		function device.decreaseBrightness()
			local url
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=brightnessdown&type=command&idx=' .. device.id
			return domoticz.openURL(url)
		end

		function device.setNightMode()
			local url
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=nightlight&type=command&idx=' .. device.id
			return domoticz.openURL(url)
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/device-adapters/rgbw_device.lua
		end

		function device.setDiscoMode(modeNum)
			if (type(modeNum) ~= 'number' or modeNum < 1 or modeNum > 9) then
				domoticz.log('Mode number needs to be a number from 1-9', utils.LOG_ERROR)
			end
			local url
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=discomodenum' .. tonumber(modeNum) .. '&type=command&idx=' .. device.id
			return domoticz.openURL(url)
		end

		function device.setRGB(r, g, b)
			if (not type(r) == 'number' or not type(b) == 'number' or not type(g) == number) then
				domoticz.log('RGB values need to be numbers from 0-255', utils.LOG_ERROR)
				return
			end

			if (r < 0 or r > 255 or b < 0 or b > 255 or g < 0 or g > 255) then
				domoticz.log('RGB values need to be numbers from 0-255', utils.LOG_ERROR)
				return
			end

			local h, s, b, isWhite = domoticz.utils.rgbToHSB(tonumber(r), tonumber(g), tonumber(b))
			url = domoticz.settings['Domoticz url'] ..
					'/json.htm?param=setcolbrightnessvalue&type=command&idx=' ..
					device.id ..
					'&hue=' .. tostring(h) ..
					'&brightness=' .. tostring(b) ..
					'&iswhite=' ..  tostring(isWhite)
			return domoticz.openURL(url)
		end
	end
}
