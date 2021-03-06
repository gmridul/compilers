
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

#ifndef LOCAL_ENVIRONMENT_HH
#define LOCAL_ENVIRONMENT_HH

#include<string>
#include<map>

#define VAR_SPACE "         "

using namespace std;

typedef enum
{
	int_result,
    float_result,
	void_result,
    goto_result,
    return_result_void,
    return_result_notvoid
} Result_Enum;

class Eval_Result;
class Local_Environment;

class Eval_Result 
{
protected:
	Result_Enum result_type;

public:
	int main_result_int;
	float main_result_float;
	virtual int get_value();
	virtual void set_value(int value);
	virtual void set_floatvalue(float number);
	virtual float get_floatvalue();

	virtual bool is_variable_defined();
	virtual void set_variable_status(bool def);

	virtual void set_result_enum(Result_Enum res) = 0;
	virtual Result_Enum get_result_enum() = 0;
};

class Eval_Result_BB:public Eval_Result
{
    int current_block;
public:
	int get_value();
	void set_value(int value);

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};


class Eval_Result_Value:public Eval_Result
{
public:
	virtual void set_value(int number) = 0;
	virtual void set_floatvalue(float number) = 0;
	virtual int get_value() = 0;
	virtual float get_floatvalue() = 0;

	virtual bool is_variable_defined() = 0;
	virtual void set_variable_status(bool def) = 0;

	virtual void set_result_enum(Result_Enum res) = 0;
	virtual Result_Enum get_result_enum() = 0;
};

class Eval_Result_Value_Void:public Eval_Result_Value
{
	bool defined;
public:
	Eval_Result_Value_Void();
	~Eval_Result_Value_Void();

	void set_value(int number){}
	int get_value(){return 1;}
    void set_floatvalue(float number){}
	float get_floatvalue(){return 1.1;}

	void set_variable_status(bool def);
	bool is_variable_defined();

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};

class Eval_Result_Value_Int:public Eval_Result_Value
{
	int value;
	bool defined;
public:
	Eval_Result_Value_Int();
	~Eval_Result_Value_Int();

	void set_value(int number);
	int get_value();
    void set_floatvalue(float number){}
	float get_floatvalue(){return 1.1;}

	void set_variable_status(bool def);
	bool is_variable_defined();

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};

class Eval_Result_Value_Float:public Eval_Result_Value
{
	float value;
	bool defined;
public:
	Eval_Result_Value_Float();
	~Eval_Result_Value_Float();

	void set_floatvalue(float number);
	float get_floatvalue();
	int get_value(){return 1;}
	void set_value(int value){}

	void set_variable_status(bool def);
	bool is_variable_defined();

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};

class Eval_Result_Return:public Eval_Result
{
    Eval_Result * e;
    bool int_defined,float_defined,void_defined; 
public:
    Eval_Result_Return();
    ~Eval_Result_Return();
    int get_value(){}
    void set_value(int value){}
    void set_floatvalue(float number){}
    float get_floatvalue(){}
    void set_Eval(Eval_Result * etemp, Result_Enum r);
	Eval_Result & get_Eval_Result();
    bool is_variable_defined();
    void set_variable_status(bool def);

    void set_result_enum(Result_Enum res);
    Result_Enum get_result_enum();
};


class Local_Environment
{
	public:
	map<string, Eval_Result *> variable_table;


	Local_Environment();
	~Local_Environment();

	void print(ostream & file_buffer);
	bool is_variable_defined(string name);
	Eval_Result * get_variable_value(string name);
	void put_variable_value(Eval_Result & value, string name);
	bool does_variable_exist(string name);
};

extern Local_Environment interpreter_global_table;

#endif
