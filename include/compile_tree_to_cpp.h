//
//  compile_tree_to_cpp.h
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

	const char* code_cpp_header = 
	R"CPP_CODE(

	#include <string>
    #include <iostream>
    #include <assert.h>
    #include <cmath>
	
    class it_variable
	{
	public:

		enum type
		{
			INT,
			FLOAT,
			STRING
		};

		type m_type{ INT };

		union value
		{
			int   	     m_i;
			float  	 	 m_f;
			std::string* m_pstr;
		};

		value m_value{ 0 };

		it_variable() : it_variable(0) {}

		it_variable(int i)
		{
			m_type = INT;
			m_value.m_i = i;
		}

		it_variable(float f)
		{
			m_type = FLOAT;
			m_value.m_f = f;
		}

		it_variable(const char* cstr)
		{
			m_type = STRING;
			m_value.m_pstr = new std::string(cstr);
		}

		it_variable(const std::string& str)
		{
			m_type = STRING;
			m_value.m_pstr = new std::string(str);
		}

		it_variable(const it_variable& value)
		{
			m_type = value.m_type;

					if (m_type == STRING)
			{
				m_value.m_pstr = new std::string(*value.m_value.m_pstr);
			}
			else
			{
				m_value = value.m_value;
			}
		}

		it_variable& operator = (const it_variable& value) 
		{
			//dealloc
			this->~it_variable();
			//copy type
			m_type = value.m_type;
			//copy value
			if (m_type == STRING)
			{
				m_value.m_pstr = new std::string(*value.m_value.m_pstr);
			}
			else
			{
				m_value = value.m_value;
			}
			return *this;
		}

		virtual ~it_variable()
		{
			if (m_type == STRING) delete m_value.m_pstr;
		}

		it_variable operator + (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i + var.m_value.m_i;
			}

			if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f + var.m_value.m_i;
			}

			if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i + var.m_value.m_f;
			}

			if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f + var.m_value.m_f;
			}

			if (m_type == STRING && var.m_type == STRING)
			{
				return *m_value.m_pstr + *var.m_value.m_pstr;
			}

			if (m_type == STRING && var.m_type == FLOAT)
			{
				return *m_value.m_pstr + std::to_string(var.m_value.m_f);
			}

			if (m_type == STRING && var.m_type == INT)
			{
				return *m_value.m_pstr + std::to_string(var.m_value.m_i);
			}

			if (m_type == FLOAT && var.m_type == STRING)
			{
				return std::to_string(m_value.m_f) + *var.m_value.m_pstr;
			}

			if (m_type == INT && var.m_type == STRING)
			{
				return std::to_string(m_value.m_i) + *var.m_value.m_pstr;
            }
            
            assert(0);
            return it_variable();
		}

		it_variable operator - (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i - var.m_value.m_i;
			}

            if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f - var.m_value.m_i;
			}

            if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i - var.m_value.m_f;
			}

            if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f - var.m_value.m_f;
            }
            
            assert(0);
            return it_variable();
		}

		it_variable operator / (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i / var.m_value.m_i;
			}

            if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f / var.m_value.m_i;
			}

            if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i / var.m_value.m_f;
			}

            if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f / var.m_value.m_f;
            }
            
            assert(0);
            return it_variable();
		}

		it_variable operator * (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i * var.m_value.m_i;
			}

            if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f * var.m_value.m_i;
			}

            if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i * var.m_value.m_f;
			}

            if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f * var.m_value.m_f;
            }
            
            assert(0);
            return it_variable();
		}

		bool operator == (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i == var.m_value.m_i;
			}

					if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f == var.m_value.m_i;
			}

					if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i == var.m_value.m_f;
			}

					if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f == var.m_value.m_f;
			}
            
            assert(0);
            return false;
		}

		bool operator > (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i > var.m_value.m_i;
			}

            if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f > var.m_value.m_i;
			}

            if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i > var.m_value.m_f;
			}

            if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f > var.m_value.m_f;
            }
            
            assert(0);
            return false;
		}

		bool operator < (it_variable var)
		{
			if (m_type == INT && var.m_type == INT)
			{
				return m_value.m_i < var.m_value.m_i;
			}

            if (m_type == FLOAT && var.m_type == INT)
			{
				return m_value.m_f < var.m_value.m_i;
			}

            if (m_type == INT && var.m_type == FLOAT)
			{
				return m_value.m_i < var.m_value.m_f;
			}

            if (m_type == FLOAT && var.m_type == FLOAT)
			{
				return m_value.m_f < var.m_value.m_f;
            }
            
            assert(0);
            return false;
		}
	};
    
    it_variable intero(it_variable var)
    {
        switch (var.m_type)
        {
            case it_variable::INT:    return var.m_value.m_i; break;
            case it_variable::FLOAT:  return (int) var.m_value.m_f; break;
            case it_variable::STRING: return std::stoi(*var.m_value.m_pstr,nullptr); break;
            default: assert(0); break;
        }
    }
    
    it_variable stringa(it_variable var)
    {
        switch (var.m_type)
        {
            case it_variable::INT:    return std::to_string(var.m_value.m_i); break;
            case it_variable::FLOAT:  return std::to_string(var.m_value.m_f); break;
            case it_variable::STRING: return *var.m_value.m_pstr; break;
            default: assert(0); break;
        }
    }
    
	void stampa(it_variable var)
	{
		switch (var.m_type)
		{
		case it_variable::INT: std::cout << var.m_value.m_i; break;
		case it_variable::FLOAT: std::cout << var.m_value.m_f; break;
		case it_variable::STRING: std::cout << *var.m_value.m_pstr; break;		
		default: assert(0); break;
		}
	}

	template<typename ... Args>
	void stampa(it_variable var, Args ... args)
	{
		stampa(var);
		stampa(args...);
    }
    
    #if 0
        void leggi(it_variable& var)
        {
            std::string value;
            std::cin >> value ;
            var = value;
        }
    #endif
    
    it_variable lettura()
    {
        std::string value;
        std::cin >> value ;
        return it_variable(value);
    }
    
    it_variable lettura(it_variable value)
    {
        std::string out_value;
        std::cout << *stringa(value).m_value.m_pstr << ": ";
        std::cin  >>  out_value ;
        return it_variable(out_value);
    }
    
    it_variable inserimento()
    {
        std::string value;
        std::cin >> value ;
        return it_variable(value);
    }
    
    it_variable inserimento(it_variable value)
    {
        std::string out_value;
        std::cout << *stringa(value).m_value.m_pstr << ": ";
        std::cin  >>  out_value ;
        return it_variable(out_value);
    }
    
    it_variable seno(it_variable var)
	{
		switch (var.m_type)
		{
		case it_variable::INT: return std::sin( (float)var.m_value.m_i ); break;
		case it_variable::FLOAT: return std::sin( var.m_value.m_f ); break;
		default: assert(0); break;
		}
	}

    it_variable coseno(it_variable var)
	{
		switch (var.m_type)
		{
		case it_variable::INT: return std::sin( (float)var.m_value.m_i ); break;
		case it_variable::FLOAT: return std::sin( var.m_value.m_f ); break;
		default: assert(0); break;
		}
	}
    

    )CPP_CODE";

	class compile_tree_to_cpp
	{
	public:

		std::list < std::string > m_errors;
		std::string m_cpp_code;

		std::string compile_exp(syntactic_tree::exp_node* node)
		{
			if (node->m_type == syntactic_tree::EXP_NODE)
			{
				return "(" + compile_exp(node->m_left) + ")" + node->m_name + "(" + compile_exp(node->m_right) + ")";
			}
			if (node->m_type == syntactic_tree::VARIABLE_NODE)
			{
				return node->to<syntactic_tree::variable_node>()->m_name;
			}
			if (node->m_type == syntactic_tree::CONSTANT_NODE)
			{
				syntactic_tree::constant_node* c_node = node->to<syntactic_tree::constant_node>();
				switch (c_node->m_const_type)
				{
				case syntactic_tree::constant_node::FLOAT:  return std::to_string(c_node->m_value.m_float) + "f";
				case syntactic_tree::constant_node::INT:    return std::to_string(c_node->m_value.m_int);
				case syntactic_tree::constant_node::STRING: return compiler_utilities::compile_string(c_node->m_value.m_string);
				default: assert(0); break;
				}
			}
			if (node->m_type == syntactic_tree::CALL_NODE)
			{
				return compile_call((syntactic_tree::call_node*)node,false);
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
                cout += std::string("it_variable ")
                     + fields.m_variable->m_name
                     + (fields.m_exp ? "=" + compile_exp(fields.m_exp) : "")
                     + "; \n";
            }
			//else
            return cout;
		}

		std::string compile_op(syntactic_tree::op_node* node)
		{
			return node->m_variable->m_name + " = " + compile_exp(node->m_exp) + ";\n";
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

		compile_tree_to_cpp& compile(const syntactic_tree* tree)
		{
			m_cpp_code = std::string(code_cpp_header)+"\nint main()\n{\n" + compile_statements(tree->m_root->m_staments) + "return 0;\n}\n";
			return *this;
		}

	};
};