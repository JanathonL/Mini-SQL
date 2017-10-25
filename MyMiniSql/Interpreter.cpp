#include "Interpreter.h"
#include"API.h"
using namespace std;
Interpreter::Interpreter()
{
}

Interpreter::~Interpreter()
{
}

int Interpreter::getquery()
{
	string query;
	cin >> query;
	if (query == "help") {
		return 9;
	}
	else if (query == "quit") {
		return 10;
	}
	else if (query == "execfile")
	{
		cin >> filename;
		return 8;
	}
	if (query == "create")
	{
		cin >> query;
		if (query == "table")
		{
			int res = create_table();
			if (res)
			{
				return 1;
			}
			else
			{
				cout << "Please Rewrite Query" << endl;
				return 0;
			}
		}
		else if (query == "index")
		{
			int res = create_index();
			if (res)
			{
				return 3;
			}
			else
				return 0;
		}
		else
		{
			cout << "Syntax Error" << endl;
		}
	}
	else if (query == "drop")
	{
		cin >> query;
		if (query == "table")
		{
			int res = drop_table();
			if (res)
			{
				return 2;
			}
			else
			{
				cout << "Syntax Error" << endl;
				return 0;
			}
		}
		else if (query == "index")
		{
			int res;
			res = drop_index();
			if (res)
			{
				return 4;
			}
			else
			{
				cout << "Syntax Error" << endl;
				return 0;
			}
		}
	}
	else if (query == "insert")
	{
		cin >> query;
		if (query == "into")
		{
			int res = insert_tuple();
			if (res)
			{
				return 5;
			}
			else
				return 0;
		}
		else
		{
			cout << "Syntax Error" << endl;
			return 0;
		}
	}
	else if (query == "delete")
	{
		cin >> query;
		if (query == "from")
		{
			int res = delete_tuple();
			if (res)
			{
				return 6;
			}
			else
				return 0;
		}
		else if (query == "*")
		{
			cin >> query;
			if (query == "from")
			{
				cin >> query;
				int begin = query.find(';');
				if (begin != string::npos)
					query = query.substr(0, begin);
				this->con.tablename = query;
				this->con.attr_name = "all";
				return 6;

			}
			else
			{
				cout << "Syntax Error" << endl;
				return 0;
			}
		}
		else
		{
			
			cout << "Syntax Error" << endl;
			return 0;
		}
	}
	else if (query == "select")
	{
		int res = select();
		if (res)
			return 7;
		else
			return 0;
	}
	else if (query == "quit")
	{
		return  10;
	}
	else 
	{

		char outstring[100];
		cin.getline(outstring, 100, '\n');
		cout << "Syntax Error" << endl;
		return 0;
	}
	
}

