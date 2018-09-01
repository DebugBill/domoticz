local utils = require('Utils')
local Adapters = require('Adapters')
<<<<<<< HEAD:scripts/dzVents/runtime/Device.lua
=======
local TimedCommand = require('TimedCommand')
local TIMED_OPTIONS = require('TimedCommandOptions')

>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/Device.lua

local function Device(domoticz, data, dummyLogger)

	local self = {}
	local state
	local adapterManager = Adapters(dummyLogger)


<<<<<<< HEAD:scripts/dzVents/runtime/Device.lua
	function self.update(...)
		-- generic update method for non-switching devices
		-- each part of the update data can be passed as a separate argument e.g.
		-- device.update(12,34,54) will result in a command like
		-- ['UpdateDevice'] = '<id>|12|34|54'
		local command = self.id
		for i, v in ipairs({ ... }) do
			command = command .. '|' .. tostring(v)
		end

		domoticz.sendCommand('UpdateDevice', command)
=======
	function self.update(nValue, sValue, protected)
		local params = {
			idx = self.id,
			nValue = (nValue ~= nil and nValue ~= '')  and nValue or nil,
			sValue = (sValue ~= nil and sValue ~= '')  and tostring(sValue) or nil,
			_trigger = true,
			protected = protected ~= nil and protected or nil
		}
		return TimedCommand(domoticz, 'UpdateDevice', params, 'updatedevice')
>>>>>>> 98723b7da9467a49222b8a7ffaae276c5bc075c1:dzVents/runtime/Device.lua
	end

	function self.dump()
		domoticz.logDevice(self)
	end

	self['name'] = data.name
	self['id'] = data.id -- actually, this is the idx
	self['idx'] = data.id -- for completeness
	self['_data'] = data
	self['baseType'] = data.baseType

	if (_G.TESTMODE) then
		function self.getAdapterManager()
			return adapterManager
		end
	end

	-- process generic first
	adapterManager.genericAdapter.process(self, data, domoticz, utils, adapterManager)

	-- get a specific adapter for the current device
	local adapters = adapterManager.getDeviceAdapters(self)

	self._adapters = {}

	for i, adapter in pairs(adapters) do
		utils.log('Processing device-adapter for ' .. self.name .. ': ' .. adapter.name, utils.LOG_DEBUG)
		adapter.process(self, data, domoticz, utils, adapterManager)
		table.insert(self._adapters, adapter.name)
	end

	if (_G.TESTMODE) then
		function self._getUtilsInstance()
			return utils
		end
	end

	return self
end

return Device