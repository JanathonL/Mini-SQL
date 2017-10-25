#include "RecordManager.h"
#include "CatalogManager.h"

string delete_values[32];
vector<string> select_values;
vector<int> offset;
vector<int> delete_offset;
MBuffer mybuffer;
int delete_num;
int select_num;

int Insert(Tuple & tuple)
{
	//char* insert_values = new char[RecordSize];
	string filename = tuple.table_name + "_table.rec";
	Block* target = mybuffer.Find_next_empty_block(filename);
	//memset(insert_values, 0, sizeof(insert_values));
	target->record = Convertvalue(tuple);
	strcpy(target->tablename, tuple.table_name.c_str());
	strcpy(target->accessed, "1");
	strcpy(target->written, "1");
	//strcpy(target->record, insert_values);
	mybuffer.Write_block_to_file(filename, target);
	//delete[] insert_values;
	if (DEBUG)
		cout << "insert successfully" << endl;
	return target->offset;
}

char * Convertvalue(Tuple & tuple)
{
	char* v = new char[RecordSize];
	char* temp = new char[AttrbuteSize];
	char* ptr = v;
	memset(v, 0, sizeof(v));
	memset(temp, 0, sizeof(temp));
	int cnt = 0;
	for (int i = 0; i < tuple.attr_count; i++) {
		cnt++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, tuple.attr_values[i].c_str(), AttrbuteSize);
		for (int j = 0; j < AttrbuteSize; j++) {
			if (temp[j] >= '0'&&temp[j] <= '9' || temp[j] == '.' || temp[j] >= 'a'&&temp[j] <= 'z' || temp[j] >= 'A'&&temp[j] <= 'Z') {
				continue;
			}
			else temp[j] = ' ';
		}
		//strcat(v, temp);
		int j = 0;
		for (int k = (cnt - 1)*AttrbuteSize; k < cnt*AttrbuteSize - 1; k++) {
			v[k] = temp[j++];
		}
	}
	delete[] temp;
	return v;
}

bool Select_record(Table table)
{
	Show_select(table);
	select_values.clear();
	select_values = Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	char* v = new char[RecordSize];
	for (int i = 0; i < select_values.size(); i++) {
		strcpy(v, select_values[i].c_str());
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			v += AttrbuteSize;          //每次都要移动32位
			//Change_char_to_int(ch_value);
			Trim_char(ch_value);
			Show_result(ch_value);
		}
		cout << endl;
		v -= table.attr_count*AttrbuteSize;
	}
	return true;
}

bool Judge_primarykey_values(Tuple & tuple)
{
	Table table;
	table = Read_Table_Info(tuple.table_name);
	select_values.clear();
	select_values = Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	//int primary_id = table.getPrimaryKeyId();
	int is_find = 0;
	char* v = new char[RecordSize];
	for (int i = 0; i < select_values.size(); i++) {
		
		strcpy(v, select_values[i].c_str());
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			v += AttrbuteSize;          //每次都要移动32位
										//Change_char_to_int(ch_value);
			Trim_char(ch_value);
			trim(tuple.attr_values[k]);
			if (table.attrs[k].attr_key_type != EMPTY) {
				if (strcmp(ch_value, tuple.attr_values[k].c_str()) == 0) {
					cout << table.attrs[k].attr_name << " is primary key and the value exists" << endl;
					return 1;
				}
			}
			//Show_result(ch_value);
		}
		//cout << endl;
		v -= table.attr_count*AttrbuteSize;
	//	delete[] v;

	}
	delete[] ch_value;
	return 0;
}

