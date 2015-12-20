//
//  parser.h
//  ITALanguage
//
//  Created by Gabriele Di Bari on 12/12/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//
#pragma once

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include <syntactic_tree.h>
//utf 8
#define UTF_8_CODE

namespace it_language
{
	/*
	// ebnf

	// consts
	// variable   := [a-Z]
	// escaped    := lit('"') | '\\' | '/' | 'b' | 'f' | 'n' | 'r' | 't'
	// chars      := any_chars - '"' - '\\' | '\\' - escaped
	// string     := '"' *chars '"'
	// constant   := double | integer | string
	// space      := *(' '|'\t'|'\r'|'\n')
	// end_comand := '.'
	// end_partial:= ';'
	// end_line   := ','

	//operators
	// logicOp   := ('e'|'o'|'&&'|'||')
	// logicComp := (['è'] 'maggiore' 'di'|['è'] 'minore' 'di'|['è'] 'uguale' 'a'|'<'|'>'|'==')
	// logicOne  := ('non'|'!')

	exp		   := logic
	logic      := compare *(logicOp compare) 
	compare    := summinus ?(logicComp summinus)
	summinus   := timediv *(('+'|'-') timediv)
	timediv    := oneop *(('*'|'/') oneop)
	oneop      := ('-' value) | (logicOne value) | value
	value      := constant | '(' exp ')' | call | assignable
	assignable := variable | variable '[' exp ']'

	//statments
	italanguage := +(stament end_comand)
	staments    := *(stament [end_line]) stament [end_partial]
    stament		:= declaration |  if | call | cicle | operation
    assignment  := ('='|['ed'] 'assegna' 'gli'|'come')
    operation   := assignable assignament exp | 
                   'assegna' exp ('a'|'ad') assignable
	declaration := ("dichiara"|"definisci") (variable | variable assignament exp) *( ',' (variable | variable assignament exp))
	if			:= 'se' exp 'allora' staments ['altrimenti' staments]
    call        := ('esegui'|'chiama') variable [('di'|'del'|'con'|'su'|'dalla'|'da') [*(exp ',') exp]]
	cicle       :=  while | for_each
	while       := 'mentre' <exp> 'ripeti' ':' staments
	for_each    := 'per' 'ogni' variable ('in'|'appartenente'|'di') variable ':' staments
	*/
	class parser
	{
	public:

		//context
		size_t                     m_line;
        //errors
        struct error_info
        {
            size_t m_line { 0 };
            std::string m_error;
            
