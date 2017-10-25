#include "IndexManager.h"
#include "BufferManager.h"
#include "CatalogManager.h"

int IndexManager::Im_Create_index(Table table,string table_name,string attr_name, string index_name)
{
	int exist=Judge_table_exist(table_name);
	if (!exist) {
		cout << "Table " << table_name << " doesn't exist " << endl;
		return 0;
	}
	exist = Judge_attr_exist(table_name, attr_name);
	if (!exist) {
		cout << "table " << table_name << " doesn't have " << attr_name << endl;
		return 0;
	}
	Index index;
	index.attr_name = attr_name;
	index.index_name = index_name;
	index.table_name = table_name;
	exist = Judge_index_exist(index);
	if (exist) {
		cout << index_name << " exists" << endl;
	}
	bplus_tree * target = Find_empty_bplus();
	target->attr_name = attr_name;
	target->index_name = index_name;
	target->table_name = table_name;
	vector<int> offset;
	vector<int> key, data;
	key.clear();
	data.clear();
	vector<string> select_values = Select_values(table,offset);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	int id = table.searchAttrId(attr_name);
	for (int i = 0; i < select_values.size(); i++) {
		char* v = new char[RecordSize];
		strcpy(v, select_values[i].c_str());
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == id) {
				int values = Change_char_to_int(ch_value);
				bplus_tree_put(target, values, offset[i]);
				key.push_back(values);
				data.push_back(offset[i]);
			}
		//	Show_result(ch_value);
		}
	}
	//bplus_tree_dump(target);
	
	//Get_tree_key_and_offset(target, key, data);
	//cout << "key----data----" << endl;
	//for (int i = 0; i < key.size(); i++) {
	//	cout << key[i] << "\t" << data[i] << endl;
	//}
	MBuffer mb;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	Create_index(index,target);
	mb.Write_index_to_file(index_rec, index, key, offset);
	return 0;
}

int IndexManager::Im_Update_index(Table table, string table_name, string attr_name, string index_name)
{
	string indexfilename = index_name + table_name + "_index.rec";
	remove(indexfilename.c_str());
	int exist = Judge_table_exist(table_name);
	if (!exist) {
		cout << "Table " << table_name << " doesn't exist " << endl;
		return 0;
	}
	exist = Judge_attr_exist(table_name, attr_name);
	if (!exist) {
		cout << "table " << table_name << " doesn't have " << attr_name << endl;
		return 0;
	}
	Index index;
	index.attr_name = attr_name;
	index.index_name = index_name;
	index.table_name = table_name;
	exist = Judge_index_exist(index);
	if (exist) {
		cout << index_name << " exists" << endl;
	}
	bplus_tree * target = Find_target_bplus(index_name,table_name);
	target->attr_name = attr_name;
	target->index_name = index_name;
	target->table_name = table_name;
	target->root = new bplus_node;
	for (int i = 0; i < MAX_LEVEL; i++) {
		target->head[i] = new bplus_node;
	}
	vector<int> offset;
	vector<int> key, data;
	key.clear();
	data.clear();
	vector<string> select_values = Select_values(table, offset);
	char* ch_value = new char[32];//临时char变量
	memset(ch_value, 0, sizeof(ch_value));
	int is_find = 0;
	int id = table.searchAttrId(attr_name);
	for (int i = 0; i < select_values.size(); i++) {
		char* v = new char[RecordSize];
		strcpy(v, select_values[i].c_str());
		for (int k = 0; k < table.attr_count; k++) {
			memcpy(ch_value, v, 32);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == id) {
				int values = Change_char_to_int(ch_value);
				bplus_tree_put(target, values, offset[i]);
				key.push_back(values);
				data.push_back(offset[i]);
			}
			Show_result(ch_value);
		}
	}
	bplus_tree_dump(target);

	//Get_tree_key_and_offset(target, key, data);
	//cout << "key----data----" << endl;
	//for (int i = 0; i < key.size(); i++) {
	//	cout << key[i] << "\t" << data[i] << endl;
	//}
	MBuffer mb;
	string index_rec = index.index_name + index.table_name + "_index.rec";
	Create_index(index, target);
	mb.Write_index_to_file(index_rec, index, key, offset);
	return 0;
}

int IndexManager::Initial_bplus_tree_from_file(string indexfilename, bplus_tree * tree)
{
	MBuffer mb;
	vector<int> key, offset;
	mb.Read_index_from_file(indexfilename, tree, key, offset);
	for (int i = 0; i < key.size(); i++) {
		bplus_tree_put(tree, key[i], offset[i]);
	//	cout << key[i] << " " << offset[i] << endl;
	}
	//bplus_tree_dump(tree);
	return 1;
}

bplus_tree * IndexManager::Find_empty_bplus()
{
	for (int i = 0; i < BLOCKNUM; i++) {
		if (bplus[i]->table_name == "" || bplus[i]->index_name == "") {
			return bplus[i];
		}
	}
	return nullptr;
}

bplus_tree * IndexManager::Find_target_bplus(string indexname, string tablename)
{
	for (int i = 0; i < BLOCKNUM; i++) {
		if (bplus[i]->table_name == tablename || bplus[i]->index_name == indexname) {
			return bplus[i];
		}
	}
	return nullptr;
}

int IndexManager::Im_get_offset_from_key(bplus_tree * tree, int key)
{
	return bplus_tree_get(tree, key);
}

int IndexManager::Im_select(bplus_tree * tree, Table table, string attr_name, string cmp_attr_name, Op op, const char * condition)
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
	vector<string> select_values;
	select_values.clear();
	int key_values = Change_char_to_int(condition);
	select_values = Select_values(table,op, key_values,tree);
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
			if (is_find) break;
			memcpy(ch_value, v, 32);
			Trim_char(ch_value);
			v += AttrbuteSize;          //每次都要移动32位
			if (k == select_id) {
				strcpy(select_value, ch_value);
				Trim_char(select_value);
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
