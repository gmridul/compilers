
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

#include<iostream>
#include<fstream>
#include<cassert>

using namespace std;

#include"user-options.hh"
#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast(int line)
{
    report_internal_error("Should not reach, Ast : check_ast");
}

Data_Type Ast::get_data_type()
{
    report_internal_error("Should not reach, Ast : get_data_type");
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    report_internal_error("Should not reach, Ast : print_value");
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    report_internal_error("Should not reach, Ast : get_value_of_evaluation");
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
    report_internal_error("Should not reach, Ast : set_value_of_evaluation");
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs)
{
    lhs = temp_lhs;
    rhs = temp_rhs;
}

Assignment_Ast::~Assignment_Ast()
{
    delete lhs;
    delete rhs;
}

Data_Type Assignment_Ast::get_data_type()
{
    return node_data_type;
}

bool Assignment_Ast::check_ast(int line)
{
    if (lhs->get_data_type() == rhs->get_data_type())
    {
        node_data_type = lhs->get_data_type();
        return true;
    }
    report_error("Assignment statement data type not compatible", line);
}

void Assignment_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << AST_SPACE << "Asgn:\n";

    file_buffer << AST_NODE_SPACE"LHS (";
    lhs->print_ast(file_buffer);
    file_buffer << ")\n";
    file_buffer << AST_NODE_SPACE << "RHS (";
    rhs->print_ast(file_buffer);
    file_buffer << ")\n";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result & result = rhs->evaluate(eval_env, file_buffer);
    if (result.is_variable_defined() == false){
        report_error("Variable should be defined to be on rhs", NOLINE);
    }

    lhs->set_value_of_evaluation(eval_env, result);

    // Print the result

    print_ast(file_buffer);
    lhs->print_value(eval_env, file_buffer);

    return result;
}
////////////////////////////////////////////////////////////////

Function_Call_Ast::Function_Call_Ast(list<Ast *> plist, Procedure* proc) {
    parameter_list = plist;
    p = proc;
    if(plist.empty()) parnull=false;
    else parnull=true;
}

Function_Call_Ast::~Function_Call_Ast() {
    delete p;    
}

void Function_Call_Ast::print_ast(ostream & file_buffer) {
    file_buffer<<"FN CALL: "<<p->get_proc_name()<<"(";
    for(list<Ast *>::iterator it=parameter_list.begin();it!=parameter_list.end();it++) {
        (*it)->print_ast(file_buffer);
    }
    file_buffer<<")";
}

Eval_Result & Function_Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    list<Eval_Result * >& evaluated = *new list<Eval_Result * >();
    for(list<Ast *>::iterator it=parameter_list.begin();it!=parameter_list.end();it++) {
        evaluated.push_back(&((*it)->evaluate(eval_env,file_buffer)));
    }
    p->put_variable_value(evaluated);
    return p->evaluate(file_buffer);
}

////////////////////////////////////////////////////////////////

Arithmetic_Expression_Ast::Arithmetic_Expression_Ast(Ast * temp_lhs, Ast * temp_rhs, arithop op) {
    lhs=temp_lhs;
    rhs=temp_rhs;
    aop=op;
}

Arithmetic_Expression_Ast::~Arithmetic_Expression_Ast() {
    delete lhs;
    delete rhs;
}

int Arithmetic_Expression_Ast::arithvalint(int lhsvalue, int rhsvalue) {
    if(aop==PLUS) return (lhsvalue+rhsvalue);
    else if(aop==MINUS) return (lhsvalue-rhsvalue);
    else if(aop==MULT) return (lhsvalue*rhsvalue);
    else if(aop==DIV) return (lhsvalue/rhsvalue);
}
float Arithmetic_Expression_Ast::arithvalfloat(float lhsvalue, float rhsvalue) {
    if(aop==PLUS) {return (lhsvalue+rhsvalue);}
    else if(aop==MINUS) return (lhsvalue-rhsvalue);
    else if(aop==MULT) return (lhsvalue*rhsvalue);
    else if(aop==DIV) {return (lhsvalue/rhsvalue);}
}

Data_Type Arithmetic_Expression_Ast::get_data_type() {
    return lhs->get_data_type();
}

