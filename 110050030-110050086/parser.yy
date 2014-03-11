
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
	int integer_value;
	float float_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	list<Symbol_Table_Entry *> *symbol_entry_list;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER
%token <integer_value> BASIC_BLOCK
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE VOID IF ELSE GOTO ASSIGN_OP   

%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <symbol_entry_list> parameter_statement_list
%type <ast> assignment_statement
%type <ast> goto_statement
%type <ast> unary_expression
%type <ast> if_statement
%type <ast> variable
%type <ast> variable1
%type <ast> for_precedence
%type <ast> constant
%type <ast_list> arg_list
%type <ast> any
%type <ast> relational_expression
%type <ast> arithmetic_expression
%type <symbol_entry> parameter_statement

%left EQ NE
%left GT GE LT LE
%left '+' '-'
%left '*' '/'


%start program

%%

program:
	func_decl_statement_list procedure_list
	{
		// #if 0
		
		// #endif
	}
|
	declaration_statement_list func_decl_statement_list procedure_list
	{
		// #if 0
		program_object.set_global_table(*$1);
		if (!$1)
			report_internal_error("parsing wrong");

		$1->global_list_in_proc_map_check(get_line_number()); // checks the presence of variable name is not same as any function

		delete $1 ;
		// #endif
	}
|
	procedure_list
	{
		// #if 0
		
		// #endif
	}
;
	
procedure_list:
	procedure_list procedure_fun
|
	procedure_fun
;

procedure_fun:
	procedure_name
	{
	return_statement_used_flag = false;
	}
	procedure_body
	{
		// #if 0
		
		
		// #endif
	};


procedure_name:
	NAME '(' parameter_statement_list ')'
		{
		// #if 0
			
		// error checking for existence
				current_procedure = program_object.get_procedure(*$1);
		
		// #endif
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		// #if 0
		current_procedure->set_local_list(*$2);
		delete $2;
		// #endif
	}
	basic_block_list '}'
	{
		// #if 0
		current_procedure->set_basic_block_list(*$4);

		delete $4;
		// #endif
	}
|
	'{' basic_block_list '}'
		{
		// #if 0
		current_procedure->set_basic_block_list(*$2);

		delete $2;
		// #endif
		}
;

declaration_statement_list:
	declaration_statement
	{
		// #if 0
		int line = get_line_number();
		if (!$1)
			report_error("declaration_statement cannot be null", line);

		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
			report_error("Variable name cannot be same as procedure name", line);

		$$ = new Symbol_Table();
		$$->push_symbol($1);
		// #endif
	}
|
	declaration_statement_list declaration_statement
	{
		// #if 0
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		int line = get_line_number();
		if (!$2)
			report_error("declaration statement list cannot be null", line);

		program_object.variable_in_proc_map_check($2->get_variable_name(), line);

		string var_name = $2->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
			report_error("Variable name cannot be same as procedure name", line);

		if ($1)
		{
			if($1->variable_in_symbol_list_check(var_name))
				report_error("Variable is declared twice", line);

			$$ = $1;
		}

		else
			$$ = new Symbol_Table();

		$$->push_symbol($2);
		// #endif
	}
;

func_decl_statement_list:
	func_decl_statement_list function_declaration 
|	
	function_declaration
	{
		// #if 0
		// #endif
	}
;

function_declaration:
	INTEGER NAME '(' parameter_statement_list ')' ';'
	{
		// #if 0
		Procedure *temp = new Procedure (int_data_type,*$2,*$4);
		program_object.set_procedure_map(*temp);
		
		// #endif
	}
|
	FLOAT NAME '(' parameter_statement_list ')' ';'
	{
		// #if 0
		Procedure *temp = new Procedure (float_data_type,*$2,*$4);
		program_object.set_procedure_map(*temp);
		
		// #endif
	}
|
	VOID NAME '(' parameter_statement_list ')' ';'
	{
		// #if 0
		Procedure *temp = new Procedure (void_data_type,*$2,*$4);
		program_object.set_procedure_map(*temp);
		
		// #endif
	}
;

parameter_statement_list:
	parameter_statement
	{
		// #if 0
		$$=new list<Symbol_Table_Entry* >();
		$$->push_back($1);	
		// #endif
	}
|
	 parameter_statement_list ',' parameter_statement 
	{
		// #if 0
		if ($1){
			$$=$1;
		}
		else{
			$$=new list<Symbol_Table_Entry* >();
		}	
		$$->push_back($3);	
		// #endif
	}
