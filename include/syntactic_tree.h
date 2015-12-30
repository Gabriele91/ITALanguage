//
//  syntactic_tree.h
//  ITALanguage
//
//  Created by Gabriele Di Bari on 12/12/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//
#pragma once
#include <vector>
#include <string>

namespace it_language
{
	//tree
	class syntactic_tree
	{
	public:

		enum node_type
		{
			NONE_NODE,
			ROOT_NODE,
			DEC_NODE,
			OP_NODE,
			CALL_NODE,
			IF_NODE,
			WHILE_NODE,
			FOR_NODE,
			EXP_NODE,
			VALUE_NODE,
			CONSTANT_NODE,
			ASSIGNABLE_NODE,
			VARIABLE_NODE,
			FIELD_NODE
		};

		//class declaretion
		class node;
		class dec_node;
		class op_node;
		class if_node;
		class while_node;
		class for_node;
		class exp_node;
		class value_node;
		class constant_node;
		class assignable_node;
		class variable_node;
		class field_node;
		//node list
		using list_nodes = std::vector< node* >;
		using list_exps  = std::vector< exp_node* >;

		//besic node
		class node
		{
		public:

			size_t    m_line{ 0 };
			size_t    m_char{ 0 };
			node_type m_type{ NONE_NODE };

			virtual bool is(node_type type) const
			{
				return m_type == type;
			}

			template < class T >
			T* to()
			{
				return (T*)this;
			}
            
            virtual ~node()
            {
                //none
            }
		};

		//root node
		class root_node : public node
		{
		public:
			//statments
			list_nodes m_staments;
			//root
			root_node()
			{
				m_type = ROOT_NODE;
			};
			//append statment
			virtual root_node* append(node* node)
			{
				m_staments.push_back(node);
				return this;
			}
            
            virtual ~root_node()
            {
                for(auto& node : m_staments) delete node;
            }
		};

		//dec node
		class dec_node : public node
		{
		public:
			//variable [and exp]
            struct dec_field
            {
                variable_node* m_variable{ nullptr };
                exp_node*      m_exp     { nullptr };
                
                dec_field(variable_node* variable,exp_node* exp)
                {
                    m_variable = variable;
                    m_exp      = exp;
                }
            };
            //list declaretion
            std::list < dec_field > m_decs;
			//declaretion node
			dec_node()
			{
				m_type = DEC_NODE;
			};
            //append a declaretion
            void append(const dec_field& field)
            {
                m_decs.push_back(field);
            }
            
            void append(variable_node* variable,exp_node* exp)
            {
                m_decs.push_back(std::move(dec_field(variable,exp)));
            }
            
            
            virtual ~dec_node()
            {
                for(dec_field& dec : m_decs)
                {
                    if(dec.m_variable) delete dec.m_variable;
                    if(dec.m_exp)      delete dec.m_exp;
                }
            }
		};

		//op node
		class op_node : public node
		{
		public:

			//variable and exp
			assignable_node* m_assignable{ nullptr };
			exp_node*        m_exp{ nullptr };

			//op node
			op_node()
			{
				m_type = OP_NODE;
			}
            
            virtual ~op_node()
            {
                if(m_assignable) delete m_assignable;
                if(m_exp)        delete m_exp;
            }

		};

        //while node
        class while_node : public node
        {
        public:
            
            //while exp, staments
            exp_node*  m_exp{ nullptr };
            list_nodes m_staments;
            
            //if node
            while_node()
            {
                m_type = WHILE_NODE;
            }
            
            //append statment
            virtual while_node* append(node* node)
            {
                m_staments.push_back(node);
                return this;
            }
            
            virtual ~while_node()
            {
                if(m_exp)                    delete m_exp;
                for(auto& node : m_staments) delete node;
            }
            
        };
		//if node
		class if_node : public node
		{
		public:

			//if exp, staments, [else exp, staments]
			exp_node*  m_exp{ nullptr };
			list_nodes m_staments;
			//additionals
			list_nodes m_else_staments;

