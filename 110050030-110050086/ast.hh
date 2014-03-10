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

#ifndef AST_HH
#define AST_HH

#include <string>
#include <iomanip>
#define AST_SPACE "         "
#define AST_NODE_SPACE "            "

using namespace std;
enum relop {LT,GT,LE,GE,NE,EQ};
enum arithop {PLUS,MINUS,MULT,DIV};
enum typecast_type {VOID,INTEGER,FLOAT, DOUBLE};

class Ast;
class Procedure;

class Ast
{
    protected:
        Data_Type node_data_type;
    public:
        Ast();
        ~Ast();

        virtual Data_Type get_data_type();
        virtual bool check_ast(int line);

        virtual void print_ast(ostream & file_buffer) = 0;
        virtual void print_value(Local_Environment & eval_env, ostream & file_buffer);

        virtual Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
        virtual void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
        virtual Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer) = 0;
};

class Assignment_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;

    public:
    Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs);
    ~Assignment_Ast();

    Data_Type get_data_type();
    bool check_ast(int line);

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Unary_Expression_Ast:public Ast {
    Ast * rhs;
    public:
    Unary_Expression_Ast(Ast * temp_rhs);

    Data_Type get_data_type();
    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Typecast_Ast:public Ast {
    Ast * rhs;
    typecast_type top;
    public:
    Typecast_Ast(Ast * temp_rhs,typecast_type top);

    Data_Type get_data_type();

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Function_Call_Ast:public Ast
{
    list<Ast *> parameter_list;
    Procedure* p;
    bool parnull;
    public:

    Function_Call_Ast(list<Ast *> plist, Procedure* proc);
    ~Function_Call_Ast();
    void print_ast(ostream & file_buffer);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
}

class Arithmetic_Expression_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;
    arithop aop;

    public:

    Arithmetic_Expression_Ast(Ast * temp_lhs, Ast * temp_rhs, arithop op);
    Arithmetic_Expression_Ast(Ast * temp_rhs, arithop op);
    ~Arithmetic_Expression_Ast();
    int arithvalint(int lhsvalue, int rhsvalue); 
    float arithvalfloat(float lhsvalue, float rhsvalue);
    Data_Type get_data_type();
    void print_arithop(ostream & file_buffer);
    void print_ast(ostream & file_buffer);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Relation_Expression_Ast:public Ast
{
    Ast * lhs;
    Ast * rhs;
    relop rop;

    public:
    Relation_Expression_Ast(Ast * temp_lhs, Ast * temp_rhs, relop op);
    ~Relation_Expression_Ast();

    //bool check_ast(int line);
    bool boolval(float lhsvalue, float rhsvalue);
    void print_relop(ostream & file_buffer); 
    void print_ast(ostream & file_buffer);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
    Data_Type get_data_type();
};

class Conditional_Goto_Ast:public Ast {
    Ast* cond;
    int bbif, bbelse;

    public:
    Conditional_Goto_Ast(Ast * temo_cond, int temp_bbif, int temp_bbelse);
    ~Conditional_Goto_Ast();
    void print_ast(ostream & file_buffer);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Unconditional_Goto_Ast:public Ast {
    int bb;

    public:
    Unconditional_Goto_Ast(int temp_bb);
    ~Unconditional_Goto_Ast();
    void print_ast(ostream & file_buffer);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};


class Name_Ast:public Ast
{
    string variable_name;
    Symbol_Table_Entry variable_symbol_entry;

    public:
    Name_Ast(string & name, Symbol_Table_Entry & var_entry);
    ~Name_Ast();

    Data_Type get_data_type();

    void print_ast(ostream & file_buffer);

    void print_value(Local_Environment & eval_env, ostream & file_buffer);
    Eval_Result & get_value_of_evaluation(Local_Environment & eval_env);
    void set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result);
    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

template <class T>
class Number_Ast:public Ast
{
    T constant;

    public:
    Number_Ast(T number, Data_Type constant_data_type);
    ~Number_Ast();

    Data_Type get_data_type();

    void print_ast(ostream & file_buffer);

    Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

class Return_Ast:public Ast
{
    Ast* rhs;

    public:
        Return_Ast(Ast* temp);
        Return_Ast();
        ~Return_Ast();

        void print_ast(ostream & file_buffer);

        Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);
};

#endif
