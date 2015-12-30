
#include <iostream>
#include <fstream>
#include <sstream>
#include <program_language.h>


int main(int argc,const char* args[])
{
    //fast access
    using program_language = it_language::program_language;
    using compiler_flags   = program_language::compiler_flags;
    using compiler_ouput   = program_language::compiler_ouput;
    //name app
    std::string name(args[0]);
    //C++ args
    std::vector< std::string > v_args;
    //alloc args
    v_args.resize(argc-1);
    //copy args
    for(size_t i=1;i!=argc;++i) v_args[i-1]=args[i];
    //options
    bool         b_show_version  = false;
    int          f_compier_flags = compiler_flags::NONE;
    std::string  i_source;
    std::string  o_source_cpp;
    std::string  o_source_js;
    {
        if(v_args.size()==1 && ( v_args[0]=="-h" || v_args[0]=="--help" ))
        {
            MESSAGE( name << " [options]" );
            MESSAGE( "Options:" );
            MESSAGE( "\t--help/-h help" );
            MESSAGE( "\t--version/-v compiler version" );
            MESSAGE( "\t--source/-s <path> source code" );
            MESSAGE( "\t--output-cpp/-o <path>  output C++ code" );
            MESSAGE( "\t--output-js/-j <path> output javascript code" );
            return 0;
        }
        else if(v_args.size()==1)
        {
            MESSAGE("Invalid input");
            return -1;
        }
        else
        {
            //for all
            for(size_t arg=0; arg!= v_args.size() ; ++arg)
            {
                if (v_args[arg] == "-v" || v_args[arg] == "--version")
                {
                    b_show_version = true;
                }
                else if( (arg+1)!= v_args.size() )
                {
                    if (v_args[arg] == "-s" || v_args[arg] == "--source")
                    {
                        i_source = v_args[arg + 1]; ++arg;
                    }
                    else if (v_args[arg] == "-o" || v_args[arg] == "--output-cpp")
                    {
                        f_compier_flags |= compiler_flags::TO_CPP;
                        o_source_cpp     = v_args[arg + 1]; ++arg;
                    }
                    else if (v_args[arg] == "-j" || v_args[arg] == "--output-js")
                    {
                        f_compier_flags |= compiler_flags::TO_JS;
                        o_source_js      = v_args[arg + 1]; ++arg;
                    }
                    else
                    {
                        MESSAGE( "Invalid input" );
                        return -1;
                    }
                }
                else
                {
                    MESSAGE( "Invalid input" );
                    return -1;
                }
            }
        }
    }
    //COMPILER
    {
        //compiler object
        program_language it_compiler;
        //read code // "source.it"
        std::ifstream source_file(i_source);
        std::string source((std::istreambuf_iterator<char>(source_file)),
                           (std::istreambuf_iterator<char>()));
        //compile
        program_language::compiler_ouput compiler_ouput;
        compiler_ouput = it_compiler.compile(source,f_compier_flags);
        //ouput:
        if(compiler_ouput.m_type & program_language::ERRORS)
        {
            std::cout << compiler_ouput.m_errors;
        }
        //output code cpp
        if( f_compier_flags & compiler_flags::TO_CPP )
        {
            //"TestOputCode/_ouput_.cpp"
            std::ofstream outfile(o_source_cpp);
            outfile << compiler_ouput.m_out_cpp;
            outfile.close();
        }
        //output code js
        if( f_compier_flags & compiler_flags::TO_JS )
        {
            //"TestOputCodeJS/index.html"
            std::ofstream outfile(o_source_js);
            outfile << compiler_ouput.m_out_js;
            outfile.close();
        }
    }
    
    return 0;
}