# ITALanguage
Programming language in italian, created for teaching.

#EBNF

    italanguage := +(stament end_comand)
    staments    := *(stament [end_line]) stament [end_partial]
    stament     := declaration |  if | call | cicle | operation
    assignment  := ('='|['ed'] 'assegna' 'gli'|'come')
    operation   := variable assignament exp | 
                   'assegna' exp ('a'|'ad') variable
  	declaration := ("dichiara"|"definisci") (variable | variable assignament exp) *( ',' (variable | variable assignament exp))
  	if          := 'se' exp 'allora' staments ['altrimenti' staments]
    call        := ('esegui'|'chiama') variable [('di'|'del'|'con'|'su'|'dalla'|'da') [*(exp ',') exp]]
  	cicle       :=  while | for_each
  	while       := 'mentre' <exp> 'ripeti' ':' staments
  	for_each    := 'per' 'ogni' variable ('in'|'appartenente'|'di') variable ':' staments

#LICENSES
    MIT
    
    Copyright (c) 2013 Gabriele Di Bari
    
    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:
    
    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