            error_info(size_t line,const std::string& error)
            {
                m_line = line;
                m_error= error;
            }
            
        };
		std::list < error_info >  m_errors;
		//variables
		std::vector< std::string > m_variables;
		//put variable
		void push_variable(const std::string& str)
		{
			m_variables.push_back(str);
		}
		bool exists_variable(const std::string& str)
		{
			return std::find(m_variables.begin(), m_variables.end(), str) != m_variables.end();
		}
		//push error
		void push_error(const std::string& error)
		{
            m_errors.push_front(error_info( m_line, error ));
		}
		//output file
		std::string				  m_ouput;
		///////////////////////////////////////////////////////////////////////////////////////////////
		//compare string
		static bool strcmp(const char* left, const char* right, size_t len)
		{
			return (std::strncmp(left, right, len) == 0);
		}
		static bool strcmp(const char* left, const char* right)
		{
			return strcmp(left, right, std::strlen(right));
		}
		static bool strcmp_skip(const char*& left, const char* right, size_t len)
		{
			if (strcmp(left, right, len))
			{
				left += len;
				return true;
			}
			return false;
		}
		static bool strcmp_skip(const char*& left, const char* right)
		{
			return strcmp_skip(left, right, std::strlen(right));
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		//defines
#define CSTRCMP(x,y) it_language::parser::strcmp(x,y,sizeof(y)-1)
#define CSTRCMP_SKIP(x,y) it_language::parser::strcmp_skip(x,y,sizeof(y)-1)
		///////////////////////////////////////////////////////////////////////////////////////////////
		//static types
		static bool is_line_space(char c)
		{
			return 	 c == ' ' || c == '\t';
		}
		static bool is_space(char c)
		{
			return 	 c == ' ' || c == '\t' || c == '\r' || c == '\n';
		}
		static bool is_start_name(char c)
		{
			return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
		}
		static bool is_char_name(char c)
		{
			return 	 (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
		}
		static bool is_start_string(char c)
		{
			return c == '\"';
		}
		static bool is_uint_number(char c)
		{
			return 	 (c >= '0' && c <= '9');
		}
		static bool is_float_number(char c)
		{
			return 	 (c >= '0' && c <= '9') || c == '.';
		}
		static bool is_start_int_number(char c)
		{
			return 	 (c >= '0' && c <= '9') || c == '-';
		}
		static bool is_start_float_number(char c)
		{
			return 	 (c >= '0' && c <= '9') || c == '.' || c == '-';
		}
		static bool is_line_comment(const char* c)
		{
			return (*c) == '/' && (*(c + 1)) == '/';
		}
		static bool is_start_multy_line_comment(const char* c)
		{
			return (*c) == '/' && (*(c + 1)) == '*';
		}
		static bool is_end_multy_line_comment(const char* c)
		{
			return (*c) == '*' && (*(c + 1)) == '/';
		}
		static bool is_start_arg(char c)
		{
			return (c == '(');
		}
		static bool is_end_arg(char c)
		{
			return (c == ')');
		}
		static bool is_start_index(char c)
		{
			return (c == '[');
		}
		static bool is_end_index(char c)
		{
			return (c == ']');
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		void skip_line_space(const char*& inout)
		{
			while (is_line_space(*inout)) ++(inout);
		}
		void skip_space(const char*& inout)
		{
			while (is_space(*inout)) ++(inout);
		}
		void skip_line_comment(const char*& inout)
		{
			if (is_line_comment(inout))
			{
				while (*(inout) != EOF &&
					*(inout) != '\0'&&
					*(inout) != '\n')
					++(inout);
			}
		}
		void skip_multy_line_comment(const char*& inout,bool count_lines=true)
		{
			if (is_start_multy_line_comment(inout))
			{
				while (*(inout) != EOF &&
					*(inout) != '\0'&&
					!is_end_multy_line_comment(inout))
				{
                    m_line += count_lines ? (*(inout)) == '\n' : 0;
					++(inout);
				}
				if ((*(inout)) == '*') ++(inout);
			}
		}
		void skip_space_end_comment(const char*& inout,bool count_lines=true)
		{
			while (is_space(*(inout)) ||
				is_line_comment(inout) ||
				is_start_multy_line_comment(inout))
			{
				skip_line_comment(inout);
				skip_multy_line_comment(inout);
				m_line += count_lines ? (*(inout)) == '\n' : 0;;
				++(inout);
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		//parser
		//parser of a unsigned int number
		static bool parse_uint_number(const char*& c, size_t& i)
		{
			std::string number;

			while (is_uint_number(*c)) { number += (*c); ++c; }

			if (number.size())
			{
				std::istringstream(number) >> i;
				return true;
			}

			return false;
		}
		//parser of a int number
		static bool parse_int_number(const char*& c, int& i)
		{
			std::string number;
			//minus
			if ((*c) == '-')
			{
				number += (*c);
				++c;
				if (!is_uint_number(*c)) return false;
			}
			//this part is equals of uint
			while (is_uint_number(*c)) { number += (*c); ++c; }

			if (number.size())
			{
				std::istringstream(number) >> i;
				return true;
			}

			return false;
		}
		//parser float
		static bool parse_ufloat_number(const char*& c, float& i)
		{
			std::string number;
			//before point
			while (is_uint_number(*c)) { number += (*c); ++c; }
			//find point?
			if ((*c) == '.' && is_uint_number(*(c + 1)))
			{
				number += (*c); ++c;
			}
			//next point
			while (is_uint_number(*c)) { number += (*c); ++c; }
			//return float
			if (number.size())
			{
				std::istringstream(number) >> i;
				return true;
			}
			//not valid
			return false;
		}
		static bool parse_float_number(const char*& c, float& i)
		{
			std::string number;
			//minus
			if ((*c) == '-')
			{
				number += (*c);
				++c;
				if (!is_float_number(*c)) return false;
			}
			//before point
			while (is_uint_number(*c)) { number += (*c); ++c; }
			//find point?
			if ((*c) == '.') { number += (*c); ++c; }
			//next point
			while (is_uint_number(*c)) { number += (*c); ++c; }
			//return float
			if (number.size())
			{
				std::istringstream(number) >> i;
				return true;
			}
			//not valid
			return false;
		}
		//parser of a string
		bool parse_cstring(const char* in, const char** cout, std::string& out)
		{
			const char *tmp = in; out = "";
			if ((*tmp) == '\"')  //["...."]
			{
				++tmp;  //[...."]
				while ((*tmp) != '\"' && (*tmp) != '\n')
				{
					if ((*tmp) == '\\') //[\.]
					{
						++tmp;  //[.]
						switch (*tmp)
						{
						case 'n':
							out += '\n';
							break;
						case 't':
							out += '\t';
							break;
						case 'b':
							out += '\b';
							break;
						case 'r':
							out += '\r';
							break;
						case 'f':
							out += '\f';
							break;
						case 'a':
							out += '\a';
							break;
						case '\\':
							out += '\\';
							break;
						case '?':
							out += '\?';
							break;
						case '\'':
							out += '\'';
							break;
						case '\"':
							out += '\"';
							break;
						case '\n': /* jump unix */
							++m_line;
							break;
						case '\r': /* jump mac */
							++m_line;
							if ((*(tmp + 1)) == '\n') ++tmp; /* jump window (\r\n)*/
							break;
						default:
							return true;
							break;
						}
					}
					else
					{
						if ((*tmp) != '\0') out += (*tmp);
						else return false;
					}
					++tmp;//next char
				}
				if ((*tmp) == '\n') return false;
				if (cout) (*cout) = tmp + 1;
				return true;
			}
			//}
			return false;
		}
		//parsing a name (variable/keyword)
		bool parse_name(const char* in, const char** cout, std::string& out)
		{
			if (!is_start_name(*in)) return false;
			out += *in;
			++in;
			while (is_char_name(*in))
			{
				out += *in;
				++in;
			}
			(*cout) = in;
			return true;
		}
		//parser exp
        /*
        logicOp   := ('e'|'o'|'&&'|'||')
        logicComp := (['è'] 'maggiore' 'di'|['è'] 'minore' 'di'|['è'] 'uguale' 'a'|'<'|'>'|'==')
		logicOne  := ('non'|'!')

		exp		   := logic
		logic      := compare *(logicOp compare) 
		compare    := summinus ?(logicComp summinus)
		summinus   := timediv *(('+'|'-') timediv)
		timediv    := oneop *(('*'|'/') oneop)
		oneop      := ('-' value) | (logicOne value) | value
		value      := constant | '(' exp ')' | call | assignable
		assignable := variable | variable '[' exp ']'
		*/
		//parse value
		bool parse_value(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			skip_space_end_comment(ptr);
			//costant number
			if (is_float_number(*ptr))
			{
				float value = 0;
				if (!parse_ufloat_number(ptr, value))
				{
					push_error("not valid float costant");
					return false;
				}
				node = syntactic_tree::constant(value, m_line);
			}
			//costant string
			else if (is_start_string(*ptr))
			{
				std::string value;
				if (!parse_cstring(ptr, &ptr, value))
				{
					push_error("not valid string costant");
					return false;
				}
				node = syntactic_tree::constant(value, m_line);
			}
			//is call ?
			else if (is_call(ptr))
			{
				//get call node
				syntactic_tree::node* call_node;
				if (!parse_call(ptr, call_node)) return false;
				//call node is a exp node
				node = (syntactic_tree::exp_node*)call_node;
			}
			//is variable/field request
			else if (is_an_assignable_attribute(ptr))
			{
				//get assignable attribute
				syntactic_tree::node* var_field_node;
				if (!parse_assignable(ptr, var_field_node)) return false;
				//field node is a exp node
				node = (syntactic_tree::exp_node*)var_field_node;
			}
			//is exp
			else if (is_start_arg(*ptr))
			{
				//skip (
				++ptr;
				//parse exp
				if (!parse_exp(ptr, node))
				{
					push_error("not valid expression (value)");
					return false;
				}
				//skip
				skip_space_end_comment(ptr);
				//end )
				if (!is_end_arg(*ptr))
				{
					push_error("not valid expression (value)");
					return false;
				}
				//skip )
				++ptr;
			}
			//return true...
			return true;
		}
		//one op
		bool parse_oneop(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			//value node
			syntactic_tree::exp_node *oneop = nullptr;
			syntactic_tree::exp_node *value = nullptr;
			//skip
			skip_space_end_comment(ptr);
			//parse op
			if (CSTRCMP_SKIP(ptr, "-"))
			{
				oneop = syntactic_tree::exp("-", nullptr, m_line);
			}
			else if (CSTRCMP_SKIP(ptr, "!"))
			{
				oneop = syntactic_tree::exp("!", nullptr, m_line);
			}
			//skip
			skip_space_end_comment(ptr);
			//parser value
			if (!parse_value(ptr, value))
			{
				push_error("not valid expression (one op)");
				return false;
			}
			// oneop := ('-' value) | (logicOne value) | | value
			if (oneop)
			{
				oneop->m_left = value;
				node = oneop;
			}
			else
			{
				node = value;
			}
			//success
			return true;
		}
		//parser timediv
		bool parse_timediv(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			//value node
			syntactic_tree::exp_node *left   = nullptr;
			syntactic_tree::exp_node *opnode = nullptr;
			//skip
			skip_space_end_comment(ptr);
			//parse value
			if (!parse_oneop(ptr, opnode))   return false;
			//skip
			skip_space_end_comment(ptr);
			//cicle
			while (*ptr == '*' || *ptr == '/')
			{
				//left node
				left   = opnode;
				//node
				opnode = syntactic_tree::exp(std::string() + *ptr, nullptr, m_line);
				//jmp op (*|/)
				++ptr;
				//compone node
				opnode->m_left = left;
				//skip
				skip_space_end_comment(ptr);
				//parse value
				if (!parse_oneop(ptr, opnode->m_right))  return false;
				//skip
				skip_space_end_comment(ptr);
			}
			//save node
			node = opnode;
			//success
			return true;
		}
		//parser timediv
		bool parse_summinus(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			//value node
			syntactic_tree::exp_node *left = nullptr;
			syntactic_tree::exp_node *opnode = nullptr;
			//skip
			skip_space_end_comment(ptr);
			//parse value
			if (!parse_timediv(ptr, opnode))   return false;
			//skip
			skip_space_end_comment(ptr);
			//cicle
			while (*ptr == '+' || *ptr == '-')
			{
				//left node
				left = opnode;
				//node
				opnode = syntactic_tree::exp(std::string() + *ptr, nullptr, m_line);
				//jmp op (+|-)
				++ptr;
				//compone node
				opnode->m_left = left;
				//skip
				skip_space_end_comment(ptr);
				//parse value
				if (!parse_timediv(ptr, opnode->m_right))  return false;
				//skip
				skip_space_end_comment(ptr);
			}
			//save node
			node = opnode;
			//success
			return true;
		}
        //parse compare string op
        struct type_compare_op
        {
            size_t      m_line;    //< line counter
            std::string m_op;      //< type operation
            bool        m_success; //< success to parsing
            const char* m_ptr;     //< string pointer
        };
        type_compare_op parse_compare_string_operation(const char* ptr)
        {
            type_compare_op op_return
            {
                (size_t)(0),
                std::string(),
                false,
                nullptr
            };
            //save current line
            size_t tmp_line = m_line;
            //['è'] 'maggiore' ['di']|['è'] 'minore' ['di']| ['è'] 'uguale' ['a']
            #if defined( UTF_8_CODE )
                //è = C3 A8 in utf8
                const char op_e[] ={ char(0xC3),char(0xA8), '\0' };
            #else
                //è = E8 in ASCCI-Latin
                const char op_e[] = { char(0xE8), '\0' };
            #endif
            strcmp_skip(ptr,op_e);
            //jump space
            skip_space_end_comment(ptr);
            //compare
            if(CSTRCMP_SKIP(ptr, "minore"))
            {
                //jump space
                skip_space_end_comment(ptr);
                //search di
                CSTRCMP_SKIP(ptr,"di");
                //return
                op_return =
                {
                    m_line,
                    "<",
                    true,
                    ptr
                };
            }
            else if(CSTRCMP_SKIP(ptr, "maggiore"))
            {
                //jump space
                skip_space_end_comment(ptr);
                //jump 'di'
                CSTRCMP_SKIP(ptr,"di");
                //return
                op_return =
                {
                    m_line,
                    ">",
                    true,
                    ptr
                };
            }
            else if(CSTRCMP_SKIP(ptr, "uguale"))
            {
                //jump space
                skip_space_end_comment(ptr);
                //jump 'a'
                CSTRCMP_SKIP(ptr,"a");
                //return
                op_return =
                {
                    m_line,
                    "==",
                    true,
                    ptr
                };
            }
            //reset
            m_line = tmp_line;
            //return
            return op_return;
        }
		//parser compare
		bool parse_compare(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			//value node
			syntactic_tree::exp_node *left = nullptr;
			syntactic_tree::exp_node *logiccomp = nullptr;
			syntactic_tree::exp_node *right = nullptr;
			//skip
			skip_space_end_comment(ptr);
			//parse value
			if (!parse_summinus(ptr, left))   return false;
			//skip
			skip_space_end_comment(ptr);
            // logicComp := (str oprations|'<'|'>'|'==')
            if ( CSTRCMP_SKIP(ptr, "<") )
			{
				logiccomp = syntactic_tree::exp(std::string("<"), nullptr, m_line);
            }
            else if( CSTRCMP_SKIP(ptr, ">") )
            {
                logiccomp = syntactic_tree::exp(std::string(">"), nullptr, m_line);
            }
            else if( CSTRCMP_SKIP(ptr, "==") )
            {
                logiccomp = syntactic_tree::exp(std::string("=="), nullptr, m_line);
            }
			else
			{
                //parser a op
                auto op_parsed = parse_compare_string_operation(ptr);
                //success?
                if(op_parsed.m_success)
                {
                    ptr       = op_parsed.m_ptr;
                    m_line    = op_parsed.m_line;
                    logiccomp = syntactic_tree::exp(op_parsed.m_op, nullptr, m_line);
                }
                else
                {
                    node = left;
                    return true;
                }
			}
			//skip
			skip_space_end_comment(ptr);
			//parse value
			if (!parse_summinus(ptr, right))  return false;
			//create exp
			logiccomp->m_left = left;
			logiccomp->m_right = right;
			//save node
			node = logiccomp;
			//success
			return true;
		}
		//parser logic
		bool parse_logic(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			//value node
			syntactic_tree::exp_node *left = nullptr;
			syntactic_tree::exp_node *opnode = nullptr;
			//skip
			skip_space_end_comment(ptr);
			//parse value
			if (!parse_compare(ptr, opnode))   return false;
			//skip
			skip_space_end_comment(ptr);
			//cicle
			while (CSTRCMP(ptr, "e") ||
				   CSTRCMP(ptr, "&&") ||
				   CSTRCMP(ptr, "o") ||
				   CSTRCMP(ptr, "||")  )
			{
				//left node
				left = opnode;
				//op
				if ( CSTRCMP_SKIP(ptr, "e") || CSTRCMP_SKIP(ptr, "&&") )
				{
					opnode = syntactic_tree::exp(std::string("&&"), nullptr, m_line);
				}
				else if( CSTRCMP_SKIP(ptr, "o") || CSTRCMP_SKIP(ptr, "||") )
				{
					opnode = syntactic_tree::exp(std::string("||"), nullptr, m_line);
				}
				//compone node
				opnode->m_left = left;
				//skip
				skip_space_end_comment(ptr);
				//parse value
				if (!parse_compare(ptr, opnode->m_right))  return false;
				//skip
				skip_space_end_comment(ptr);
			}
			//save node
			node = opnode;
			//success
			return true;
		}
		//parser exp
		bool parse_exp(const char*& ptr, syntactic_tree::exp_node*& node)
		{
			/* none */
			return parse_logic(ptr, node);
		}
		/*
		//statments
		italanguage := +(stament end_comand)
		//stament
        staments    := *(stament [end_line]) stament [end_partial]
		stament		:= declaration |  if | call | cicle | operation
        assignment  := ('='|['ed'] 'assegna' 'gli'|'come')
        operation   := assignable assignament exp |
                       'assegna' exp ('a'|'ad') assignable
        declaration := ("dichiara"|"definisci") (variable | variable assignament exp) *( ',' (variable | variable assignament exp))
		if			:= 'se' exp 'allora' staments ['altrimenti' staments]
		call        := ('esegui'|'chiama') variable [('di'|'del'|'con'|'su'|'dalla'|'da') [*(exp ',') exp]]
		cicle       :=  while | for_each
		while       := 'mentre' <exp> 'ripeti' ':' staments
		for_each    := 'per' 'ogni' variable ('in'|'appartenente'|'di') variable ':' staments
		*/
		//parse staments
		bool parse_staments(const char*& ptr, syntactic_tree::list_nodes& nodes)
		{
			//skip
			skip_space_end_comment(ptr);
			//staments
			while (*ptr != '.')
			{
				//parse
				syntactic_tree::node* stament;
				if (!parse_stament(ptr, stament)) return false;
				//push stament
				nodes.push_back(stament);
				//skip
				skip_space_end_comment(ptr);
                //jump commond
                if (*ptr == ',') ++ptr;
                if (*ptr == ';'){ ++ptr; break; }
			}
			return true;
		}
		//parse stament
		bool parse_stament(const char*& ptr, syntactic_tree::node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			//staments
            if (CSTRCMP(ptr, "dichiara") || CSTRCMP(ptr, "definisci")) return parse_declaration(ptr, node);
            if (CSTRCMP(ptr, "se"))									   return parse_if(ptr, node);
            if (CSTRCMP(ptr, "mentre") || CSTRCMP(ptr, "per"))	       return parse_cicle(ptr, node);
            if (is_call(ptr))								           return parse_call(ptr, node);
			if (is_operation(ptr))								       return parse_operation(ptr, node);
			//error
			push_error("not valid stament");
			return false;
		}
		//is field request?
		bool is_a_field(const char* ptr)
		{
			//skip space
			skip_space_end_comment(ptr, false);
			//is a name?
			if (!is_start_name(*ptr)) return false;
			//parse variable
			std::string variable_name;
			if (!parse_name(ptr, &ptr, variable_name)) return false;
			//skip space
			skip_space_end_comment(ptr, false);
			//index start char?
			return is_start_index(*ptr);
		}
		//parse field
		bool parse_field(const char*& ptr, syntactic_tree::node*& node)
		{
			syntactic_tree::exp_node*      exp_node = nullptr;
			syntactic_tree::variable_node* variable_node = nullptr;
			//parse variable string
			std::string variable_name;
			if (!parse_name(ptr, &ptr, variable_name))
			{
				push_error("not valid variable name");
				return false;
			}
			if (!exists_variable(variable_name))
			{
				push_error("variable not declared");
				return false;
			}
			//skip
			skip_space_end_comment(ptr);
			//skip '['
			++ptr;
			//skip
			skip_space_end_comment(ptr);
			//parse exp
			if (!parse_exp(ptr, exp_node))
			{
				push_error("not valid indexing expression (value)");
				return false;
			}
			//skip
			skip_space_end_comment(ptr);
			//end ']'
			if (!is_end_index(*ptr))
			{
				push_error("not valid indexing, not found ']'");
				return false;
			}
			//skip ']'
			++ptr;
			//alloc variable node
			variable_node = syntactic_tree::variable(variable_name, m_line);
			//new field node
			node = syntactic_tree::field(variable_node, exp_node, m_line);
			//correct
			return true;
		}
		//is variable
		bool is_variable(const char*& ptr)
		{
			//skip space
			skip_space_end_comment(ptr, false);
			//is a name?
			if (!is_start_name(*ptr)) return false;
			//is a variable
			return true;
		}
		//parse variable
		bool parse_variable(const char*& ptr, syntactic_tree::node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			//parse variable
			std::string variable_name;
			if (!parse_name(ptr, &ptr, variable_name))
			{
				push_error("not valid variable name");
				return false;
			}
			if (!exists_variable(variable_name))
			{
				push_error("variable not declared");
				return false;
			}
			//return
			node = syntactic_tree::variable(variable_name, m_line);
			//return true...
			return true;
		}
		//is assignable attribute
		bool is_an_assignable_attribute(const char* ptr)
		{
			//is field
			if (is_a_field(ptr)) return true;
			//is variable
			return is_variable(ptr);
		}
		//parse assignable
		bool parse_assignable(const char*& ptr, syntactic_tree::node*& node)
		{
			if (is_a_field(ptr))
			{
				if (parse_field(ptr, node)) return true;
			}
			else if (is_variable(ptr))
			{
				if (parse_variable(ptr, node)) return true;
			}
			//error
			push_error("the assignable attribute isn't valid");
			return false;
		}
		//parse declaration
		bool parse_declaration(const char*& ptr, syntactic_tree::node*& node)
		{
			syntactic_tree::exp_node *exp = nullptr;
			//skip
			skip_space_end_comment(ptr);
			// ("dichiara"|"definisci") 
			if (!CSTRCMP_SKIP(ptr, "dichiara") && !CSTRCMP_SKIP(ptr, "definisci"))
				return false;
            //create node
            node = syntactic_tree::declaretion(m_line);
            //save node
            auto* dec_node = (syntactic_tree::dec_node*)node;
            //parse list of variables
            do
            {
                // skip space
                skip_space_end_comment(ptr);
				//is variable?
				if (!is_variable(ptr)) return false;
                // parse variable name
                std::string variable_name;
                if (!parse_name(ptr, &ptr, variable_name))
                {
                    push_error("not valid variable name");
                    return false;
                }
                //skip space
                skip_space_end_comment(ptr);
                //find assignment?
                if (is_assignment(ptr))
                {
                    // parser
                    parse_assignment(ptr);
                    // exp
                    if (!parse_exp(ptr, exp)) return false;
                }
                //variable olready exist?
                if (exists_variable(variable_name))
                {
                    //dealloc exp
                    if (exp) delete exp;
                    //push error
                    push_error("variable olready declared");
                    return false;
                }
                //push variable
                push_variable(variable_name);
                //build node
                dec_node->append(syntactic_tree::variable(variable_name), exp);
            }
            while(CSTRCMP_SKIP(ptr, ","));
			//is parsed
			return true;
		}
        //is assignment?
        bool is_assignment(const char* ptr)
        {
            //skip space
            skip_space_end_comment(ptr,false);
            // = ?
            if (CSTRCMP_SKIP(ptr, "=")) return true;
            // 'come' ?
            if (CSTRCMP_SKIP(ptr, "come")) return true;
            //jump 'ed' (not required)
            if(CSTRCMP_SKIP(ptr, "ed")) skip_space_end_comment(ptr,false);
            //jump 'assegna'
            if (CSTRCMP_SKIP(ptr, "assegna"))
            {
                //skip space
                skip_space_end_comment(ptr,false);
                //skip
                if(CSTRCMP_SKIP(ptr, "gli")) return true;
            }
            return false;
        }
        //parse assignment
        bool parse_assignment(const char*& ptr)
        {
            //skip
            skip_space_end_comment(ptr);
            // = ?
            if (CSTRCMP_SKIP(ptr, "=")) return true;
            // 'come' ?
            if (CSTRCMP_SKIP(ptr, "come")) return true;
            //jump 'ed' (not required)
            if(CSTRCMP_SKIP(ptr, "ed")) skip_space_end_comment(ptr);
            //jump 'assegna'
            if (CSTRCMP_SKIP(ptr, "assegna"))
            {
                //skip space
                skip_space_end_comment(ptr);
                //skip
                if(CSTRCMP_SKIP(ptr, "gli")) return true;
            }
            return false;
            //error
            push_error("not find = or ['ed'] 'assegna' 'gli'");
            return false;
        }
        //is operation?
        bool is_operation(const char* ptr)
        {
            //skip space
            skip_space_end_comment(ptr,false);
            //is assegna' or variable?
            if( CSTRCMP(ptr,"assegna") ) return true;
            if( is_an_assignable_attribute(ptr) ) return true;
            return false;
        }
		//parse operation
		bool parse_operation(const char*& ptr, syntactic_tree::node*& node)
		{
			//exp
			syntactic_tree::exp_node *exp = nullptr;
			//skip
			skip_space_end_comment(ptr);
            //'assegna' <exp> ('a'|'ad') <variable>
            if(CSTRCMP_SKIP(ptr, "assegna"))
            {
                //skip space
                skip_space_end_comment(ptr);
                // exp
                if (!parse_exp(ptr, exp)) return false;
                //skip space
                skip_space_end_comment(ptr);
                // parse 'a'|'ad'
                if (!CSTRCMP_SKIP(ptr,"ad") && !CSTRCMP_SKIP(ptr,"a"))
                {
                    // dealloc exp
                    if (exp) delete exp;
                    // push error
                    push_error("not find 'a' or 'ad'");
                    return false;
                }
                //skip space
                skip_space_end_comment(ptr);
                // parse variable or field
				syntactic_tree::node *assignable_node = nullptr;
				// is a variable o field?
				if (!parse_assignable(ptr, assignable_node)) return false;
				//build node
				node = syntactic_tree::operation((syntactic_tree::assignable_node*)assignable_node, exp, m_line);
            }
            else
            // <assignable> <assignment> <exp>
            {
				// parse variable or field
				syntactic_tree::node *assignable_node = nullptr;
				// is a variable o field?
				if (!parse_assignable(ptr, assignable_node)) return false;
                //skip space
                skip_space_end_comment(ptr);
                //find assignment ?
				if (!parse_assignment(ptr)) 
				{
					if (assignable_node) delete assignable_node; 
					return false;
				}
                // exp
                if (!parse_exp(ptr, exp))
				{
					if (assignable_node) delete assignable_node;
					return false;
				}
				//build node
				node = syntactic_tree::operation((syntactic_tree::assignable_node*)assignable_node, exp, m_line);
            }
			//is parsed
			return true;
		}
        //parse staments
        bool parse_if_staments(const char*& ptr, syntactic_tree::if_node* if_node)
        {
            //skip
            skip_space_end_comment(ptr);
            //else staments?
            bool else_node =  false;
            //staments
            while (*ptr != '.')
            {
                //skip
                skip_space_end_comment(ptr);
                //is else?
                if (!else_node && CSTRCMP_SKIP(ptr, "altrimenti")) else_node = true;
                //parse
                syntactic_tree::node* stament;
                if (!parse_stament(ptr, stament)) return false;
                //push stament
                if(else_node) if_node->else_append(stament);
                else          if_node->append(stament);
                //skip
                skip_space_end_comment(ptr);
                //jump commond
                if (*ptr == ',') ++ptr;
                if (*ptr == ';'){ ++ptr; break; }
            }
            return true;
        }
		//parse if
		bool parse_if(const char*& ptr, syntactic_tree::node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			// 'se'
			if (!CSTRCMP_SKIP(ptr, "se")) return false;
			// exp
			syntactic_tree::exp_node *exp = nullptr;
			if (!parse_exp(ptr, exp)) return false;
			//skip
			skip_space_end_comment(ptr);
			// 'allora'
			if (!CSTRCMP_SKIP(ptr, "allora"))
			{
				push_error("not found \"allora\" keyword");
				return false;
			}
			//skip
			skip_space_end_comment(ptr);
			//build node
			         node = syntactic_tree::if_else(exp, m_line);
			auto* if_node = (syntactic_tree::if_node*) (node);
			//add sub nodes
			if (!parse_if_staments(ptr, if_node)) return false;
			//else 
			return true;
		}
        //special case (compact call)
        bool is_call(const char* ptr)
        {
            //skip
            skip_space_end_comment(ptr,false);
            // 'esegui'
            if (CSTRCMP_SKIP(ptr, "esegui")||CSTRCMP_SKIP(ptr, "chiama")) return true;
            // or is a call compact
            std::string call_name;
            if (!parse_name(ptr, &ptr, call_name)) return false;
            //skip space
            skip_space_end_comment(ptr,false);
            //and id/con
            if (CSTRCMP(ptr, "di")    ||
                CSTRCMP(ptr, "del")   ||
                CSTRCMP(ptr, "con")   ||
                CSTRCMP(ptr, "su")    ||
                CSTRCMP(ptr, "dalla") ||
                CSTRCMP(ptr, "da")) return true;
            //or isn't a call
            return false;
        }
		//parse call
		bool parse_call(const char*& ptr, syntactic_tree::node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			// 'esegui'
			if (!CSTRCMP_SKIP(ptr, "esegui") &&
                !CSTRCMP_SKIP(ptr, "chiama"))
                if(!is_call(ptr)) //compact form?
                {
                    return false;
                }
			//skip
			skip_space_end_comment(ptr);
			// parse variable name
			std::string call_name;
			if (!parse_name(ptr, &ptr, call_name))
			{
				push_error("not valid functionname");
				return false;
			}
			//build call
			node = syntactic_tree::call(syntactic_tree::variable(call_name), m_line);
			auto* call_node = (syntactic_tree::call_node*)node;
			//skip space
			skip_space_end_comment(ptr);
			// 'di'|'con'
			if (CSTRCMP_SKIP(ptr, "di")   ||
                CSTRCMP_SKIP(ptr, "del")  ||
                CSTRCMP_SKIP(ptr, "con")  ||
                CSTRCMP_SKIP(ptr, "su")   ||
                CSTRCMP_SKIP(ptr, "dalla")||
                CSTRCMP_SKIP(ptr, "da"))
			{
				do
				{
					//skip space
					skip_space_end_comment(ptr);
					//parse exp
					syntactic_tree::exp_node* exp;
					//parse exp
					if (!parse_exp(ptr, exp)) return false;
					//append exp
					call_node->append(exp);
					//skip space
					skip_space_end_comment(ptr);
				} while (CSTRCMP_SKIP(ptr, ","));
			}
			//else 
			return true;
		}
        //parse while
        bool parse_while(const char*& ptr, syntactic_tree::node*& node)
        {
            //skip
            skip_space_end_comment(ptr);
            // 'mentre'
            if (!CSTRCMP_SKIP(ptr, "mentre")) return false;
            // exp
            syntactic_tree::exp_node *exp = nullptr;
            if (!parse_exp(ptr, exp)) return false;
            //skip
            skip_space_end_comment(ptr);
            // 'ripeti'
            if (!CSTRCMP_SKIP(ptr, "ripeti"))
            {
                push_error("not found \"ripeti\" keyword");
                return false;
            }
            //skip
            skip_space_end_comment(ptr);
            // ':'
            if (!CSTRCMP_SKIP(ptr, ":"))
            {
                push_error("not found \":\" keyword");
                return false;
            }//skip
            skip_space_end_comment(ptr);
            //build node
			            node = syntactic_tree::while_do(exp, m_line);
            auto* while_node = (syntactic_tree::while_node*) (node);
            //add sub nodes
            if (!parse_staments(ptr, while_node->m_staments)) return false;
            //success
            return true;
        }
        //parse for
        bool parse_for(const char*& ptr, syntactic_tree::node*& node)
        {
            return false;
        }
		//parse cicle
		bool parse_cicle(const char*& ptr, syntactic_tree::node*& node)
        {
            if (CSTRCMP(ptr, "mentre"))		return parse_while(ptr, node);
            if (CSTRCMP(ptr, "per"))		return parse_for(ptr, node);
			return false;
		}
		//parse itlanguage
		bool parse_italanguage(const char*& ptr, syntactic_tree::root_node*& node)
		{
			//skip
			skip_space_end_comment(ptr);
			//staments
			while (*ptr != EOF && *ptr!='\0')
			{
				//parse
				syntactic_tree::node* stament;
				if (!parse_stament(ptr, stament)) return false;
				//push stament
				node->append(stament);
				//skip
				skip_space_end_comment(ptr);
				//jump commond
				if (!CSTRCMP_SKIP(ptr, "."))
				{
					push_error("not find the end of a stament (n.b. \'.\')");
					return false;
				}
				//skip
				skip_space_end_comment(ptr);
			}
			return true;
		}
		//parse language
		bool italanguage(const std::string& source, syntactic_tree& tree)
		{
            //stat at line 1
            m_line = 1;
            //contant string ptr
			const char* source_ptr = source.c_str();
            //start parsing
			return parse_italanguage(source_ptr, tree.m_root);
		}
	};
};