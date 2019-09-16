--[[

     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |Ver| T |  TKL  |      Code     |          Message ID           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Token (if any, TKL bytes) ...                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |   Options (if any) ...                                        |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |1 1 1 1 1 1 1 1|    Payload (if any) ...                       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

--]]

local aux_core = require( "aux_core" )

local module = {}

module.MESSAGE_CON = 0
module.MESSAGE_NON = 1
module.MESSAGE_ACK = 2
module.MESSAGE_RST = 3

module.REQUEST_EMPTY  = 0
module.REQUEST_GET    = 1
module.REQUEST_POST   = 2
module.REQUEST_PUT    = 3
module.REQUEST_DELETE = 4

module.RESPONSE_OK                         = 0x40
module.RESPONSE_CREATED                    = 0x41
module.RESPONSE_DELETED                    = 0x42
module.RESPONSE_VALID                      = 0x43
module.RESPONSE_CHANGED                    = 0x44
module.RESPONSE_CONTENT                    = 0x45
module.RESPONSE_CONTINUE                   = 0x5f
module.RESPONSE_BAD_REQUEST                = 0x80
module.RESPONSE_UNAUTHORIZED               = 0x81
module.RESPONSE_BAD_OPTION                 = 0x82
module.RESPONSE_FORBIDDEN                  = 0x83
module.RESPONSE_NOT_FOUND                  = 0x84
module.RESPONSE_METHOD_NOT_ALLOWED         = 0x85
module.RESPONSE_NOT_ACCEPTABLE             = 0x86
module.RESPONSE_PRECONDITION_FAILED        = 0x8c
module.RESPONSE_REQUEST_ENTITY_TOO_LARGE   = 0x8d
module.RESPONSE_UNSUPPORTED_CONTENT_FORMAT = 0x8f
module.RESPONSE_INTERNAL_SERVER_ERROR      = 0xa0
module.RESPONSE_NOT_IMPLEMENTED            = 0xa1
module.RESPONSE_BAD_GATEWAY                = 0xa2
module.RESPONSE_SERVICE_UNAVAILABLE        = 0xa3
module.RESPONSE_GATEWAY_TIMEOUT            = 0xa4
module.RESPONSE_PROXYING_NOT_SUPPORTED     = 0xa5

module.OPTION_IF_MATCH       = 1	-- C, opaque, 0-8 B, (none)
module.OPTION_URI_HOST       = 3	-- C, String, 1-255 B, destination address
module.OPTION_ETAG           = 4	-- E, opaque, 1-8 B, (none)
module.OPTION_IF_NONE_MATCH  = 5	-- empty, 0 B, (none)
module.OPTION_OBSERVE        = 6	-- E, empty/uint, 0 B/0-3 B, (none)
module.OPTION_URI_PORT       = 7	-- C, uint, 0-2 B, destination port
module.OPTION_LOCATION_PATH  = 8	-- E, String, 0-255 B, -
module.OPTION_URI_PATH       = 11	-- C, String, 0-255 B, (none)
module.OPTION_CONTENT_FORMAT = 12	-- E, uint, 0-2 B, (none)
module.OPTION_MAXAGE         = 14	-- E, uint, 0--4 B, 60 Seconds
module.OPTION_URI_QUERY      = 15	-- C, String, 1-255 B, (none)
module.OPTION_ACCEPT         = 17	-- C, uint, 0-2 B, (none)
module.OPTION_LOCATION_QUERY = 20	-- E, String, 0-255 B, (none)
module.OPTION_BLOCK2         = 23	-- C, uint, 0--3 B, (none)
module.OPTION_BLOCK1         = 27	-- C, uint, 0--3 B, (none)
module.OPTION_PROXY_URI      = 35	-- C, String, 1-1034 B, (none)
module.OPTION_PROXY_SCHEME   = 39	-- C, String, 1-255 B, (none)
module.OPTION_SIZE1          = 60	-- E, uint, 0-4 B, (none)

module.MEDIATYPE_TEXT_PLAIN               = 0	-- text/plain (UTF-8)
module.MEDIATYPE_APPLICATION_LINK_FORMAT  = 40	-- application/link-format
module.MEDIATYPE_APPLICATION_XML          = 41	-- application/xml
module.MEDIATYPE_APPLICATION_OCTET_STREAM = 42	-- application/octet-stream
module.MEDIATYPE_APPLICATION_RDF_XML      = 43	-- application/rdf+xml
module.MEDIATYPE_APPLICATION_EXI          = 47	-- application/exi
module.MEDIATYPE_APPLICATION_JSON         = 50	-- application/json

function module.IsPrints( value )

	if aux_core.IsNil( value ) then

		return false
	end

	assert( aux_core.IsString( value ), "string expected" )

	for i = 1, #value, 1 do

		if not aux_core.IsPrint( value:byte( i, i ) ) then

			return false
		end
	end

	return true
