#include "CatalogManager.h"

bool Create_table(Table table)
{
	ofstream out;
	int i;
	int exist = Judge_table_exist(table.table_name);
	if (exist) {
		cout << "Table " << table.table_name << " exist!" << endl;
		return false;
	}
	else {
		string table_info = table.table_name + "_table.info";
		out.open(table_info.c_str(), ios::app);
		for (i = 0; i < table.attr_count; i++) {
			out << table.attrs[i].attr_name << " ";
			out << table.attrs[i].attr_key_type << " ";
			out << table.attrs[i].attr_type << " ";
			out << table.attrs[i].attr_len << " ";
			out << table.attrs[i].attr_id << " ";
		}
		out.close();
		string table_rec = table.table_name + "_table.rec";
		out.open(table_rec.c_str(), ios::app);
		out.close();
	}
	return true;
}

bool Create_index(Index index)
{
	int exist = Judge_index_exist(index);
	if (exist) {
		cout << "index " << index.index_name << " exists" << endl;
		return false;
	}
	ofstream out;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	out.open(index_rec, ios::app);
	out << index.index_name << " " << index.table_name << " " << index.attr_name << endl;
	out.close();
	out.open("Index.txt", ios::app);
	out << index.index_name << " " << index.table_name << " " << index.attr_name << endl;
	out.close();
	return true;
}

bool Create_index(Index index, bplus_tree * tree)
{
	int exist = Judge_index_exist(index);
	if (exist) {
		cout << "index " << index.index_name << " exists" << endl;
		return false;
	}
	ofstream out;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	out.open(index_rec, ios::app);
	//out << index.index_name << " " << index.table_name << " " << index.attr_name << endl;

	out.close();
	out.open("Index.txt", ios::app);
	out << index.index_name << " " << index.table_name << " " << index.attr_name << endl;
	out.close();
	return true;
}

bool Drop_table(string table_name)
{
	int exist = Judge_table_exist(table_name);
	if (!exist) return false;
	string table_info = table_name + "_table.info";
	string table_rec = table_name + "_table.rec";
	remove(table_info.c_str());
	remove(table_rec.c_str());
	return true;
}

bool Drop_index(Index& index)
{
	int exist = Judge_index_exist(index);
	if (!exist) return false;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	remove(index_rec.c_str());
	ifstream in;
	in.open("Index.txt", ios::in || ios::out);
	if (!in) {
		cout << "can't open Index.txt" << endl;
		return false;
	}
	string index_temp[100];
	string table_temp[100];
	string attr_temp[100];
	int k=0;
	string index_name;
	string table_name;
	string attr_name;
	int flag;
	if (!in.eof()) {
		flag = true;
		in >> index_name;
		in >> table_name;
		in >> attr_name;
		if (index_name==index.index_name&&table_name == index.table_name&&attr_name == index.attr_name) {
			flag = false;
		}
		if (flag) {
			index_temp[k] = index_name;
			table_temp[k] = table_name;
			attr_temp[k] = attr_name;
			k++;
		}
	}
	in.close();
	remove("Index.txt");
	ofstream out;
	out.open("Index.txt", ios::app);
	for (int i = 0; i < k; i++) {
		out << index_temp[i]<<" ";
		out << table_temp[i]<<" ";
		out << attr_temp[i] << endl;
	}
	out.close();
	return true;
}

bool Judge_attr_primary_unique(string table_name, string attr_name)
{
	int flag = false;
	int exist = Judge_table_exist(table_name);
	if (!exist) {
		cout << "Table " << table_name << " doesn't exist!" << endl;
		return false;
	}
	ifstream in;
	string table_info = table_name + "_table.info";
	in.open(table_info, ios::in);

	string attrname;
	string key_type;
	string temp;
	while (!in.eof()) {
		//	getline(in, attrname);
		in >> attrname;
		in >> key_type;
		if (attrname == attr_name) {
			if (key_type != "0"&&key_type!="283") {
				flag = true;
				break;
			}
		}
		else {
			for (int i = 0; i < 3; i++) {   //跳过后面跟着的三个属性
				in >> attrname;
				if (in.eof()) return false;
			}
		}
	}
	in.close();
	return flag;
}

