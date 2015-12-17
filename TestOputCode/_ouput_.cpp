

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
    

    
int main()
{
it_variable x; 
x = lettura("qualcosa");
it_variable indice=0.000000f; 
it_variable n=intero(inserimento("numero di ripetizioni")); 
while(((indice)<(n))||((indice)==(n)))
{
stampa(x,"\n"); 
indice = (indice)+(1.000000f);
}
return 0;
}
