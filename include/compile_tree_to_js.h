//
//  compile_tree_to_js.h
//  ITALanguage
//
//  Created by Gabriele Di Bari on 12/12/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//
#pragma once

#include <string>
#include <list>
#include <assert.h>
#include <syntactic_tree.h>
#include <compiler_utilities.h>

namespace it_language
{
    
	const char* code_js_header =
	R"JS_CODE(
    <!DOCTYPE html>
    <html>
	<head>
	<meta charset="UTF-8"/>
	</head>
    <style>
    
    .console
    {
        margin: 20px;
    }
    
    .console #header
    {
        width: 640px;
        color: #000;
        background-color: #CFCFCF;
        padding: 5px 20px 5px 20px;
        border-top-left-radius:  5px;
        border-top-right-radius: 5px;
        box-shadow: 10px 10px 5px #999999;
    }
    
    .console #text
    {
        width: 640px;
        min-height: 480px;
        color: #FFF;
        text-align: left;
        background-color: #000;
        padding: 20px;
        border-bottom-left-radius:  5px;
        border-bottom-right-radius: 5px;
        box-shadow: 10px 10px 5px #999999;
    }
    
    </style>
    <body>
    <center>
    <div class="console">
        <div id="header">CONSOLE</div>
        <div id="text"></div>
    <div>
    </center>
    </body>
    <script>
    
    function stampa()
    {
        function nl2br (str)
        {
            return (str + "").replace(/([^>\r\n]?)(\r\n|\n\r|\r|\n)/g, "$1" + "<br>" + "$2");
        }
        var console = document.getElementById("text");
        for (var i=0; i < arguments.length; ++i)
        {
            console.innerHTML += nl2br(arguments[i]);
        }
    }

    function vettore()
    {
       return [];
    }   

	function lunghezza(value)
    {
       return value.length;
    }   

	function dimensione(value)
    {
       return value.length;
    }   

	function inserisci(v_array,value)
    {
       v_array.push(value);
    }   

    function leggi(value)
    {
        /*none*/
    }
    
    function lettura()
    {
        if(arguments.length == 0) return prompt("inserisci");
        else                      return prompt(arguments[0]);
    }
    
    function inserimento()
    {
        if(arguments.length == 0) return prompt("inserisci");
        else                      return prompt(arguments[0]);
    }
    
    function seno(value)
    {
        return Math.sin(value);
    }
    
    function coseno(value)
    {
        return Math.cos(value);
    }
    
    function intero(value)
    {
        if(typeof value === 'string') return parseInt(value);
        else                          return value;
    }
    