bool Select_record(Table table, string attr_name)
{
	int id = table.searchAttrId(attr_name);
	if (id == -1) {
		cout << "table " << table.table_name << " doesn't have " << attr_name << endl;
		return false;
	}
	Show_select(table, attr_name);
	select_values.clear();
	select_values = Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	char* v = new char[RecordSize];
	for (int i = 0; i < select_values.size(); i++) {
		
		strcpy(v, select_values[i].c_str());
		is_find = 0;
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == id) {
				is_find = 1;
				//for (int j = 0; j < 32; j++) {
				//	if (ch_value[j] == ' ') {
				//		break;
				//	}
				//	else {
				//		cout << ch_value[j];
				//	}
				//}
				//cout << "\t";
				Show_result(ch_value);
			}

		}
		v -= table.attr_count*AttrbuteSize;
		if (is_find)
			cout << endl;
	}
	return true;
}
bool Select_record(Table table, string cmp_attr_name, Op op, const char* condition) {
	//int select_id = table.searchAttrId(attr_name);
	int cmp_id = table.searchAttrId(cmp_attr_name);
	if (cmp_id == -1) {
		cout << "table " << table.table_name << " doesn't have " << cmp_attr_name << endl;
		return false;
	}
	Show_select(table);
	select_values.clear();
	select_values = Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	char* select_value = new char[32];
	char* v = new char[RecordSize];
	char* s = new char[RecordSize];

	for (int i = 0; i < select_values.size(); i++) {
		strcpy(v, select_values[i].c_str());
		strcpy(s, select_values[i].c_str());
		is_find = 0;

		//vector<int> record_to_show;
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, s, 32);
			Trim_char(ch_value);
			s += AttrbuteSize;          //每次都要移动32位
			if (k == cmp_id) {
				if (op == EQ) {
					if (Value_cmp(ch_value, condition) == 0) {
						is_find = 1;
						//	record_to_show.push_back(k);
					}
				}
				else if (op == NE) {
					if (Value_cmp(ch_value, condition) != 0) {
						is_find = 1;
						/*cout << select_value << "\t";*/
						//Show_result(select_value);
					//	record_to_show.push_back(k);

					}
				}
				else if (op == LT) {
					if (Value_cmp(ch_value, condition) < 0) {
						is_find = 1;
						//cout << select_value << "\t";
						//Show_result(select_value);
					//	record_to_show.push_back(k);

					}
				}
				else if (op == GT) {
					if (Value_cmp(ch_value, condition) > 0) {
						is_find = 1;
						//cout << select_value << "\t";
						//Show_result(select_value);
					//	record_to_show.push_back(k);

					}
				}
				else if (op == LE) {
					if (Value_cmp(ch_value, condition) <= 0) {
						is_find = 1;
						//cout << select_value << "\t";
						//Show_result(select_value);
					//	record_to_show.push_back(k);

					}
				}
				else if (op == GE) {
					if (Value_cmp(ch_value, condition) >= 0) {
						is_find = 1;
						//cout << select_value << "\t";
						//Show_result(select_value);
						//record_to_show.push_back(k);
					}
				}
			}
		}
		s -= table.attr_count*AttrbuteSize;

		if (is_find == 0) continue;
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			Trim_char(ch_value);
			v += AttrbuteSize;
			Show_result(ch_value);
		}
		if (is_find)
			cout << endl;
		v -= table.attr_count*AttrbuteSize;

	}
	return true;
}
bool Select_record(Table table, string attr_name, string cmp_attr_name, Op op, const char* condition)
{
	int select_id = table.searchAttrId(attr_name);
	int cmp_id = table.searchAttrId(cmp_attr_name);
	if (select_id == -1) {
		cout << "table " << table.table_name << " doesn't have " << attr_name << endl;
		return false;
	}
	if (cmp_id == -1) {
		cout << "table " << table.table_name << " doesn't have " << cmp_attr_name << endl;
		return false;
	}
	Show_select(table, attr_name);
	select_values.clear();
	select_values = Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	char* select_value = new char[32];
	for (int i = 0; i < select_values.size(); i++) {
		char* v = new char[RecordSize];
		strcpy(v, select_values[i].c_str());
		char* s = new char[RecordSize];
		strcpy(s, select_values[i].c_str());
		is_find = 0;
		for (int k = 0; k < table.attr_count; k++) {
			if (is_find) break;
			memcpy(ch_value, s, 32);
			Trim_char(ch_value);
			s += AttrbuteSize;
			if (k == select_id) {
				strcpy(select_value, ch_value);
				Trim_char(select_value);
				is_find = 1;
			}
		}
		is_find = 0;
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			Trim_char(ch_value);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == select_id) {
				strcpy(select_value, ch_value);
			}
			if (k == cmp_id) {
				if (op == EQ) {
					if (Value_cmp(ch_value, condition) == 0) {
						is_find = 1;
						Show_result(select_value);
					}
				}
				else if (op == NE) {
					if (Value_cmp(ch_value, condition) != 0) {
						is_find = 1;
						/*cout << select_value << "\t";*/
						Show_result(select_value);
					}
				}
				else if (op == LT) {
					if (Value_cmp(ch_value, condition) < 0) {
						is_find = 1;
						//cout << select_value << "\t";
						Show_result(select_value);
					}
				}
				else if (op == GT) {
					if (Value_cmp(ch_value, condition) > 0) {
						is_find = 1;
						//cout << select_value << "\t";
						Show_result(select_value);
					}
				}
				else if (op == LE) {
					if (Value_cmp(ch_value, condition) <= 0) {
						is_find = 1;
						//cout << select_value << "\t";
						Show_result(select_value);
					}
				}
				else if (op == GE) {
					if (Value_cmp(ch_value, condition) >= 0) {
						is_find = 1;
						//cout << select_value << "\t";
						Show_result(select_value);
					}
				}
			}
		}
		if (is_find)
			cout << endl;
	}
	return true;
}