int Interpreter::create_table()
{
	string query;
	cin >> query;
	this->tab.table_name = query;
	cin >> query;
	tab.attr_count = 0;
	if (query != "(")
	{
		cout << "Syntax Error" << endl;
		return 0;
	}
	//getchar();
	string line, str;
	int attr_num = 0;
	while (getline(cin, line))
	{
		if (line == ");"||line==")")
			return 1;
		for (string::iterator i = line.begin(); i != line.end(); i++)
		{
			if (*i == '(' || *i == ')' || *i == ',')
			{
				*i = ' ';
			}
		}
		istringstream stream(line);
		//cout << "line:" << line << endl;
		string name, type, len, tag, primary_key;
		stream >> name;
		if (name == "") continue;
		stream >> type;
		////----------------------修改 开始----------------------
		//while (!stream.eof()) {
		//	stream >> name;
		//	stream >> type;
		//	if (type == "int") {
		//		tab.attrs[tab.attr_count].attr_type = 0;
		//		tab.attrs[tab.attr_count].attr_id = tab.attr_count;
		//		tab.attrs[tab.attr_count].attr_key_type = EMPTY;
		//		tab.attrs[tab.attr_count].attr_name = name;
		//		tab.attrs[tab.attr_count].attr_len = 1;
		//		tab.attr_count++;
		//		continue;
		//	}
		//	else if (type == "float") {
		//		tab.attrs[tab.attr_count].attr_type = -1;
		//		tab.attrs[tab.attr_count].attr_id = tab.attr_count;
		//		tab.attrs[tab.attr_count].attr_key_type = EMPTY;
		//		tab.attrs[tab.attr_count].attr_name = name;
		//		tab.attrs[tab.attr_count].attr_len = 1;
		//		tab.attr_count++;
		//		continue;
		//	}	
		//}
		//tab.attr_count--;
		//return 1;
		////----------------------修改 结束----------------------
		stream >> len;
		stream >> tag;
		int is_primarykey = 0;
		int tag_int = EMPTY;
		if (name == "primary")
		{
			if (type == "key")
			{
				primary_key = len;
				int i;
				for (i = 0; i < tab.attr_count; i++)
				{
					if (primary_key == tab.attrs[i].attr_name)
					{
						tab.attrs[i].attr_key_type = PRIMARY;
						break;
					}
				}
				if (i == tab.attr_count)
				{
					cout << "The attribute in the primary constraint is not found!" << endl;
					return 0;
				}
				else is_primarykey = 1;
				//else
				//	return 1;//这些地方不能返回，只有最前面能返回
			}
			else
			{
				cout << "Syntax Error!" << endl;
				return 0;
			}
			//return 1;//这些地方不能返回，只有最前面能返回
		}
		int intlen = atoi(len.c_str());
		if (type == "float")
		{
			tab.attrs[tab.attr_count].attr_type = FLOAT;//这里的float的值就是你的-1
			tab.attrs[tab.attr_count].attr_len = 1;//你之前没有弄属性长度
			if (len == "unique")
			{
				tag_int = UNIQUE;
			}
			else if (len == "primary")
			{
				tag_int = PRIMARY;
			}
			else if (!len.empty())
			{
				cout << "No Such Data Type!" << endl;
				return 0;
			}
			else if (len.empty())
			{
				tab.attrs[tab.attr_count].attr_key_type = EMPTY;
			}
		}
		else if (type == "int")
		{
			tab.attrs[tab.attr_count].attr_type = INT;//这里的int的值就是你的0
			tab.attrs[tab.attr_count].attr_len = 1;//你之前没有弄属性长度
			if (len == "unique")
			{
				tag_int = UNIQUE;
			}
			else if (len == "primary")
			{
				tag_int = PRIMARY;
			}
			else if (!len.empty())
			{
				cout << "No Such Data Type!" << endl;
				return 0;
			}
			else if (len.empty())
			{
				tab.attrs[tab.attr_count].attr_key_type = EMPTY;
			}
		}
		else if (type == "char")
		{
			tab.attrs[tab.attr_count].attr_len = atoi(len.c_str());
			tab.attrs[tab.attr_count].attr_type = CHAR;
			if (tag == "unique")
			{
				tag_int = UNIQUE;
			}
			else if (tag == "primary")
			{
				tag_int = PRIMARY;
			}
			else if (tag.empty())
			{
				tag_int = EMPTY;
			}
			else if (!tag.empty())
			{
				cout << "No Such Data Type!" << endl;
				return 0;
			}
		}
		else if(!is_primarykey){
			cout << "Data Type " << type << " doesn't exists" << endl;
			return 0;
		}
		if (name == "primary") continue;
		tab.attrs[tab.attr_count].attr_name = name;
		tab.attrs[tab.attr_count].attr_id = tab.attr_count;
		tab.attrs[tab.attr_count].attr_key_type = tag_int;
		tab.attr_count++;
		//return 1;
	}
}

int  Interpreter::drop_table()
{
	string name;
	cin >> name;
	for (string::iterator i = name.begin(); i != name.end(); i++)
	{
		if (*i == ';')
			*i = ' ';
	}
	int begin = 0;

	begin = name.find(" ", begin);  //查找空格在str中第一次出现的位置

	while (begin != -1)  //表示字符串中存在空格

	{

		name.replace(begin, 1, "");  // 用空串替换str中从begin开始的1个字符

		begin = name.find(" ", begin);  //查找空格在替换后的str中第一次出现的位置

	}
	tab.table_name = name;
	return 1;
}

