
#include <iostream>
#include <fstream>
#include <sstream>
#include <compile_tree_to_cpp.h>
#include <compile_tree_to_js.h>
#include <syntactic_tree.h>
#include <parser.h>



int main(int argc, const char* argv[])
{
	using tree         = it_language::syntactic_tree;
    using parser       = it_language::parser;
    using cpp_compiler = it_language::compile_tree_to_cpp;
    using js_compiler  = it_language::compile_tree_to_js;


	tree      it_tree;
	parser	  it_parser;

	std::ifstream source_file("source.it");
	std::string source((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());

	if (!it_parser.italanguage(source,it_tree))
	//ouput errors
	{
		for (auto& error : it_parser.m_errors)
		{
			std::cout << error.m_line << ":" << error.m_error << "\n";
		}
	}

    //CPP COMPILER
    {
        cpp_compiler compiler;
        compiler.compile(&it_tree);
        //errors?
        if(compiler.m_errors.size())
            //ouput errors
            for (auto& error : compiler.m_errors)
            {
                std::cout << error << "\n";
            }
        //output code
        std::ofstream outfile("TestOputCode/_ouput_.cpp");
        outfile << compiler.m_cpp_code;
        outfile.close();
    }
    //JS COMPILER
    {
        js_compiler compiler;
        compiler.compile(&it_tree);
        //errors?
        if(compiler.m_errors.size())
            //ouput errors
            for (auto& error : compiler.m_errors)
            {
                std::cout << error << "\n";
            }
        //output code
        std::ofstream outfile("TestOputCodeJS/index.html");
        outfile << compiler.m_js_code;
        outfile.close();
    }
	return 0;
}