vector<string>  Select_values(Table table)
{
	string filename = table.table_name + "_table.rec";
	select_values.clear();
	//offset.clear();
	char* values = new char[RecordSize];
	//char values[RecordSize];
	mybuffer.Read_block_from_file(filename);
	for (int i = 0; i < BLOCKNUM; i++) {
		if (strcmp(mybuffer.Buffer[i]->written, "1") == 0) {//说明写过有数据，那就拿出来	
			memset(values, 0, sizeof(values));
			int temp = i;
			strcpy(values, (mybuffer.Buffer[temp]->record));
			select_values.push_back(values);
			i = temp;
			offset.push_back(i);
		}
	}
	if (select_values.empty()) {
		return select_values;
	}
	return select_values;
}

vector<string> Select_values(Table table, vector<int>& offset)
{
	string filename = table.table_name + "_table.rec";
	vector<string> select_values;
	select_values.clear();
	offset.clear();
	char* values = new char[RecordSize];
	memset(values, 0, sizeof(values));
	mybuffer.Read_block_from_file(filename);
	for (int i = 0; i < BLOCKNUM; i++) {
		if (strcmp(mybuffer.Buffer[i]->written, "1") == 0) {//说明写过有数据，那就拿出来
			memset(values, 0, sizeof(values));
			strcpy(values, (mybuffer.Buffer[i]->record));
			offset.push_back(i);
			select_values.push_back(values);
		}
	}
	delete[] values;
	if (select_values.empty()) {
		return select_values;
	}
	return select_values;
}

vector<string> Select_values(Table table, Op op, int key, bplus_tree * tree)
{
	string filename = table.table_name + "_table.rec";
	vector<string> select_values;
	select_values.clear();
	offset.clear();
	char* values = new char[RecordSize];
	memset(values, 0, sizeof(values));
	mybuffer.Read_block_from_file(filename);


	if (op == EQ) {
		int off = bplus_tree_get(tree, key);
		if (off == -1) return select_values;
		strcpy(values, (mybuffer.Buffer[off]->record));
		offset.push_back(off);
		select_values.push_back(values);
	}
	else {
		for (int i = 0; i < BLOCKNUM; i++) {
			if (strcmp(mybuffer.Buffer[i]->written, "1") == 0) {//说明写过有数据，那就拿出来
				strcpy(values, (mybuffer.Buffer[i]->record));
				offset.push_back(i);
				select_values.push_back(values);
			}
		}
		if (select_values.empty()) {
			return select_values;
		}
	}
	delete[] values;
	return select_values;
}

void Show_select(Table table)
{
	string filename = table.table_name + "_table.rec";
	int exist = Judge_table_exist(table.table_name);
	if (!exist) {
		cout << "Table " << table.table_name << " doesn't exist!" << endl;
		return;
	}
	for (int i = 0; i < table.attr_count; i++) {
		cout << table.attrs[i].attr_name << "\t";
	}
	cout << endl;
	for (int i = 0; i < table.attr_count; i++) {
		cout << "-------";
	}
	cout << endl;
}

void Show_select(Table table, string attr_name)
{
	string filename = table.table_name + "_table.rec";
	int exist = Judge_table_exist(table.table_name);
	if (!exist) {
		cout << "Table " << table.table_name << " doesn't exist!" << endl;
		return;
	}
	for (int i = 0; i < table.attr_count; i++) {
		if (table.attrs[i].attr_name == attr_name)
			cout << table.attrs[i].attr_name << "\t";
	}
	cout << endl;
	cout << "-------";
	cout << endl;
}

int Value_cmp(char * value, const char * condition)
{
	if (value[0] >= 'a'&&value[0] <= 'z' || value[0] >= 'A'&&value[0] <= 'Z') {
		return strcmp(value, condition);
	}
	else if (value[0] >= '0'&&value[0] <= '9') {
		int value_is_float = 0;
		int condition_is_float = 0;
		int value_float_pos = 0;
		int condition_float_pos = 0;
		int cnt_value, cnt_condition;
		cnt_value = 0;
		cnt_condition = 0;
		for (int i = 0; value[i] != 0; i++) {
			if (value[i] == '.') {
				value_is_float = 1;
				value_float_pos = i;
			}
			cnt_value++;
		}
		for (int i = 0; condition[i] != 0; i++) {
			if (condition[i] == '.') {
				condition_is_float = 1;
				condition_float_pos = i;
			}
			cnt_condition++;
		}
		if (condition_is_float) cnt_condition = condition_float_pos;
		if (value_is_float) cnt_value = value_float_pos;
		if ((cnt_condition) > (cnt_value)) return -1;
		else if ((cnt_condition) < (cnt_value)) return 1;
		else {
			return strcmp(value, condition);
		}
	}
	else {
		return strcmp(value, condition);
	}
}

