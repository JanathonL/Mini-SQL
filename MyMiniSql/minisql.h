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


//	用于描述表中一个属性在表中的具体信息
class Attribute
{
public:
	string attr_name;
	int attr_type;	//属性的数据类型，分别为CHAR, FLOAT, INT
	int attr_key_type;//属性的主键类型，分别为PRIMARY, UNIQUE, NULL(EMPTY)
	int attr_len; 	//属性所存放的数据的长度，如果不是CHAR，则长度标记为1
	int attr_id;    //属性在表中是第几个
};

//	用于描述表的信息
struct Table
{
	string table_name;  //表名
	int attr_count;				//表中属性的总个数
	Attribute attrs[32];	//表的所有属性列表, 最多32个属性
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

//	用于描述判断条件的信息

struct Condition
{
	string tablename;
	string attr_name;	//条件所对应的属性名
	string op_type;		//条件所用到的比较模式，分别为"<>", "=", ">=", "<=", "<", ">"
	string cmp_value;	//条件所需要进行比较的值
	string connective;
	//--------------------修改 开始-------------------------------------
	string cmp_attr_name;   //用于比较的属性名
	//--------------------修改 结束-------------------------------------
	int condition_num;
};
typedef list<Condition> Condition_list;

//	用于描述索引信息
struct Index
{
	string index_name;
	string table_name;
	string attr_name;	//	索引所对应的属性
};

//insert into card values("", "", 1234.4);
struct Tuple : public Table
{
	string attr_values[32];
};