			//if node
			if_node()
			{
				m_type = IF_NODE;
			};

			//append statment
			virtual if_node* append(node* node)
			{
				m_staments.push_back(node);
				return this;
			}

			//append statment else node
			virtual if_node* else_append(node* node)
			{
				m_else_staments.push_back(node);
				return this;
			}

			//have a else
			bool is_only_if() const
			{
				return !m_else_staments.size();
			}
            
            virtual ~if_node()
            {
                if(m_exp)                         delete m_exp;
                for(auto& node : m_staments)      delete node;
                for(auto& node : m_else_staments) delete node;
            }
		};

		//expression
		class exp_node : public node
		{
		public:
			//name
			std::string m_name;
			exp_node*   m_left{ nullptr };
			exp_node*   m_right{ nullptr };
			//value node
			exp_node()
			{
				m_type = EXP_NODE;
			};
			//is one exp
			bool is_one() const
			{
				return m_left && !m_right;
			}
			//not applay operation
			bool is_link() const
			{
				return is_one() && m_name == "";
			}
			//overwrite
			virtual bool is(node_type type) const
			{
				return EXP_NODE == type ||
					   ASSIGNABLE_NODE == type ||
					   VALUE_NODE == type ||
					   CONSTANT_NODE == type ||
					   VARIABLE_NODE == type ||
					   FIELD_NODE == type ||
					   CALL_NODE == type;
            }
            
            virtual ~exp_node()
            {
                if(m_left)   delete m_left;
                if(m_right)  delete m_right;
            }
		};

		//op node
		class call_node : public exp_node
		{
		public:

			//variable and exp
			variable_node* m_variable{ nullptr };
			list_exps      m_args;

			//op node
			call_node()
			{
				m_type = CALL_NODE;
			};

			//append statment
			virtual call_node* append(exp_node* node)
			{
				m_args.push_back(node);
				return this;
            }
            
            virtual ~call_node()
            {
                if(m_variable)              delete m_variable;
                for(auto& node : m_args)    delete node;
            }
		};

		//value node
		class value_node : public exp_node
		{
		public:
			//next node
			exp_node*      m_exp{ nullptr };
			//value node
			value_node()
			{
				m_type = VALUE_NODE;
			};

			bool is_variable() const
			{
				return m_exp->m_type == VARIABLE_NODE;
			}

			bool is_field() const
			{
				return m_exp->m_type == FIELD_NODE;
			}

			bool is_constant() const
			{
				return m_exp->m_type == CONSTANT_NODE;
			}

			bool is_exp() const
			{
				return m_exp->m_type == EXP_NODE;
            }
            
            virtual ~value_node()
            {
                if(m_exp)   delete m_exp;
            }
		};
		//costant
		class constant_node : public exp_node
		{
		public:
			//costant type
			enum constant_type
			{
				INT,
				FLOAT,
				STRING
			};
			//costant value
			struct constant_value
			{
				union
				{
					float m_float{ 0 };
					int   m_int;
				};
				std::string m_string;
			};
			//value
			constant_value  m_value;
			constant_type   m_const_type;
			//value node
			constant_node()
			{
				m_type = CONSTANT_NODE;
			};
		};
		//assignable
		class assignable_node : public exp_node
		{

		public:
			//value node
			assignable_node()
			{
				m_type = ASSIGNABLE_NODE;
			};
			//to variable node
			variable_node* to_variable_node() const
			{
				return (variable_node*) this;
			}
			//to field node
			field_node* to_field_node() const
			{
				return (field_node*) this;
			}
		};
		//variable
		class variable_node : public assignable_node
		{
		public:
			//name
			std::string m_name;
			//value node
			variable_node()
			{
				m_type = VARIABLE_NODE;
			};
		};

		//field node
		class field_node : public assignable_node
		{
		public:
			//values
			assignable_node* m_assignable{ nullptr };
			exp_node*	     m_exp{ nullptr };
			//field node
			field_node()
			{
				m_type = FIELD_NODE;
			};
            