    function stringa(value)
    {
        if(typeof value === 'string') return value;
        else                          return value+"";
    }
    
    )JS_CODE";
    const char* code_js_end =
    R"JS_CODE(
    </script>
    </html>
    )JS_CODE";
    
    class compile_tree_to_js
    {
    public:
        
        std::list < std::string > m_errors;
        std::string m_js_code;

		const std::string& compile_variable(syntactic_tree::variable_node* node)
		{
			return node->m_name;
		}

		std::string compile_assignable(syntactic_tree::assignable_node* node)
		{
			//return variable
			if (node->m_type == syntactic_tree::VARIABLE_NODE) return compile_variable(node->to_variable_node());
			//else
			return compile_assignable(node->to_field_node()->m_assignable) + "[" + compile_exp(node->to_field_node()->m_exp) + "]";
		}

        std::string compile_exp(syntactic_tree::exp_node* node)
        {
            if (node->m_type == syntactic_tree::EXP_NODE)
            {
                return "(" + compile_exp(node->m_left) + ")" + node->m_name + "(" + compile_exp(node->m_right) + ")";
            }
            if (node->m_type == syntactic_tree::CONSTANT_NODE)
            {
                syntactic_tree::constant_node* c_node = node->to<syntactic_tree::constant_node>();
                switch (c_node->m_const_type)
                {
                    case syntactic_tree::constant_node::FLOAT:  return std::to_string(c_node->m_value.m_float);
                    case syntactic_tree::constant_node::INT:    return std::to_string(c_node->m_value.m_int);
                    case syntactic_tree::constant_node::STRING: return compiler_utilities::compile_string(c_node->m_value.m_string);
                    default: assert(0); break;
                }
            }
            if (node->m_type == syntactic_tree::CALL_NODE)
            {
                return compile_call((syntactic_tree::call_node*)node,false);
            }
			if (node->m_type == syntactic_tree::FIELD_NODE ||
				node->m_type == syntactic_tree::VARIABLE_NODE)
			{
				auto*  assignable_node = node->to<syntactic_tree::assignable_node>();
				return compile_assignable(assignable_node);
			}
            if (node->is_link())
            {
                return "(" + compile_exp(node->m_left) + ")";
            }
            if (node->is_one())
            {
                return node->m_name + "(" + compile_exp(node->m_left) + ")";
            }
            m_errors.push_back(std::to_string(node->m_line) + ":" + std::to_string(node->m_char) + ":invalid expression");
            //return void string
            return "";
        }
        
        std::string compile_dec(syntactic_tree::dec_node* node)
        {
            //..
            std::string cout;
            //..
            for(auto fields : node->m_decs)
            {
                cout += std::string("var ")
                + fields.m_variable->m_name
                + (fields.m_exp ? "=" + compile_exp(fields.m_exp) : "")
                + ";\n";
            }
            //else
            return cout;
        }
        
        std::string compile_op(syntactic_tree::op_node* node)
        {
			return compile_assignable(node->m_assignable) + " = " + compile_exp(node->m_exp) + ";\n";
        }
        
        std::string compile_call(syntactic_tree::call_node* node,bool end_line = true)
        {
            //args to string..
            std::string ouput_args;
            for (size_t i = 0; i != node->m_args.size(); ++i)
            {
                ouput_args += compile_exp(node->m_args[i]) +( i < node->m_args.size()-1 ?",":"" );
            }
            //call
            return node->m_variable->m_name + "(" + ouput_args + ")" + (end_line ? "; \n" : "");
        }
        
        std::string compile_while(syntactic_tree::while_node* node)
        {
            return "while("+compile_exp(node->m_exp)+")\n"
            "{\n" + compile_statements(node->m_staments) + "}\n";
        }
        
        std::string compile_if_else(syntactic_tree::if_node* node)
        {
            if (node->is_only_if())
            {
                return "if (" + compile_exp(node->m_exp) + ")\n"
                       "{\n" + compile_statements(node->m_staments) + "}\n";
            }
            return "if (" + compile_exp(node->m_exp) + ")\n"
                   "{\n" + compile_statements(node->m_staments) + "}\n"
                   "else\n"
                   "{\n" + compile_statements(node->m_else_staments) + "}\n";
        }
        
        std::string compile_statements(syntactic_tree::list_nodes& nodes)
        {
            std::string output;
            for (syntactic_tree::node* node : nodes)
            {
                switch (node->m_type)
                {
                    case syntactic_tree::DEC_NODE:    output += compile_dec(node->to<syntactic_tree::dec_node>()); break;
                    case syntactic_tree::OP_NODE:     output += compile_op(node->to<syntactic_tree::op_node>()); break;
                    case syntactic_tree::CALL_NODE:   output += compile_call(node->to<syntactic_tree::call_node>()); break;
                    case syntactic_tree::IF_NODE:     output += compile_if_else(node->to<syntactic_tree::if_node>()); break;
                    case syntactic_tree::WHILE_NODE:  output += compile_while(node->to<syntactic_tree::while_node>()); break;
                    default:
                        m_errors.push_back(std::to_string(node->m_line) + ":" + std::to_string(node->m_char) + ":invalid statement");
                        break;
                }
            }
            return output;
        }
        
        compile_tree_to_js& compile(const syntactic_tree* tree)
        {
            m_js_code =
              std::string(code_js_header)
            + "\n"
            + compile_statements(tree->m_root->m_staments)
            + std::string(code_js_end);
            return *this;
        }
        
    };
};