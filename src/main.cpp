
#include <iostream>
#include <fstream>
#include <sstream>
#include <program_language.h>



int main(int argc, const char* argv[])
{
    //fast access
    using program_language = it_language::program_language;
    //compiler object
    program_language it_compiler;
    //read code
	std::ifstream source_file("source.it");
	std::string source((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
    //compile
    program_language::compiler_ouput compiler_ouput;
    compiler_ouput = it_compiler.compile(source,program_language::TO_JS|
                                                program_language::TO_CPP);
    //ouput:
    if(compiler_ouput.m_type & program_language::ERRORS)
    {
        std::cout << compiler_ouput.m_errors;
    }
    //output code cpp
    {
        std::ofstream outfile("TestOputCode/_ouput_.cpp");
        outfile << compiler_ouput.m_out_cpp;
        outfile.close();
    }
    //output code js
    {
        std::ofstream outfile("TestOputCodeJS/index.html");
        outfile << compiler_ouput.m_out_js;
        outfile.close();
    }
    
	return 0;
}