
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

#include<string>
#include<fstream>
#include<iostream>

using namespace std;

#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Procedure::Procedure(Data_Type proc_return_type, string proc_name, list<Symbol_Table_Entry * > plist)
{
	//evaluated = *new list<Eval_Result * >();
	return_type = proc_return_type;
	name = proc_name;
    parameter_list = plist;
}

Procedure::~Procedure()
{
	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		delete (*i);
}

string Procedure::get_proc_name()
{
	return name;
}

void Procedure::set_basic_block_list(list<Basic_Block *> bb_list)
{
	basic_block_list = bb_list;
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
	list<Symbol_Table_Entry * >  parameter_list_temp = new_list.variable_table;
	list<Symbol_Table_Entry * >::iterator it=parameter_list_temp.begin();
    for(;it!=parameter_list_temp.end();it++) {
        if((*it)) {
            local_symbol_table.push_symbol(*it);
        }
    }
	local_symbol_table.set_table_scope(local);
    //list<Symbol_Table_Entry * > parameter_list_temp;
    
}

void Procedure::push_parameter_list(){

local_symbol_table.set_table_scope(local);

	list<Symbol_Table_Entry * >::iterator it=parameter_list.begin();
    for(;it!=parameter_list.end();it++) {
        if((*it)) {
            local_symbol_table.push_symbol(*it);
        }
    }
}


Data_Type Procedure::get_return_type()
{
	return return_type;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	return local_symbol_table.get_symbol_table_entry(variable_name);
}

void Procedure::print_ast(ostream & file_buffer)
{
	file_buffer << "\n" <<PROC_SPACE << "Procedure: "<<this->get_proc_name() << "\n\n";

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_bb(file_buffer);
}

void Procedure::put_variable_value(Local_Environment & eval_env) {
    list<Eval_Result* >::iterator it=evaluated.begin();
    list<Symbol_Table_Entry * >::iterator it1=parameter_list.begin();
    if((*it1)==NULL) return;
    for(; it!=evaluated.end()||it1!=parameter_list.end();it++,it1++) {
        eval_env.put_variable_value(**it,(*it1)->get_variable_name());
        }
}
	
Basic_Block & Procedure::get_start_basic_block()
{
	list<Basic_Block *>::iterator i;
	i = basic_block_list.begin();
	return **i;
}

Basic_Block * Procedure::get_next_bb(Basic_Block & current_bb)
{
	bool flag = false;

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	{
		if((*i)->get_bb_number() == current_bb.get_bb_number())
		{
			flag = true;
			continue;
		}
		if (flag)
			return (*i);
	}
	
	return NULL;
}

Basic_Block * Procedure::goto_bb(Basic_Block & current_bb, Eval_Result* result)
{
	bool flag = false;
	list<Basic_Block *>::iterator i;
    if(result->get_result_enum()==goto_result) {
        int goto_bbval = result->get_value();
        for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
        {
            if((*i)->get_bb_number() == goto_bbval)
            {
                return (*i);
            }
        }
    }
    else {

        for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
        {
            if((*i)->get_bb_number() == current_bb.get_bb_number())
            {
                flag = true;
                continue;
            }
            if (flag)
                return (*i);
        }
    }

    return NULL;
}

Eval_Result & Procedure::evaluate(ostream & file_buffer)
{
	Local_Environment * eval_env =  new Local_Environment();
	
	put_variable_value(*eval_env);
	local_symbol_table.create(*eval_env);
	
	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure " << "<< "<<name<<" >>" << "\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
	eval_env->print(file_buffer);
	file_buffer << "\n";
	Basic_Block * current_bb = &(get_start_basic_block());
	while (current_bb)
	{
        
		result = &(current_bb->evaluate(*eval_env, file_buffer));
		current_bb = goto_bb(*current_bb, result);		
	}

	file_buffer << "\n\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating) Function: << "<<name<<" >>\n";
	eval_env->print(file_buffer);
    if(result->get_result_enum()!=return_result_void) {
        if(result->get_result_enum()==int_result)  {
           file_buffer<<AST_NODE_SPACE<<"return : "<<result->main_result_int<<"\n";
        }
        else if(result->get_result_enum()==float_result) {
            file_buffer<<AST_NODE_SPACE<<"return : "<<result->main_result_float<<"\n";
        }
    }
	//eval_env->print(file_buffer);

	return *result;
}

