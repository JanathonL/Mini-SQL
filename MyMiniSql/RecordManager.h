#ifndef _RecordManager_H_
#define _RecordManager_H_

#include "BufferManager.h"
#include "bplustree.h"
#include "MiniSQL.h"

//·µ»Øoffset
int Insert(Tuple &tuple);

char* Convertvalue(Tuple &tuple);


bool Select_record(Table table);
bool Judge_primarykey_values(Tuple &tuple);
bool Select_record(Table table, string attr_name);
bool Select_record(Table table, string cmp_attr_name, Op op, const char* condition);
bool Select_record(Table table, string attr_name,string cmp_attr_name, Op op,const char* condition);

vector<string> Select_values(Table table);
vector<string> Select_values(Table table,vector<int> &offset);
vector<string> Select_values(Table table, Op op, int key, bplus_tree * tree);
void Show_select(Table table);
void Show_select(Table table, string attr_name);
int Value_cmp(char* value, const char* condition);
void Show_result(char* value);
bool Delete_record(Table table, string attr_name, Op op,const char* condition);
bool Delete_all(Table table);

Op Change_str_to_op(string op);
std::string& trim(std::string &s);

void Trim_char(char * str);
int Change_char_to_int(const char* ch_value);
int Change_string_to_int(string str);


bool Delete_tuple(vector<int> offsetlist, Table table, Condition_list list);


bool Select_tuple(vector<int> offsetlist, Table table, Condition_list list);


#endif
