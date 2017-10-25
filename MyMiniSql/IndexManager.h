#pragma once
#include "bplustree.h"
#include "minisql.h"
#include"CatalogManager.h"
#include "RecordManager.h"
#include "BufferManager.h"
class IndexManager {
public:
	bplus_tree* bplus[BLOCKNUM];//×î¶àBLOCKNUM¸öindex
	IndexManager() {
		for (int i = 0; i < BLOCKNUM; i++) {
			bplus[i]= bplus_tree_init(5,5,10,"","","");
		}
	}
	int Im_Create_index(Table table,string table_name, string attr_name, string index_name);
	int Im_Update_index(Table table, string table_name, string attr_name, string index_name);
	int Initial_bplus_tree_from_file(string indexfilename, bplus_tree* tree);
	bplus_tree* Find_empty_bplus();
	bplus_tree* Find_target_bplus(string indexname,string tablename);
	int Im_get_offset_from_key(bplus_tree* tree, int key);
	int Im_select(bplus_tree * tree, Table table, string attr_name, string cmp_attr_name, Op op, const char * condition);
};