bool Judge_table_exist(string table_name)
{
	ifstream in;
	in.open(table_name + "_table.info", ios::in);
	if (!in) {
		return false;
	}
	in.close();
	return true;
}



bool Judge_index_exist(Index& index)
{
	ifstream in;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	in.open(index_rec.c_str(), ios::in);
	//cout << index_rec << endl;
	if (!in) return false;
	in.close();
	return true;
}

bool Judge_attr_exist(string table_name, string attr_name)
{
	int flag = false;
	int exist = Judge_table_exist(table_name);
	if (!exist) {
		cout << "Table " << table_name << " doesn't exist!" << endl;
		return false;
	}
	ifstream in;
	string table_info = table_name + "_table.info";
	in.open(table_info, ios::in);

	string attrname;
	string temp;
	while (!in.eof()) {
	//	getline(in, attrname);
		in >> attrname;
		if (attrname == attr_name) {
			flag = true;
			break;
		}
		else {
			for (int i = 0; i < 4; i++) {   //跳过后面跟着的四个属性
				in >> attrname;
				if (in.eof()) return false;
			}
		}
	}
	in.close();
	return flag;
}

Table Read_Table_Info(string table_name)
{
	Table table;
	table.table_name = table_name;
	table.attr_count = 0;

	int exist = Judge_table_exist(table_name);
	if (!exist) {
		cout << "Table " << table.table_name << " doesn't exist!" << endl;
		return table;
	}
	ifstream in;
	string table_info = table_name + "_table.info";
	in.open(table_info, ios::in);
	int i = 0;
	while (!in.eof()) {
		in >> table.attrs[i].attr_name;
		in >> table.attrs[i].attr_key_type;
		in >> table.attrs[i].attr_type;
		in >> table.attrs[i].attr_len;
		in >> table.attrs[i].attr_id;
		i++;
	}
	table.attr_count = i-1;
	in.close();
	return table;
}

string Find_index_name(string table_name, string attr_name)
{
	ifstream in;
	in.open("Index.txt", ios::in);
	string index;
	string table;
	string attr;
	while(!in.eof()) {
		in >> index;
		in >> table;
		in >> attr;
		if (table == table_name&&attr == attr_name) {
			return index;
		}
	}
	return "";
}

void Show_table(Table & table)
{
	int exist = Judge_table_exist(table.table_name);
	if (!exist) {
		cout << "Table " << table.table_name << " doesn't exist!" << endl;
		return;
	}
	cout << "table_name:" << table.table_name << endl;
	cout << "count:" << table.attr_count << endl;
	for (int i = 0; i < table.attr_count; i++) {
		cout << "name:" << table.attrs[i].attr_name;
		cout << "\tkeytype:" << Attribute_key_type_to_str(table.attrs[i]);
		cout << "\ttype:" << Attribute_type_to_str(table.attrs[i]);
		cout << "\tlength:" << table.attrs[i].attr_len;
		cout << "\tid:" << table.attrs[i].attr_id;
		cout << endl;
	}
}

string Attribute_key_type_to_str(Attribute& attrs)
{
	string str = "";
	if (attrs.attr_key_type == PRIMARY || attrs.attr_type == 2)
		str = "primary key";
	else if (attrs.attr_key_type == UNIQUE || attrs.attr_type == 1)
		str = "unique key";
	else
		str = "normal key";
	return str;
}

string Attribute_type_to_str(Attribute & attrs)
{
	if (attrs.attr_type == CHAR || attrs.attr_type == 2) {
		return "char";
	}
	else if (attrs.attr_type == FLOAT || attrs.attr_type == 1) {
		return "float";
	}
	else if (attrs.attr_type == INT || attrs.attr_type == 0) {
		return "int";
	}
}