            virtual ~field_node()
            {
                if(m_assignable) delete m_assignable;
                if(m_exp)        delete m_exp;
            }
		};

		//utilities

		//variable
		static variable_node* variable(const std::string& name, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new variable_node;
			node->m_name = name;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		//constant
		static constant_node* constant(int value, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new constant_node;
			node->m_value.m_int = value;
			node->m_const_type = constant_node::constant_type::INT;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}
		static constant_node* constant(float value, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new constant_node;
			node->m_value.m_float = value;
			node->m_const_type = constant_node::constant_type::FLOAT;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}
		static constant_node* constant(const std::string& value, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new constant_node;
			node->m_value.m_string = value;
			node->m_const_type     = constant_node::constant_type::STRING;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}
		static constant_node* constant(const constant_node::constant_value& value,
								       constant_node::constant_type type, 
									   size_t line = 0, 
									   size_t ichar = 0)
		{
			auto* node = new constant_node;
			node->m_value       = value;
			node->m_const_type  = type;
			node->m_line        = line;
			node->m_char        = ichar;
			return node;
		}

		//field
		static field_node* field(assignable_node* var_to_field, exp_node* exp_field, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new field_node;
			node->m_assignable = var_to_field;
			node->m_exp        = exp_field;
			node->m_line       = line;
			node->m_char       = ichar;
			return node;
		}

		//call
		static call_node* call(variable_node* var_to_call, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new call_node;
			node->m_variable = var_to_call;
			node->m_line     = line;
			node->m_char     = ichar;
			return node;
		}

		//value
		static value_node* value(variable_node* var_node, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new value_node;
			node->m_exp = var_node;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		static value_node* value(constant_node* var_node, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new value_node;
			node->m_exp = var_node;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		static value_node* value(exp_node* var_node, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new value_node;
			node->m_exp = var_node;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		//expression
		static exp_node* exp(const std::string& name, exp_node* left, exp_node* right, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new exp_node;
			node->m_name = name;
			node->m_left = left;
			node->m_right = right;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		static exp_node* exp(const std::string& name, exp_node* left, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new exp_node;
			node->m_name = name;
			node->m_left = left;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		static exp_node* exp(exp_node* next, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new exp_node;
			node->m_left = next;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

        //declaretion stament
        static dec_node* declaretion(size_t line = 0, size_t ichar = 0)
        {
            auto* node = new dec_node;
            node->m_line = line;
            node->m_char = ichar;
            return node;
        }
        static dec_node* declaretion(variable_node* variable, size_t line = 0, size_t ichar = 0)
        {
            auto* node = new dec_node;
            node->append(variable,nullptr);
            node->m_line = line;
            node->m_char = ichar;
            return node;
        }
		static dec_node* declaretion(variable_node* variable, exp_node* exp, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new dec_node;
            node->append(variable,exp);
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		//operation
		static op_node* operation(assignable_node* assignable, exp_node* exp, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new op_node;
			node->m_assignable = assignable;
			node->m_exp = exp;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		static op_node* assignment(assignable_node* assignable, exp_node* exp, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new op_node;
			node->m_assignable = assignable;
			node->m_exp = exp;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}
        //while stament
        static while_node* while_do(exp_node* if_exp, size_t line = 0, size_t ichar = 0)
        {
            auto* node = new while_node;
            node->m_exp  = if_exp;
            node->m_line = line;
            node->m_char = ichar;
            return node;
        }
		//if stament
		static if_node* if_else(exp_node* if_exp, size_t line = 0, size_t ichar = 0)
		{
			auto* node = new if_node;
			node->m_exp = if_exp;
			node->m_line = line;
			node->m_char = ichar;
			return node;
		}

		//tree root
		root_node* m_root = { new root_node };
		//append stament
		root_node* append(node* node)
		{
			m_root->append(node);
			return m_root;
		}
        //delete root
        virtual ~syntactic_tree()
        {
            delete m_root;
        }
	};
};