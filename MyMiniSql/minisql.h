#pragma once

#include <algorithm>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdarg>


using namespace std;
#define DEBUG false
#define BLOCKNUM 1000
enum DBtype
{
	AND = 258,
	ON = 259,
	SELECT = 260,
	FROM = 261,
	WHERE = 262,
	DROP = 263,
	TABLE = 264,
	CREATE = 265,
	INDEX = 266,
	PRIMARY = 267,
	KEY = 268,
	VALUES = 269,
	UNIQUE = 270,
	INSERT = 271,
	INTO = 272,
	DELETE = 273,
	QUIT = 274,
	EXECFILE = 275,
	INT = 0,
	FLOAT = -1,
	CHAR = 278,
	NAME = 279,
	STRING = 280,
	INTNUM = 281,
	FLOATNUM = 282,
	EMPTY = 283,
	COMPARISON = 284,
	STMT = 285,
	ATTR_INFO = 286,
	ATTR_LIST = 287,
	ATTR = 288,
	DATA_TYPE = 289,
	ATTR_VALUE_LIST = 290,
	WH_LIST = 291,
	WH_NAME = 292
};
enum Op {//EQ =,LT <,GT >,LE <=,GE  >=,NE !=
	EQ = 1,
	LT = 2,
	GT = 3,
	LE = 4,
	GE = 5,
	NE = 6,
	Wrong = -1
};

//allow user input 500 characters once in a line
#define MAX_ONE_LINE 256

//prompt
#define WELCOME     "Welcome to the minisql system. Hope you have a good day."
#define PROMPT      "\nminisql> "
#define ENTER_SIGN  "      -> "
#define BYE_BYE     "Good Bye. See you next time."


//	������������һ�������ڱ��еľ�����Ϣ
class Attribute
{
public:
	string attr_name;
	int attr_type;	//���Ե��������ͣ��ֱ�ΪCHAR, FLOAT, INT
	int attr_key_type;//���Ե��������ͣ��ֱ�ΪPRIMARY, UNIQUE, NULL(EMPTY)
	int attr_len; 	//��������ŵ����ݵĳ��ȣ��������CHAR���򳤶ȱ��Ϊ1
	int attr_id;    //�����ڱ����ǵڼ���
};

//	�������������Ϣ
struct Table
{
	string table_name;  //����
	int attr_count;				//�������Ե��ܸ���
	Attribute attrs[32];	//������������б�, ���32������
							//return primary key id
	int getPrimaryKeyId() {
		for (int i = 0; i < attr_count; ++i)
		{
			if (attrs[i].attr_key_type == DBtype::PRIMARY)
			{
				return i;
			}
		}
		//if no primary key
		return -1;
	}

	int searchAttrId(string att_name) {
		for (int i = 0; i < attr_count; ++i)
		{
			if (attrs[i].attr_name == att_name)
			{
				return i;
			}
		}
		//if no primary key
		return -1;
	}

	int length()
	{
		int len = 0;
		for (int i = 0; i < attr_count; ++i)
		{
			len += attrs[i].attr_len;
		}
		return len;
	}
};

//	���������ж���������Ϣ

struct Condition
{
	string tablename;
	string attr_name;	//��������Ӧ��������
	string op_type;		//�������õ��ıȽ�ģʽ���ֱ�Ϊ"<>", "=", ">=", "<=", "<", ">"
	string cmp_value;	//��������Ҫ���бȽϵ�ֵ
	string connective;
	//--------------------�޸� ��ʼ-------------------------------------
	string cmp_attr_name;   //���ڱȽϵ�������
	//--------------------�޸� ����-------------------------------------
	int condition_num;
};
typedef list<Condition> Condition_list;

//	��������������Ϣ
struct Index
{
	string index_name;
	string table_name;
	string attr_name;	//	��������Ӧ������
};

//insert into card values("", "", 1234.4);
struct Tuple : public Table
{
	string attr_values[32];
};