;

parameter_statement:
	INTEGER NAME
	{
		// #if 0
		$$ = new Symbol_Table_Entry(*$2,int_data_type);
		
		// #endif
	}
|
	FLOAT NAME
	{
		// #if 0
		$$ = new Symbol_Table_Entry(*$2,float_data_type);
		// #endif
	}

|

	{
		// #if 0
		$$ = NULL;
		
		// #endif
	}
;

declaration_statement:
	INTEGER NAME ';'
	{
		 // #if 0
		if(!$2)
			report_error("variable name cannot be null", get_line_number());

		$$ = new Symbol_Table_Entry(*$2, int_data_type);
		delete $2;
		// #endif
	}
|
	FLOAT NAME ';'
	{
	 // #if 0
		if(!$2)
			report_error("variable name cannot be null", get_line_number());

		$$ = new Symbol_Table_Entry(*$2, float_data_type);
		delete $2;
		 // #endif
	}
;



basic_block_list:
	basic_block_list basic_block
	{
		 // #if 0
		if (!$2)
			report_error("Basic block doesn't exist", get_line_number());

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
		// #endif
	}
|
	basic_block
	{
		// #if 0
		if (!$1)
			report_error("Basic block doesn't exist", get_line_number());

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
		// #endif
	}
	
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
	{
		// #if 0
		$$ = new Basic_Block($1,*$3);
		// #endif
	}
/*|
	BASIC_BLOCK ':' if_statement
	{
		// #if 0
		$$ = new Basic_Block($1,*$3);
		// #endif
	}*/

	
;


executable_statement_list:
	assignment_statement_list
	{
		// #if 0
		$$ = $1;
		// #endif
	}
|
	assignment_statement_list RETURN ';'
	{
		// #if 0
		Ast * ret = new Return_Ast();

		if ($1)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
		// #endif
	}
| 
	assignment_statement_list RETURN for_precedence';'
	{
		// #if 0
		Ast * ret = new Return_Ast($3);

		if ($1)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
		// #endif
	}
|
	
assignment_statement_list if_statement
{
		// #if 0
		if ($1)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back($2);
		 // #endif
	}
|  
assignment_statement_list goto_statement
{
		// #if 0
		if ($1)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back($2);
		 // #endif
}
;


if_statement:
	
	IF '('for_precedence')'
		GOTO BASIC_BLOCK ';'
	ELSE
		GOTO BASIC_BLOCK ';'
{
		// #if 0
			$$ = new Conditional_Goto_Ast($3, $6, $10);
		// #endif
}
;

goto_statement:
	GOTO BASIC_BLOCK ';'
	{
		// #if 0
		$$ = new Unconditional_Goto_Ast($2);
		
		// #endif
	}
;

relational_expression:
	arithmetic_expression
	{
		// #if 0
		$$ = $1;
		// #endif
	} 
|
	relational_expression LE relational_expression
	{
		// #if 0
		$$ = new Relation_Expression_Ast( $1,  $3, relop::LE);
		// #endif
	}
|
	relational_expression GE relational_expression
	{
		 // #if 0
		$$ = new Relation_Expression_Ast((Ast *) $1, (Ast *) $3, relop::GE);
		// #endif
	}

|
	relational_expression LT relational_expression
	{
		// #if 0
		$$ = new Relation_Expression_Ast((Ast *) $1, (Ast *) $3, relop::LT);
		// #endif
	}
|
	relational_expression GT relational_expression
	{
		// #if 0
		$$ = new Relation_Expression_Ast((Ast *) $1, (Ast *) $3, relop::GT);
		// #endif
	}

;


arithmetic_expression:
	arithmetic_expression '+' arithmetic_expression
	{
			// #if 0
		$$= new Arithmetic_Expression_Ast((Ast *) $1, (Ast *) $3, arithop::PLUS);
		// #endif
	}
|
	arithmetic_expression '-' arithmetic_expression
	{
			// #if 0
		$$=new Arithmetic_Expression_Ast((Ast *) $1, (Ast *) $3, arithop::MINUS);
		// #endif
	}
|
	arithmetic_expression '*' arithmetic_expression
	{
			// #if 0
		$$=new Arithmetic_Expression_Ast((Ast *) $1, (Ast *) $3, arithop::MULT);
		// #endif
	}
|
	arithmetic_expression '/' arithmetic_expression
	{
			// #if 0
		$$=new Arithmetic_Expression_Ast((Ast *) $1, (Ast *) $3, arithop::DIV);
		// #endif
	}
