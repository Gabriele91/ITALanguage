//
//  ita_language.h
//  ITALanguage
//
//  Created by Gabriele Di Bari on 24/12/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//
#pragma once

#include <config.h>
#include <compile_tree_to_cpp.h>
#include <compile_tree_to_js.h>
#include <syntactic_tree.h>
#include <parser.h>

namespace it_language
{
    class program_language
    {
        
        using tree         = it_language::syntactic_tree;
        using parser       = it_language::parser;
        using js_compiler  = it_language::compile_tree_to_js;
        using cpp_compiler = it_language::compile_tree_to_cpp;
        //parser...
        parser       it_parser;
        
    public:
        
        enum compiler_flags
        {
            NONE    = 0x000,
            TO_JS   = 0x001,
            TO_CPP  = 0b010,
            ERRORS  = 0b100
        };
        
        struct compiler_ouput
        {
            int         m_type{ NONE };
            std::string m_out_js;
            std::string m_out_cpp;
            std::string m_errors;
        };
        
        compiler_ouput compile(const std::string& source_code,int flags = NONE)
        {
            //targets
            tree         it_tree;
            //output
            compiler_ouput output;
            //
            if (!it_parser.italanguage(source_code,it_tree))
            //ouput errors
            {
                for (auto& error : it_parser.m_errors)
                {
                    output.m_errors += std::to_string( error.m_line ) + ":" + error.m_error + "\n";
                }
                //fail
                output.m_type = ERRORS;
                //return
                return output;
            }
            //else compile to...
            //CPP COMPILER
            if(flags & TO_CPP)
            {
                cpp_compiler compiler;
                compiler.compile(&it_tree);
                //errors?
                if(compiler.m_errors.size())
                {
                    //ouput errors
                    for (auto& error : compiler.m_errors)
                    {
                        output.m_errors += error + "\n";
                    }
                    //output errors
                    output.m_type |= ERRORS;
                }
                //output code
                output.m_type |= TO_CPP;
                output.m_out_cpp = compiler.m_cpp_code;
            }
            //JS COMPILER
            if(flags & TO_JS)
            {
                js_compiler compiler;
                compiler.compile(&it_tree);
                //errors?
                if(compiler.m_errors.size())
                {
                    //ouput errors
                    for (auto& error : compiler.m_errors)
                    {
                        output.m_errors += error + "\n";
                    }
                    //output errors
                    output.m_type |= ERRORS;
                }
                //output code
                output.m_type |= TO_JS;
                output.m_out_js = compiler.m_js_code;
            }
            //return...
            return output;
        }
        
        program_language()
        {
            /*
             //vector
             function vettore();
             function lunghezza(value);
             function dimensione(value);
             function inserisci(v_array,value);
             //console
             function stampa();
             function leggi(value);
             function lettura();
             function inserimento();
             //math
             function seno(value);
             function coseno(value);
             //cast
             function intero(value);
             function stringa(value);
             */
            it_parser.append_into_context({
                                               //vector
                                               "vettore",
                                               "lunghezza",
                                               "dimensione",
                                               "inserisci",
                                               //console
                                               "stampa",
                                               "leggi",
                                               "lettura",
                                               "inserimento",
                                               //math
                                               "seno",
                                               "coseno",
                                               //cast
                                               "intero",
                                               "stringa"
                                           },
                                           {
                                               //none
                                           });
            
        }
    };
};