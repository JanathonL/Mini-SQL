#pragma once
#ifndef _CatalogManager_h_
#define _CatalogManager_h_

#include "MiniSQL.h"
#include "bplustree.h"
using namespace std;


bool Create_table(Table table);

bool Create_index(Index index);

bool Create_index(Index index,bplus_tree* tree);

bool Drop_table(string table_name);

bool Drop_index(Index& index);

bool Judge_attr_primary_unique(string table_name, string attr_name);

/**********************************************/

bool Judge_table_exist(string table_name);

bool Judge_index_exist(Index& index);

bool Judge_attr_exist(string table_name, string attr_name);

/*******************************************/
Table Read_Table_Info(string table_name);

string Find_index_name(string table_name, string attr_name);

void Show_table(Table& table);

string Attribute_key_type_to_str(Attribute& attrs);

string Attribute_type_to_str(Attribute& attrs);


#endif

