#pragma once
#include"minisql.h"
#include<sstream>
class Interpreter
{
public:
	Table tab;
	Index ind;
	Tuple tup;
	Condition con;
	string filename;
public:
	Interpreter();
	~Interpreter();
	int  getquery();			//����ָ������ ��8�����ͣ����պ�������˳�򷵻�1-8
	int  create_table();       //1
	int  drop_table();         //2
	int  create_index();       //3
	int  drop_index();         //4
	int  insert_tuple();       //5
	int  delete_tuple();       //6
	int  select();             //7
	int  execfile();           //8
};