end

function module.TypeToStr( value )

	assert( aux_core.IsNumber( value ), "number expected" )

	return ( value == module.MESSAGE_CON and "CON" ) or
	       ( value == module.MESSAGE_NON and "NON" ) or
	       ( value == module.MESSAGE_ACK and "ACK" ) or
	       ( value == module.MESSAGE_RST and "RST" ) or "UNKNOWN"
end

function module.CodeToStr( value )

	assert( aux_core.IsNumber( value ), "number expected" )

	if value < 32 then

		return ( value == module.REQUEST_EMPTY  and "EMPTY"  ) or
		       ( value == module.REQUEST_GET    and "GET"    ) or
		       ( value == module.REQUEST_POST   and "POST"   ) or
		       ( value == module.REQUEST_PUT    and "PUT"    ) or
		       ( value == module.REQUEST_DELETE and "DELETE" ) or tostring( value )
	else

		local c = ( value >> 5 ) & 0x0f

		local d = ( value >> 0 ) & 0x1f

		return string.format( "%d.%d", c, d )
	end
end

function module.FindOptions( pdu, id )

	local result = {}

	local index = 1

	for key, val in pairs( pdu ) do

		if id == key then

			result[ index ] = val

			index = index + 1
		end
	end

	return result
end

function module.FindURIQuery( pdu, name )

	local r = module.FindOptions( pdu, module.OPTION_URI_QUERY )

	for id, value in pairs( r ) do

		local first, last = value:find( name .. '=', 1, true )

		if first and last then

			return value:sub( last + 1 )
		end
	end

	return nil;
end

function module.DebugPrint( pdu )

	if aux_core.IsNil( pdu ) then

		return nil
	end

	local OptionToStr = function( num, data )

		if module.IsPrints( data ) then

			return string.format( "  %-6d %s", num, data )
		else

			return string.format( "  %-6d %s", num, aux_core.StrToHexStr( data ) )
		end
	end

	print( "Type:", module.TypeToStr( pdu:get_type() ) )

	print( "Code:", module.CodeToStr( pdu:get_code() ) )

	print( "MsgID:", pdu:get_msgid() )

	print( "Token:", aux_core.StrToHexStr( pdu:get_token() ) )

	print( "Options:" )

	for key, val in pairs( pdu ) do

		print( OptionToStr( key, val ) )
	end

	print( "Payload:" )

	local data = pdu:get_payload()

	if module.IsPrints( data ) then

		print( data )
	else

		print( aux_core.StrToHexStr( data ) )
	end
end

function module.Transact( udp, pdu, timeout )

	local lua_coap = require( "lua_coap" )

	assert( module.MESSAGE_CON == pdu:get_type() or module.MESSAGE_NON == pdu:get_type() )

	local token = pdu:get_token()

	assert( #pdu == udp:send( pdu:data() ) )

	if module.MESSAGE_NON == pdu:get_type() and ( not token ) then

		return nil
	end

	assert( 1 == udp:wait( "r", timeout ) )

	local reply = lua_coap.coap_pdu( ( udp:recv( 1500 ) ) )

	if module.MESSAGE_NON == pdu:get_type() then

		assert( module.MESSAGE_NON == reply:get_type() and token == reply:get_token() )
	else

		if token ~= reply:get_token() then

			reply = lua_coap.coap_pdu( ( udp:recv( 1500 ) ) )
		end
	end

	return reply
end

function module.TransactTo( udp, addr, port, pdu, timeout )

	local lua_coap = require( "lua_coap" )

	assert( module.MESSAGE_CON == pdu:get_type() or module.MESSAGE_NON == pdu:get_type() )

	local token = pdu:get_token()

	assert( #pdu == udp:sendto( addr, port, pdu:data() ) )

	if module.MESSAGE_NON == pdu:get_type() and ( not token ) then

		return nil
	end

	assert( 1 == udp:wait( "r", timeout ) )

	local data, addr, port = udp:recvfrom( 1500 )

	local reply = lua_coap.coap_pdu( data )

	if module.MESSAGE_NON == pdu:get_type() then

		assert( module.MESSAGE_NON == reply:get_type() and token == reply:get_token() )
	else

		if token ~= reply:get_token() then

			reply = lua_coap.coap_pdu( ( udp:recv( 1500 ) ) )
		end
	end

	return reply, addr, port
end

function module.OnceTransact( addr, port, pdu, timeout )

	local lua_socket = require( "lua_socket" )

	local udp = lua_socket.udp()

	assert( udp )

	assert( ( udp:connect( addr, port ) ) )

	local result = module.Transact( udp, pdu, timeout )

	udp:close()

	return result
end

return module