int Interpreter::create_index()
{
	string indexname, on, tablename, attriname, line;
	getline(cin, line);
	for (string::iterator i = line.begin(); i != line.end(); i++)
	{
		if (*i == '(' || *i == ')' || *i == ';')
			*i = ' ';
	}
	istringstream l(line);
	l >> indexname;
	l >> on;
	l >> tablename;
	l >> attriname;
	if (on != "on")
	{
		cout << "Syntax Error" << endl;
		return 0;
	}
	this->ind.attr_name = attriname;
	this->ind.index_name = indexname;
	this->ind.table_name = tablename;
	return 1;
}

int Interpreter::drop_index()
{
	string indexname, on, tablename;
	getline(cin, indexname);
	for (string::iterator i = indexname.begin(); i != indexname.end(); i++)
	{
		if (*i == ';')
		{
			*i = ' ';
		}
	}
	istringstream l(indexname);
	l >> indexname;
	l >> on;
	l >> tablename;
	if (on != "on")
	{
		cout << "Sytanx Error!" << endl;
		return 0;
	}
	ind.index_name = indexname;
	ind.table_name = tablename;
	return 1;
}

int Interpreter::insert_tuple()
{
	string  tablename, values;
	string line;
	vector<string> v;
	getline(cin, line, ';');
	for (string::iterator i = line.begin(); i != line.end(); i++)
	{
		if (*i == ';' || *i == '(' || *i == ')' || *i == ',' || *i == '\'')
		{
			*i = ' ';
		}
	}
	istringstream l(line);
	l >> tablename;
	l >> values;
	string str;
	int num = 0;
	if (values != "values")
	{
		cout << "Syntax Error" << endl;
		return 0;
	}
	while (l >> str)
	{
		v.push_back(str);
		num++;
	}
	tup.table_name = tablename;
	Table table = Read_Table_Info(tup.table_name);
	for (int i = 0; i < table.attr_count; i++) {
		tup.attrs[i].attr_name = table.attrs[i].attr_name;
		tup.attrs[i].attr_key_type = table.attrs[i].attr_key_type;
		tup.attrs[i].attr_len = table.attrs[i].attr_len;
		tup.attrs[i].attr_type = table.attrs[i].attr_type;
		tup.attrs[i].attr_id = table.attrs[i].attr_id;
	}
	tup.attr_count = table.attr_count;
	int k = 0;
	for (vector<string>::iterator j = v.begin(); j != v.end(); j++)
	{
		tup.attr_values[k++] = *j;
	}

}

int Interpreter::delete_tuple()
{
	string tablename, _where, attri1, attri2, op1, condition1, connective, op2, condition2;
	string line;
	cin >> tablename;
	//getchar();
	getline(cin, line);
	for (string::iterator i = line.begin(); i != line.end(); i++)
	{
		if (*i == ';' || *i == '\'')
		{
			*i = ' ';
		}
	}
	istringstream l(line);
	l >> _where;
	l >> attri1;
	l >> op1;
	l >> condition1;
	l >> connective;
	//	l >> attri2;
	//	l >> op2;
	//	l >> condition2;
	if (connective.empty())
	{
		con.condition_num = 1;
		con.tablename = tablename;
		con.attr_name = attri1;
		con.op_type = op1;
		con.cmp_value = condition1;
	}
	/*	else
	{
	//		con.condition_num = 2;
	con.attr_name[0] = attri1;
	con.op_type[0] = op1;
	con.cmp_value[0] = condition1;
	con.connective = connective;
	con.attr_name[1] = attri2;
	con.op_type[1] = op2;
	con.cmp_value[1] = condition2;
	}*/
	return  1;
}

int Interpreter::select()
{
	string attri, _from, tablename, _where, compattri, op, condi;
	string line;
	getline(cin, line);
	for (string::iterator i = line.begin(); i != line.end(); i++)
	{
		if (*i == ';' || *i == '\'')
		{
			*i = ' ';
		}
	}
	istringstream l(line);
	l >> attri;
	l >> _from;
	l >> tablename;
	l >> _where;
	l >> compattri;
	l >> op;
	l >> condi;
	if (_from != "from")
	{
		cout << "Sytanx Error!" << endl;
		return 0;
	}
	con.attr_name = attri;
	con.tablename = tablename;
	con.cmp_attr_name = compattri;
	con.op_type = op;
	con.cmp_value = condi;
	return  1;
}


int Interpreter::execfile()
{
	return 8;
}

