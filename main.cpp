#include <iostream>

#include <lispp/lispp_interpreter.h>

int main() {
	LispInterpreter interpreter(&std::cin, &std::cout);
	interpreter.LaunchREPL();
    return 0;
}
