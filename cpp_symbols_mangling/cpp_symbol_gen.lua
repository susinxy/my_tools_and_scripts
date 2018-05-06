local cpp_symbol_gen_compiler='g++'
local mediate_file_name_for_type_define='__lua_cpp_symbol_gen_temp_typedef'
local function make_class(cname,funame)
	local ret_str=''
	local class_txt
	if cname == '' then return 'void '..funame..'{}' end
	local s,e=string.find(funame,'(',1,true)
	if s == nil then return '' end
	if string.sub(funame,1,s-1) ~= cname then ret_str ='void ' end
	if string.sub(funame,1,1) == '~' then ret_str='' end
	class_txt='class '..cname..'{public:'..ret_str..funame..';};'..ret_str..cname..'::'..funame..'{}'
	return class_txt
end

local function cpp_compile_txt(code_txt,file_name)
	os.execute('echo '.."'"..code_txt.."' > "..file_name..'.cpp')
	local ret,state,code=os.execute(cpp_symbol_gen_compiler..' -c '..file_name..'.cpp 2> /dev/null')
	if ret==true then
		if state == 'exit' then
			if code == 0 then return true end
		end
	end
	return false
end

local function is_basic_type(typename)
	local ret=cpp_compile_txt(typename..' i;',mediate_file_name_for_type_define)
	os.execute('rm -rf '..mediate_file_name_for_type_define..'.*')
	return ret
end

local function make_typedef_txt(typename)
	local type_txt='typedef struct '..typename..'{char i;}'..typename..';'
	return type_txt
end

local function make_typedef(funame)

	local typedef_txt=''
	local funame_formated=string.gsub(funame,'[%(,%)]',',')
	if funame == funame_formated then return typedef_txt end
	for w in string.gmatch(funame_formated, ', *([^,]*) *') do
		if w ~= '' and is_basic_type(w) ~= true then
			typedef_txt=typedef_txt..make_typedef_txt(w)
		end
	end
	return typedef_txt
end

--[[
this function returns two results, the first one is the string found starting from the start_point unitl then end_sym string,
not including the end_sym string. the second result is what's left in the string_to_find,not including the end_sym. if end_sym
is not found. if the start point is negative, then searching backwards from then end
]]
local function cpp_get_first_token(string_to_find,end_sym,start_point)
	local string_to_find_formated=string_to_find
	local start_point_formated=start_point
	local end_sym_formated=end_sym
	if start_point < 0 then 
		start_point_formated = 0-start_point
		string_to_find_formated = string.reverse(string_to_find)
		end_sym_formated=string.reverse(end_sym)
	end
	local s,e=string.find(string_to_find_formated,end_sym_formated,start_point_formated,true)
	if s == nil then return '',string_to_find end
	local token_found=string.sub(string_to_find_formated,1,s-1)
	string_left=string.sub(string_to_find_formated,e+1,-1)
	if start_point < 0 then
		string_left=string.reverse(string_left)
		token_found=string.reverse(token_found)
	end
	return token_found,string_left
end

local function cpp_symbol_gen_txt(name)
	local funame,name_without_funame=cpp_get_first_token(name,'::',-1)
	if funame == '' then return 'extern "C" void '..name..'(){}' end
	local typedef_txt=make_typedef(funame)
	if name_without_funame == '' then return typedef_txt..make_class('',funame) end

	local class_name,name_without_class_funame=cpp_get_first_token(name_without_funame,'::',-1)
	if class_name == '' then return typedef_txt..make_class(name_without_funame,funame) end
	if name_without_class_funame == '' then return typedef_txt..make_class(class_name,funame) end
	
	local namespace,name_all_left=cpp_get_first_token(name_without_class_funame,'::',-1)
	if namespace ~= '' then print('two many ::') return '' end
	return typedef_txt..'namespace '..name_without_class_funame..'{'..make_class(class_name,funame)..'}'
end


function cpp_symbol_gen_set_cpp_compiler(compiler_name)
	cpp_symbol_gen_compiler=compiler_name
end

local mediate_file_name_for_cpp_symbol='__lua_cpp_symbol_temp_code_file'

function cpp_symbol_gen(name)
	code_txt=cpp_symbol_gen_txt(name)
	if code_txt == '' then return '' end
	--print(code_txt)
	local ret=cpp_compile_txt(code_txt,mediate_file_name_for_cpp_symbol)
	if ret == false then
		os.execute('rm -rf '..mediate_file_name_for_cpp_symbol..'.*')
		return ''
	end
	os.execute('nm -f poxis --defined-only '..mediate_file_name_for_cpp_symbol..'.o >'..mediate_file_name_for_cpp_symbol)
	local symbol_file=io.open(mediate_file_name_for_cpp_symbol)
	local symbol_info=symbol_file:read()
	symbol_file:close()
	os.execute('rm -rf '..mediate_file_name_for_cpp_symbol..'.* '..mediate_file_name_for_cpp_symbol)
	if symbol_info == nil then return '' end
	local s,e=string.find(symbol_info,' ',1,true)
	if s == nil then return '' end
	return string.sub(symbol_info,1,s-1)
end