void Arithmetic_Expression_Ast::print_arithop(ostream & file_buffer) {
    if(aop==PLUS) file_buffer<<"PLUS";  
    else if(aop==MINUS) file_buffer<<"MINUS";
    else if(aop==MULT) file_buffer<<"MULT";
    else if(aop==DIV) file_buffer<<"DIV";
}
void Arithmetic_Expression_Ast::print_ast(ostream & file_buffer) {
    file_buffer<<"\n"<<AST_SPACE<<"Arith: ";
    print_arithop(file_buffer);
    file_buffer<<"\n"<<AST_NODE_SPACE<<"LHS (";
    lhs->print_ast(file_buffer);
    file_buffer<<")\n"<<AST_NODE_SPACE<<"RHS (";
    rhs->print_ast(file_buffer);
    file_buffer<<")";
}

Eval_Result & Arithmetic_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result & rhseval = rhs->evaluate(eval_env, file_buffer);
    Eval_Result & lhseval = lhs->evaluate(eval_env, file_buffer);

    if (rhseval.is_variable_defined() == false)
        report_error("Variable should be defined to be on rhs", NOLINE);
    if (lhseval.is_variable_defined() == false)
        report_error("Variable should be defined to be on lhs", NOLINE);

    if(lhs->get_data_type()==int_data_type) {
        int rhsvalue = rhseval.get_value();
        int lhsvalue = lhseval.get_value();
        Eval_Result & result = *new Eval_Result_Value_Int();
        result.set_value(arithvalint(lhsvalue,rhsvalue));
        return result;
    }
    else if(lhs->get_data_type()==float_data_type) {
        float rhsvalue = rhseval.get_floatvalue();
        float lhsvalue = lhseval.get_floatvalue();
        Eval_Result & result = *new Eval_Result_Value_Float();
        result.set_floatvalue(arithvalfloat(lhsvalue,rhsvalue));
        return result;
    }
}
/////////////////////////////////////////////////////////////////
Relation_Expression_Ast::Relation_Expression_Ast(Ast * temp_lhs, Ast * temp_rhs, relop op) {
    lhs=temp_lhs;
    rhs=temp_rhs;
    rop=op;
}

Relation_Expression_Ast::~Relation_Expression_Ast() {
    delete lhs;
    delete rhs;
}

/*bool Relation_Expression_Ast::check_ast(int line)
  {
  if (lhs->get_data_type() == rhs->get_data_type())
  {
  node_data_type = lhs->get_data_type();
  return true;
  }
  report_error("Expression statement data type not compatible", line);
  }*/

bool Relation_Expression_Ast::boolval(float lhsvalue, float rhsvalue) {
    if(rop==LT) return (lhsvalue<rhsvalue); 
    else if(rop==GT) return (lhsvalue>rhsvalue);
    else if(rop==LE) return (lhsvalue<=rhsvalue);
    else if(rop==GE) return (lhsvalue>=rhsvalue);
    else if(rop==NE) return (lhsvalue!=rhsvalue);
    else if(rop==EQ) return (lhsvalue==rhsvalue);
}

Data_Type Relation_Expression_Ast::get_data_type(){
    return int_data_type;
}

void Relation_Expression_Ast::print_relop(ostream & file_buffer) {
    if(rop==LT) file_buffer<<"LT";  
    else if(rop==GT) file_buffer<<"GT";
    else if(rop==LE) file_buffer<<"LE";
    else if(rop==GE) file_buffer<<"GE";
    else if(rop==NE) file_buffer<<"NE";
    else if(rop==EQ) file_buffer<<"EQ";
}

void Relation_Expression_Ast::print_ast(ostream & file_buffer) {
    file_buffer<<"\n"<<AST_SPACE<<"Condition: ";
    print_relop(file_buffer);
    file_buffer<<"\n"<<AST_NODE_SPACE<<"LHS (";
    lhs->print_ast(file_buffer);
    file_buffer<<")\n"<<AST_NODE_SPACE<<"RHS (";
    rhs->print_ast(file_buffer);
    file_buffer<<")";
}