void Show_result(char * value)
{
	for (int j = 0; j < 32; j++) {
		if (value[j] == ' ' || value[j] == 0) {
			break;
		}
		else {
			cout << value[j];
		}
	}
	cout << "\t";
}

bool Delete_record(Table table, string attr_name, Op op, const char * condition)
{
	int id = table.searchAttrId(attr_name);
	if (id == -1) {
		cout << "table " << table.table_name << " doesn't have " << attr_name << endl;
		return false;
	}
	select_values.clear();
	delete_offset.clear();
	offset.clear();
	Select_values(table);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	for (int i = 0; i < select_values.size(); i++) {
		char* v = new char[RecordSize];
		strcpy(v, select_values[i].c_str());
		is_find = 0;
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			Trim_char(ch_value);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == id) {
				if (op == EQ) {
					if (Value_cmp(ch_value, condition) == 0) {
						delete_offset.push_back(offset[i]);
					}
				}
				else if (op == NE) {
					if (Value_cmp(ch_value, condition) != 0) {
						delete_offset.push_back(offset[i]);
					}
				}
				else if (op == LT) {
					if (Value_cmp(ch_value, condition) < 0) {
						delete_offset.push_back(offset[i]);
					}
				}
				else if (op == GT) {
					if (Value_cmp(ch_value, condition) > 0) {
						delete_offset.push_back(offset[i]);
					}
				}
				else if (op == LE) {
					if (Value_cmp(ch_value, condition) <= 0) {
						delete_offset.push_back(offset[i]);
					}
				}
				else if (op == GE) {
					if (Value_cmp(ch_value, condition) >= 0) {
						delete_offset.push_back(offset[i]);
					}
				}
			}
		}
	}
	string filename = table.table_name + "_table.rec";
	for (int i = 0; i < delete_offset.size(); i++) {
		int ok = mybuffer.Delete_buffer(filename, delete_offset[i]);
		if (!ok) return false;
	}
	cout << "delete succeessfully!" << endl;
	cout << delete_offset.size() << " rows affected" << endl;
	return true;
}

bool Delete_all(Table table)
{
	select_values.clear();
	delete_offset.clear();
	offset.clear();
	Select_values(table);
	string filename = table.table_name + "_table.rec";
	mybuffer.Init();
	mybuffer.Write_all_blocks_to_file(filename);
	cout << "delete succeessfully!" << endl;
	cout << select_values.size() << " rows affected" << endl;
	return false;
}
Op Change_str_to_op(string op)
{
	if (op == "<") {
		return LT;
	}
	else if (op == "=") {
		return EQ;
	}
	else if (op == "<>") {
		return NE;
	}
	else if (op == ">") {
		return GT;
	}
	else if (op == "<=") {
		return LE;
	}
	else if (op == ">=") {
		return GE;
	}
	else return Wrong;
}

std::string & trim(std::string & s)
{
	
		if (s.empty())
		{
			return s;
		}

		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		return s;
	
}

void Trim_char(char * str)
{
	char temp[20];
	memset(temp, 0, sizeof(temp));
	int i = 0;
	int cnt = 0;
	while (1) {
		if (i == 0 && str[i] == ' ') {
			i++;
			continue;
		}
		if (str[i] == ' ' || str[i] == 0) {
			break;
		}
		else {
			temp[cnt++] = str[i];
		}
		i++;
	}
	memset(str, 0, sizeof(str));
	strcpy(str, temp);
}

int Change_char_to_int(const char * ch_value)
{
	int sum = 0;
	for (int j = 0; j < 32; j++) {
		if (ch_value[j] == ' ' || ch_value[j] == 0) {
			break;
			//continue;
		}
		else {
			sum = sum * 10 + ch_value[j] - '0';
		}
	}
	return sum;
}

int Change_string_to_int(string str)
{
	int sum = 0;
	for (int i = 0; i < str.size(); i++) {
		sum = sum * 10 + str[i] - '0';
	}
	return 0;
}
