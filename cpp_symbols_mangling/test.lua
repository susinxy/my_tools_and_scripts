dofile('cpp_symbol_gen.lua')

local function print_symbol(name)
	print(name..' ---> '..cpp_symbol_gen(name))
end



print_symbol('test')
print_symbol('::test()')
print_symbol('::test(unsigned int)')
print_symbol('::test(int,char, doubole)')
print_symbol('::test(SOME_STRU, int,char,double)')
print_symbol('test::test()')
print_symbol('test::~test()')
print_symbol('test::test(int ,SOME_STRU)')
print_symbol('test::~test(int,SOME_STRU)')
print_symbol('test::call_a(int)')
print_symbol('test::call_a(int,SOME_STRU)')
print_symbol('namesp_t::test::test()')
print_symbol('namesp_t::test::call_a(int,SOME_STRU)')

