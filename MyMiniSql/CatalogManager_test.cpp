#include"CatalogManager.h"
#include"minisql.h"
int main() {
	Table table;
	table.table_name = "test_create_table";
	table.attr_count = 1;
	table.attrs[0].attr_name = "id";
	table.attrs[0].attr_key_type = EMPTY;
	table.attrs[0].attr_id = 1;
	table.attrs[0].attr_type = INT;
	table.attrs[0].attr_len = 1;
	Create_table(table);
	Table table1 = Read_Table_Info("test_create_table");
	cout << "table1:" << endl;
	Show_table(table1);
	cout << "table:" << endl;
	Show_table(table);
    //Drop_table(table.table_name);
	cout << "after drop table" << endl;
	Show_table(table);
	Index index;
	index.index_name = "id_index";
	index.table_name = "test_create_table";
	index.attr_name = "id";
	int success=Create_index(index);
	if (success) cout << "create index " << index.index_name << endl;
	else cout << "can't create index" << index.index_name << endl;
	////cout << "Drop index" << endl;
	////Drop_index(index);
	//int exist=Judge_attr_exist("test_create_table", "id");
	//if (exist) cout << "exist" << endl;
	//else cout << "can't find" << endl;
	//int judge_primary_unique = Judge_attr_primary_unique("test_create_table", "id");
	//if (judge_primary_unique) cout << "primary or unique" << endl;
	//else cout << "normal" << endl;

}