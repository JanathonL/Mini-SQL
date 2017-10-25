#include "BufferManager.h"
#include "minisql.h"
#include "RecordManager.h"
#include "CatalogManager.h"
int main() {
	Table table;
	table.table_name = "test_record";
	table.attr_count = 1;
	table.attrs[0].attr_name = "id";
	table.attrs[0].attr_key_type = EMPTY;
	table.attrs[0].attr_id = 1;
	table.attrs[0].attr_type = INT;
	table.attrs[0].attr_len = 1;
	cout << "------create_table-------" << endl;
	Create_table(table);
	string filename = table.table_name + "_table.rec";
	MBuffer bm;
	//cout << "-----------bm.Init()----------" << endl;
	//bm.Init();
	//cout << "------------Write_all_blocks_to_file--------" << endl;
	//bm.Write_all_blocks_to_file(filename);

	Tuple tuple;
	tuple.table_name = "test_record";
	tuple.attr_count = 1;
	tuple.attrs[0].attr_name = "id";
	tuple.attrs[0].attr_key_type = EMPTY;
	tuple.attrs[0].attr_len = 1;
	tuple.attrs[0].attr_type = INT;
	tuple.attrs[0].attr_id = 1;
	tuple.attr_values[0] = "1231234";
	Insert(tuple);
	//bm.Read_block_from_file(filename);
	//bm.Show_all_blocks(bm.Buffer);
//	cout << "before delete_record" << endl;
	Select_record(table);
	//	Delete_record(table, "id", LT, "12345");
		//Select_record(table,"id",LT,"12345");
	//	cout << "after delete_record" << endl;
	//	Select_record(table);
		//cout << "delete_all" << endl;
		//Delete_all(table);
		//cout << "after delete_all" << endl;
		//Select_record(table);


}