Eval_Result & Relation_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result & rhseval = rhs->evaluate(eval_env, file_buffer);
    Eval_Result & lhseval = lhs->evaluate(eval_env, file_buffer);

    if (rhseval.is_variable_defined() == false)
        report_error("Variable should be defined to be on rhs", NOLINE);
    if (lhseval.is_variable_defined() == false)
        report_error("Variable should be defined to be on lhs", NOLINE);
    Eval_Result & result = *new Eval_Result_Value_Int();
    float rhsvalue;
    float lhsvalue;
    if(lhs->get_data_type()==int_data_type) {
        lhsvalue = (float)lhseval.get_value();
    }
    else if(lhs->get_data_type()==float_data_type) {
        lhsvalue = lhseval.get_floatvalue();
    }
    if(rhs->get_data_type()==int_data_type) {
        rhsvalue = (float)rhseval.get_value();
    }
    else if(rhs->get_data_type()==float_data_type)  {
        rhsvalue = rhseval.get_floatvalue();
    }
    result.set_value(boolval(lhsvalue,rhsvalue));
    return result;
}
/////////////////////////////////////////////////////////////////

Unconditional_Goto_Ast::Unconditional_Goto_Ast(int temp_bb) {
    bb = temp_bb;
}

Unconditional_Goto_Ast::~Unconditional_Goto_Ast() {}

void Unconditional_Goto_Ast::print_ast(ostream & file_buffer) {
    file_buffer<<AST_SPACE<<"Goto statement:\n"<<AST_NODE_SPACE<<"Successor: "<<bb<<"\n";
}

Eval_Result & Unconditional_Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result & result = *new Eval_Result_BB();
    result.set_value(bb);
    result.set_result_enum(goto_result);
    print_ast(file_buffer);
    file_buffer<<AST_NODE_SPACE<<"GOTO (BB " << bb<<")\n";
    return result;
}

/////////////////////////////////////////////////////////////////
Conditional_Goto_Ast::Conditional_Goto_Ast(Ast * temp_cond, int temp_bbif, int temp_bbelse) {
    cond = temp_cond;
    bbif = temp_bbif;
    bbelse  = temp_bbelse;
}

Conditional_Goto_Ast::~Conditional_Goto_Ast() {
    delete cond;
}

void Conditional_Goto_Ast::print_ast(ostream & file_buffer) {
    file_buffer<<endl<<AST_SPACE<<"If_Else statement:"<<AST_NODE_SPACE;
    cond->print_ast(file_buffer);
    file_buffer<<"\n"<<AST_NODE_SPACE<<"True Successor: "<<bbif<<"\n"<<AST_NODE_SPACE<<"False Successor: "<<bbelse<<"\n";
}

Eval_Result & Conditional_Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result & condeval = cond->evaluate(eval_env, file_buffer);
    int condvalue = condeval.get_value();
    Eval_Result & result = *new Eval_Result_BB();
    print_ast(file_buffer);
    if(condvalue==1) {
        //file_buffer << "yay";
        result.set_value(bbif);
        result.set_result_enum(goto_result);
        file_buffer <<AST_NODE_SPACE<< "Condition True : Goto (BB " << bbif << ")\n" ;
    }
    else if(condvalue==0) {
        // file_buffer << "yayas"; 
        result.set_value(bbelse);
        result.set_result_enum(goto_result);
        file_buffer << AST_NODE_SPACE<<"Condition False : Goto (BB " << bbelse << ")\n" ;        
    }
    return result;
}


/////////////////////////////////////////////////////////////////
Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry)
{
    variable_name = name;
    variable_symbol_entry = var_entry;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
    return variable_symbol_entry.get_data_type();
}

void Name_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << "Name : " << variable_name;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
    Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
    Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

    file_buffer << "\n" << AST_SPACE << variable_name << " : ";

    if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
        file_buffer << "undefined";

    else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
    {
        if (loc_var_val->get_result_enum() == int_result)
            file_buffer << loc_var_val->get_value() << "\n";
        else if(loc_var_val->get_result_enum() == float_result)
            file_buffer<<fixed<<setprecision(2) << loc_var_val->get_floatvalue() << "\n";
        else
            report_internal_error("Result type can only be int and float");
    }

    else
    {
        if (glob_var_val->get_result_enum() == int_result || glob_var_val->get_result_enum() == float_result)
        {
            if (glob_var_val == NULL)
                file_buffer << "0\n";
            else if (glob_var_val->get_result_enum() == int_result)
                file_buffer << glob_var_val->get_value() << "\n";
            else 
                file_buffer << glob_var_val->get_floatvalue() << "\n";
        }
        else {
            report_internal_error("Result type can only be int and float");
        }
    }
    file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
    if (eval_env.does_variable_exist(variable_name))
    {
        Eval_Result * result = eval_env.get_variable_value(variable_name);
        return *result;
    }

    Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
    return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
    Eval_Result_Value * i;
    if (result.get_result_enum() == int_result)
    {
        i = new Eval_Result_Value_Int();
        i->set_value(result.get_value());
    }
    else if(result.get_result_enum() == float_result) {
        i = new Eval_Result_Value_Float();
        i->set_floatvalue(result.get_floatvalue());
    }


    if (eval_env.does_variable_exist(variable_name))
        eval_env.put_variable_value(*i, variable_name);
    else
        interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

    template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type)
{
    constant = number;
    node_data_type = constant_data_type;
}

    template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

    template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
    return node_data_type;
}

    template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print_ast(ostream & file_buffer)
{
    if(node_data_type==int_data_type) {
        file_buffer << "Num : " << constant;
    }
    else if(node_data_type == float_data_type) {
        file_buffer << "Num : ";
        file_buffer<< fixed << setprecision(2) << constant;
    }
}

    template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    if (node_data_type == int_data_type)
    {
        Eval_Result & result = *new Eval_Result_Value_Int();
        result.set_value(constant);

        return result;
    }
    else if (node_data_type == float_data_type)
    {
        Eval_Result & result = *new Eval_Result_Value_Float();
        result.set_floatvalue(constant);

        return result;
    }
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * temp) {
    rhs = temp;    
}

