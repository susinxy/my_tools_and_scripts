# dependencies
The cpp_symbol_gen.lua works depending on the following tools:

1. compiler which you work with: because C++ has no standar ABI specifiction for name mangling across different compilers, therefore a compiler with which you want to generate symbols is needed.

2. command _nm_ and _grep_ is required

# why I create cpp_symbol_gen.lua
Recently, I am working on a project in my current company. It's an arm embeded system with an openSUSE linux running on it. Things like porting of the kernal and all that needed to make linux run and hardwares available are done when I got into this project. Folks before me have created a shell that can run under the environment of an running process. This shell can make function calls defined by the running process from the prompt line, setting and getting global values
and so on, while processes keep runing with no hang on. This is very convenient when interacting with the running program.

However, two things that I want is not done.
  
1. the first is that I lack of a script language embeded into the running process. this means if I want to do something programically. I have to write code and compile, then reloard.
Since the program deals with the hardware, in order to run the program, each time a reboot is required. This is painful

2. Secondly, this shell is ok calling functions in C, but has no way dealing with C++ symbols.

Two factors above is why I choose a script language like lua to be embeded into the program, and write this script to work with the shell to achive symbols calls.
The implementation of the shell and the embeding work is not presented in this git right now. Hope I will have time moving them here.

# How it works
cpp_symbol_gen.lua offers tow functions. one is called _cpp_symbol_gen_set_cpp_compiler(compiler_name)_, which used to set the compiler name. Another one is called
_cpp_symbol_gen(name)_, which takes a function or a member of a class and return the symbol in string.

The _name_ is as in the following format:
1. if variables and functions are written in C, the _name_ is just the name of the function or the variable.
>"For example, if you have a function _void do_something(int argc,char *args[])_ in a.c file. To get the symbol for this function,
try _cpp_symbol_gen('do_something')_"

2. for a sigle function or variable in .cpp file. just add _::_ in the front of the name of the function or the variable.
>"For example, if you have a function _void do_something(int argc,char *args[])_ in a.cpp file. To get the symbol for this function,
try _cpp_symbol_gen('::do_something(int,char*')_ .Please note that, here, the arguments' type are specified. This is because in C++,
arguments' type count in identifying itself."

3.for functions in a class, do as following:
>if you have following code :
>
>```
>class dumy{
>public:
>  dumy();
>  ~dumy();
>  int do_something(int argc, char* agrs[]);
>};
>```
>and then you want the symbol name for do_something, just try _cpp_symbol_gen('dumy::do_somthing(int,char*)')_ .
if _class dumy_ is withing anamespace call ns_dumy, then try _cpp_symbol_gen('ns_dumy::dumy:do_something(int,char*)')_ 