|
	unary_expression
	{
			// #if 0
		$$=$1;
		// #endif 
	}
|
'('for_precedence')'
	{
			// #if 0
		$$=$2;
		// #endif
	}
|
	any
	{
			// #if 0
		$$=$1;
		// #endif
	}
;

unary_expression:
	'-' any
	{
		$$ = new Unary_Expression_Ast((Ast *)$2);
	}
|
	'-' '('for_precedence')'
	{
		$$ = new Unary_Expression_Ast((Ast *)$3);
	}
|
	'('FLOAT')' '('arithmetic_expression')'
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $5, typecast_type::FLOAT);
		// #endif 
	}
|
	'('DOUBLE')' '('arithmetic_expression')'
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $5, typecast_type::DOUBLE);
		// #endif
	}
|
	'('INTEGER')' '('arithmetic_expression')'
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $5, typecast_type::INTEGER);
		// #endif
	}
	|
	'('FLOAT')' any
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $4, typecast_type::FLOAT);
		// #endif 
	}
|
	'('DOUBLE')' any
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $4, typecast_type::DOUBLE);
		// #endif
	}
|
	'('INTEGER')' any
	{
			// #if 0
		$$=new Typecast_Ast((Ast *) $4, typecast_type::INTEGER);
		// #endif
	}
;

for_precedence:
	relational_expression
	{
		// #if 0
		$$=$1;
		// #endif
	}
|
	for_precedence NE for_precedence
	{
		// #if 0
		$$ = new Relation_Expression_Ast((Ast *) $1, (Ast *) $3, relop::NE);
		// #endif
	}
|
	for_precedence EQ for_precedence
	{
		// #if 0
		$$ = new Relation_Expression_Ast((Ast *) $1, (Ast *) $3, relop::EQ);
		// #endif
	}

;
	
assignment_statement_list:
{
		// #if 0
		$$ = NULL;
	// #endif
		
	}
|
	assignment_statement_list assignment_statement
	{
		// #if 0
		if (!$1)
			$$ = new list<Ast *>;

		else
			$$ = $1;

		$$->push_back($2);
		// #endif
	}
;


arg_list:
	for_precedence
	{
	// #if 0
	$$= new list<Ast *>;
	$$->push_back($1);
	// #endif
	}

| 
	arg_list ',' for_precedence
	{
	// #if 0
	if ($1) {
		$$ = $1;
	}
	else
		$$ = new list<Ast *>;
	$$->push_back($3);
	// #endif
	}
|
	{
	// #if 0
	$$=new list<Ast *>;
	$$->clear();
	// #endif
	}
;

assignment_statement:
	variable1 ASSIGN_OP for_precedence ';'
	{
	// #if 0
		if (!$1 || !$3)
			report_error("lhs/rhs cannot be null", get_line_number());

		$$ = new Assignment_Ast($1, $3);
		//std::cout<<$1 <<":\t:"<<$3<<std::endl;

		int line = get_line_number();
		$$->check_ast(line);
	// #endif
	}
|
	NAME '(' arg_list ')' ';'
	{
	// #if 0
		$$ = new Function_Call_Ast(*$3,program_object.get_procedure(*$1));
	// #endif
	}
;


variable1:
	NAME
	{
		// #if 0
		Symbol_Table_Entry var_table_entry;
		if(!$1)
			report_error("variable name cannot be null", get_line_number());

		if (current_procedure->variable_in_symbol_list_check(*$1))
			 var_table_entry = current_procedure->get_symbol_table_entry(*$1);

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = program_object.get_symbol_table_entry(*$1);

		else
			report_error("Variable has not been declared", get_line_number());

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
		// #endif
	}
;

variable:
	variable1
	{
	// #if 1
	$$=$1;
	// #endif
	}
|
	NAME '(' arg_list ')'
	{
	// #if 0
	$$ = new Function_Call_Ast(*$3,program_object.get_procedure(*$1));
	// #endif
	}
;

constant:
	INTEGER_NUMBER
	{
		// #if 0
		$$ = new Number_Ast<int>($1, int_data_type);
		// #endif
	}
|
	FLOAT_NUMBER
	{
			// #if 0
		$$ = new Number_Ast<float>($1, float_data_type);
		// #endif
	}
;

any:
	variable
	{
		 // #if 0
		 	$$ = $1;
		 // #endif
	}
|
	constant
	{
		 // #if 0
		$$ = $1;
		 // #endif
	}

;


