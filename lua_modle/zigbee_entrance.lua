
local module = {}

module.zigbee_online =1
module.zigbee_offline =2
module.zigbee_coap_outime=3
module.zigbee_uart_outime=4


function module.zigbee_dev_check_modle(modle_dev)

	if(type(modle_dev)~="table") then
		print("modle_dev not a table")
			return 0
    end
	print("ep num="..#modle_dev.endpoint)
	
    for k,v in ipairs(modle_dev.endpoint) do
			print(string.format("-------------check endpoint=%d  start!!",k))
			if(type(v)~="table")then
				print(string.format("endpoint=%d not a table",k))
				return 0
			end
			
			if(type(v["ep_id"])~="number")then
				print("ep_id erroe")
				return 0
			end
			print("ep_id="..v["ep_id"])
			
			if(type(v["dev_id"])~="number")then
				print("dev_id erroe")
				return 0
			end
			print("dev_id="..v["dev_id"])			
			
			if(type(v["profile"])~="number")then
				print("profile error")
				return 0
			end
			print("profile="..v["profile"])
			
			if(type(v["device_version"])~="number")then
				print("device_version error")
				return 0
			end
			print("device_version="..v["device_version"])		

			
			if(type(v["in_cluster"])~="table" and type(v["in_cluster"])~="nil")then
				print("in_cluster error")
				return 0
			elseif(type(v["in_cluster"])=="table") then
				for k,v in ipairs(v["in_cluster"]) do
					if(type(v)~="number")then
						print(string.format("in_cluster index=%d is error",k))
						return 0
					end
					print(string.format("%04x",v))					
				end
			end
			print("in_cluster type="..type(v["in_cluster"]))	
			
			if(type(v["out_cluster"])~="table" and type(v["out_cluster"])~="nil")then
				print("out_cluster error")
				return 0
			end
			print("out_cluster num="..type(v["out_cluster"]))	
			
			print(string.format("-------------check endpoint=%d  ok!!",k))
    end	
	print("-------------check modle ok!!")
	print("----------------------------!")	
	print("----------------------------!")
	return 1
end


local zigbee_match_cluster_id= function(src_cluster,modle_clusterid)
	for k,v in pairs(src_cluster) do
		if(v==modle_clusterid)then
			print(string.format("-------------cluster_id[%d]=%04x",k,v))
			return 1;
		end
	end
	print(string.format("-------------cluster id check error modle_clusterid=%04x",modle_clusterid))
	return 0
end

local zigbee_match_ep_info=function(src_ep,modle_ep)

		if(src_ep.profile~=modle_ep.profile) then
			return 0
		end
		if(src_ep.dev_id~=modle_ep.dev_id)then
			return 0
		end	
		if(src_ep.device_version~=modle_ep.device_version)then
			return 0
		end	
		print("----------------check in_cluster")
		
		local src_incluster_type=type(src_ep.in_cluster)
		local modle_incluster_type=type(modle_ep.in_cluster)
		
		if(src_incluster_type~=modle_incluster_type)then
			return 0
		elseif(src_incluster_type=="table" and #src_ep.in_cluster~=#modle_ep.in_cluster)then
			return 0
		elseif(src_incluster_type=="table")then
			for k,v in ipairs(modle_ep.in_cluster) do
				if(0==zigbee_match_cluster_id(src_ep.in_cluster,v))then
					return 0
				end
			end			
		end	
		
		print("----------------check out_cluster")
		local src_outcluster_type=type(src_ep.out_cluster)
		local modle_outcluster_type=type(modle_ep.out_cluster)
		
		if(src_outcluster_type~=modle_outcluster_type) then
			return 0
		elseif(src_outcluster_type=="table"and #src_ep.out_cluster~=#modle_ep.out_cluster)then
			return 0
		elseif(src_outcluster_type=="table")then
			for k,v in ipairs(modle_ep.out_cluster) do
				if(0==zigbee_match_cluster_id(src_ep.out_cluster,v))then
					return 0
				end
			end				
		end	
		
        return 1
end

local zigbee_find_match_ep_info=function(dev_ep,modle_ep)
	print("modle_ep.ep_id="..modle_ep.ep_id)
        for k,v in ipairs(dev_ep) do
			if(v.ep_id==modle_ep.ep_id) then
				if(0==zigbee_match_ep_info(v,modle_ep))then
					return 0
				end
			end
        end
        return 1
end

function module.zigbee_match_dev_info(dev_ep,modle_dev)

		print("-----------------start match_dev!!")
		if(#modle_dev.endpoint~=#dev_ep)then
			print("-------------ep num error")
			return 0
		end
		print("-------------ep num is ok and ep_num="..#modle_dev.endpoint)
		
        for k,v in ipairs(modle_dev.endpoint) do
			print("-------------check ep index="..k.."::ep_id="..v.ep_id)
			if(0==zigbee_find_match_ep_info(dev_ep,v))then
				print("--------------check ep error")
				return 0
			end
        end
		print("-------------dev is ok!!!")
        return 1
end
return module
--zigbee_dev_modle(dev_lock)
--print(zigbee_match_dev_info(dev_lock,dev_lock_base))