Return_Ast::Return_Ast() {
    rhs=NULL;
}
Return_Ast::~Return_Ast() {
    delete rhs;
}

void Return_Ast::print_ast(ostream & file_buffer)
{
    file_buffer << AST_SPACE;
    if(rhs==NULL) file_buffer<< "Return <NOTHING>\n";
    else rhs->print_ast(file_buffer);
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    
    Eval_Result & result = *new Eval_Result_BB(); //define this eval result
    result = rhs->evaluate(eval_env,file_buffer);
    result.set_result_enum(goto_result);
    return result;
}

template class Number_Ast<int>;
template class Number_Ast<float>;

///////////////////////////////////////////////////////////////////////////////
Unary_Expression_Ast::Unary_Expression_Ast(Ast * temp_rhs)
{
    rhs = temp_rhs;
}

Data_Type Unary_Expression_Ast::get_data_type()
{
    return rhs->get_data_type();
}


void Unary_Expression_Ast::print_ast(ostream & file_buffer)
{
    file_buffer <<"\n"<< AST_NODE_SPACE << "Arith: UMINUS\n";

    file_buffer << AST_NODE_SPACE<< "LHS (";
    rhs->print_ast(file_buffer);
    file_buffer << ")";
}

Eval_Result & Unary_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    if(rhs->get_data_type()==int_data_type) {
        Eval_Result & result = *new Eval_Result_Value_Int();
        Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

        result.set_value(-rhsResult.get_value());
        return result;
    }
    else if(rhs->get_data_type()==float_data_type) {
        Eval_Result & result = *new Eval_Result_Value_Float();
        Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

        result.set_floatvalue(-rhsResult.get_floatvalue());
        return result;
    }
}


///////////////////////////////////////////////////////////////////////////////

Typecast_Ast::Typecast_Ast(Ast * temp_rhs,typecast_type top)
{
    rhs = temp_rhs;
    this->top = top;
    if(top==VOID)
        node_data_type= void_data_type;
    else if(top==INTEGER)
        node_data_type= int_data_type;
    else if(top==FLOAT)
        node_data_type= float_data_type;
    else if(top==DOUBLE)
        node_data_type= float_data_type; 
}

Data_Type Typecast_Ast::get_data_type()
{
    return node_data_type;
}


void Typecast_Ast::print_ast(ostream & file_buffer)
{
    rhs->print_ast(file_buffer);
}

Eval_Result & Typecast_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
    if(node_data_type==int_data_type) {
        Eval_Result & result = *new Eval_Result_Value_Int();
        Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

        if(rhs->get_data_type()==int_data_type) {
            result.set_value(rhsResult.get_value());
        }
        else if(rhs->get_data_type()==float_data_type) {
            result.set_value((int)rhsResult.get_floatvalue());
        }

        return result;
    }
    if(node_data_type==float_data_type) {
        Eval_Result & result = *new Eval_Result_Value_Float();
        Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
        if(rhs->get_data_type()==int_data_type) {
            result.set_floatvalue((float)rhsResult.get_value());
        }
        else if (rhs->get_data_type()==float_data_type) {
            result.set_floatvalue(rhsResult.get_floatvalue());
        }   
        return result;
    }
}
