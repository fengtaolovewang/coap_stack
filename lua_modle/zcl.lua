local aux_core = require( "aux_core" )

local module = {}


--Client/Server Directions
module.CLIENT_SERVER_DIR                    = 0x00
module.SERVER_CLIENT_DIR                    = 0x01

--Frame Types
module.PROFILE_CMD                     = 0x00
module.SPECIFIC_CMD                    = 0x01

-- Foundation Command IDs
module.READ                                   = 0x00
module.READ_RSP                               = 0x01
module.WRITE                                  = 0x02
module.WRITE_UNDIVIDED                        = 0x03
module.WRITE_RSP                              = 0x04
module.WRITE_NO_RSP                           = 0x05
module.CONFIG_REPORT                          = 0x06
module.CONFIG_REPORT_RSP                      = 0x07
module.READ_REPORT_CFG                        = 0x08
module.READ_REPORT_CFG_RSP                    = 0x09
module.REPORT                                 = 0x0a
module.DEFAULT_RSP                            = 0x0b
module.DISCOVER_ATTRS                         = 0x0c
module.DISCOVER_ATTRS_RSP                     = 0x0d
module.DISCOVER_CMDS_RECEIVED                 = 0x11
module.DISCOVER_CMDS_RECEIVED_RSP             = 0x12
module.DISCOVER_CMDS_GEN                      = 0x13
module.DISCOVER_CMDS_GEN_RSP                  = 0x14
module.DISCOVER_ATTRS_EXT                     = 0x15
module.DISCOVER_ATTRS_EXT_RSP                 = 0x16
--/*** Data Types ***/
module.NO_DATA                            = 0x00
module.DATA8                              = 0x08
module.DATA16                             = 0x09
module.DATA24                             = 0x0a
module.DATA32                             = 0x0b
module.DATA40                             = 0x0c
module.DATA48                             = 0x0d
module.DATA56                             = 0x0e
module.DATA64                             = 0x0f
module.BOOLEAN                            = 0x10
module.BITMAP8                            = 0x18
module.BITMAP16                           = 0x19
module.BITMAP24                           = 0x1a
module.BITMAP32                           = 0x1b
module.BITMAP40                           = 0x1c
module.BITMAP48                           = 0x1d
module.BITMAP56                           = 0x1e
module.BITMAP64                           = 0x1f
module.UINT8                              = 0x20
module.UINT16                             = 0x21
module.UINT24                             = 0x22
module.UINT32                             = 0x23
module.UINT40                             = 0x24
module.UINT48                             = 0x25
module.UINT56                             = 0x26
module.UINT64                             = 0x27
module.INT8                               = 0x28
module.INT16                              = 0x29
module.INT24                              = 0x2a
module.INT32                              = 0x2b
module.INT40                              = 0x2c
module.INT48                              = 0x2d
module.INT56                              = 0x2e
module.INT64                              = 0x2f
module.ENUM8                              = 0x30
module.ENUM16                             = 0x31
module.SEMI_PREC                          = 0x38
module.SINGLE_PREC                        = 0x39
module.DOUBLE_PREC                        = 0x3a
module.OCTET_STR                          = 0x41
module.CHAR_STR                           = 0x42
module.LONG_OCTET_STR                     = 0x43
module.LONG_CHAR_STR                      = 0x44
module.ARRAY                              = 0x48
module.STRUCT                             = 0x4c
module.SET                                = 0x50
module.BAG                                = 0x51
module.TOD                                = 0xe0
module.DATE                               = 0xe1
module.UTC                                = 0xe2
module.CLUSTER_ID                         = 0xe8
module.ATTR_ID                            = 0xe9
module.BAC_OID                            = 0xea
module.IEEE_ADDR                          = 0xf0
module.128_BIT_SEC_KEY                    = 0xf1
module.UNKNOWN                            = 0xff

function module.little_Uint16(low1,high2)
	if(aux_core.IsNumber(low1) and aux_core.IsNumber(high2) )then
		return high2*256 + low1
	end
end
function module.little_Uint32(byte1,byte2,byte3,byte4)
	if(aux_core.IsNumber(byte1) and aux_core.IsNumber(byte1) and aux_core.IsNumber(byte3) and aux_core.IsNumber(byte4) )then
		return byte4*256*65536+byte3*65536+byte2*256 + byte1
	end
end

function module.Hi16(n)
	return (n-n%256)/256
end

function module.Lo16(n)
	return n%256
end

function module.byteToHexC(n)
	local format = "%02X"
		return string.format( format, n )
end





--[[
local attribute_read_req=
{
	{
		attr_id=0x0001,
		status=0,
		data_type=21,
		payload="0102030405"
	},
}
]]--


local zcl_ProfileCmd_req=
{
--[[
local attribute_read_req=
{
	0x0000,
	0x0001,
	0x0003,
	0x0004
}
]]--
	[0]=function(read_req)
	
		local read_buff=""
		
		for i = 1, #read_req, 1 do
		
			local lo= module.Lo16(read_req[i])
			local hi= module.Hi16(read_req[i])
			
			read_buff =read_buff .. module.byteToHexC(lo) .. module.byteToHexC(hi)

		end
		return aux_core.HexStrToStr(read_buff)
	end,
--[[
local attribute_write_req=
{
	{
		attr_id=0x0001,
		data_type=21,
		payload="0102030405"
	},
}
]]--	
	[1]=function(write_req)
	
		local write_buff=""
		
		for i = 1, #write_req, 1 do
			read_buff[2*i-1]= module.Lo16(read_req[i])
			read_buff[2*i]=	module.Hi16(read_req[i])
		end
		return read_buff
	end,
}
--[[
local attribute_write_rsp=
{
	{
		attr_id=0x0002,
		status = 0x00,
	},
	{
		attr_id=0x0001,
		status = 0x00,
	},	
}
]]--

