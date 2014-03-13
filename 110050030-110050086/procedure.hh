
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

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

#ifndef PROCEDURE_HH
#define PROCEDURE_HH

#include<string>
#include<map>
#include<list>
#include "basic-block.hh"
#include "symbol-table.hh"


#define PROC_SPACE "   "
#define LOC_VAR_SPACE "      "

using namespace std;

class Procedure;

class Procedure
{
	public:
	Data_Type return_type;
	string name;
	Symbol_Table local_symbol_table;
	list<Basic_Block *> basic_block_list;
     list<Eval_Result * > evaluated ;
    list<Symbol_Table_Entry * > parameter_list;
    
	Procedure(Data_Type proc_return_type, string proc_name, list<Symbol_Table_Entry * > plist);
	~Procedure();

	string get_proc_name();
	void set_basic_block_list(list<Basic_Block *> bb_list);
	void set_local_list(Symbol_Table & new_list);
	Data_Type get_return_type();
	Symbol_Table_Entry & get_symbol_table_entry(string variable_name);
    Basic_Block * goto_bb(Basic_Block & current_bb, Eval_Result* result);
	void print_ast(ostream & file_buffer);
    void put_variable_value(Local_Environment & eval_env);
	void push_parameter_list();
	
	Basic_Block * get_next_bb(Basic_Block & current_bb);
	Basic_Block & get_start_basic_block();

	Eval_Result & evaluate(ostream & file_buffer);

	bool variable_in_symbol_list_check(string variable);
};

#endif
