#include"API.h"
int main(void)
{
	Interpreter a;
	MBuffer bm;
	IndexManager im;
	//IndexInfo ii;
	bplus_tree* target;
	string indexname;
	string indexfilename;
	Table table;
	Index index;
	cout << "Welcome to MiniSQL " << endl;
	cout << "created by JanathonL and AWX" << endl;
	cout << "Type 'help' for help. Type 'quit' to quit." << endl;
	cout << endl;
	while (1)
	{
		cout << endl;
		cout << "MiniSQL->";
		int exist = 0;
		int id = 0, ty = 0;
		int offset;
		string filename;
		Op op;
		switch (a.getquery())
		{
		case 1:
			exist = Judge_table_exist(a.tab.table_name);
			if (exist) {
				cout << a.tab.table_name << " exists" << endl;
				continue;
			}
			Create_table(a.tab);
			//cout << "-----------bm.Init()----------" << endl;
			bm.Init();
			//cout << "------------Write_all_blocks_to_file--------" << endl;
			filename = a.tab.table_name + "_table.rec";
			bm.Write_all_blocks_to_file(filename);
			cout << "create " << a.tab.table_name << " successfully!" << endl;
			break;
		case 2:
			exist = Judge_table_exist(a.tab.table_name);
			if (!exist) {
				cout << a.tab.table_name << " doesn't exist" << endl;
				continue;
			}
			Drop_table(a.tab.table_name);
			cout << "drop " << a.tab.table_name << " successfully!" << endl;
			break;
		case 3:
			//a.create_index();
			exist = Judge_index_exist(a.ind);
			if (exist) {
				cout << a.ind.index_name << " exists" << endl;
				continue;
			}
			table = Read_Table_Info(a.ind.table_name);
			id = table.searchAttrId(a.ind.attr_name);
			if (id == -1) {
				cout << a.ind.attr_name << " doesn't exist" << endl;
				continue;
			}
			ty = table.attrs[id].attr_type;
			//ii.indexName = a.ind.index_name;
			//ii.tableName = a.tab.table_name;
			//ii.attribute = a.ind.attr_name;
			//ii.type = ty;
			//table = Read_Table_Info(a.ind.table_name);
			im.Im_Create_index(table, table.table_name, a.ind.attr_name, a.ind.index_name);
			cout << "create " << a.ind.index_name << " successfully!" << endl;
			break;
		case 4:
			//a.drop_index();
			//a.ind.table_name = a.tab.table_name;
			exist = Judge_index_exist(a.ind);//ind里加table_name
			if (!exist) {
				cout << a.ind.index_name << " doesn't exist" << endl;
				continue;
			}
			table = Read_Table_Info(a.ind.table_name);
			id = table.searchAttrId(a.ind.attr_name);
			ty = table.attrs[id].attr_type;
			if (ty > 0) {
				ty = table.attrs[id].attr_type;
			}
			//im.DropIndex(a.ind.index_name, ty);
			Drop_index(a.ind);
			cout << "drop " << a.ind.index_name << " successfully!" << endl;
			break;
		case 5:
			//	a.insert_tuple();

				//exist = Judge_table_exist(a.tup.table_name);
				//if (!exist) {
				//	cout << a.tup.table_name << " doesn't exist" << endl;
				//	continue;
				//}
			exist = Judge_primarykey_values(a.tup);
			if (exist) {
				cout << "insert failed" << endl;
				break;
			}
			index.table_name = a.tup.table_name;
			for (int i = 0; i < a.tup.attr_count; i++) {
				index.index_name = "";
				index.attr_name = a.tup.attrs[i].attr_name;
				index.index_name = Find_index_name(a.tup.table_name, index.attr_name);
				if (index.index_name != "") {
					break;
				}
			}

			exist = Judge_index_exist(index);
			//-------------------test Initial_bplus_tree_from_file--- begin---
			if (exist) {
				target = im.Find_empty_bplus();
				indexname = Find_index_name(index.table_name, index.attr_name);
				indexfilename = indexname + index.table_name + "_index.rec";
				im.Initial_bplus_tree_from_file(indexfilename, target);
				//cout << "initial succeessfully!" << endl;
				//op = Change_str_to_op(a.con.op_type);
				//table = Read_Table_Info(a.con.tablename);
				int id = a.tup.searchAttrId(index.attr_name);
				int values = Change_string_to_int(a.tup.attr_values[id]);
				Block* temp = bm.Find_next_empty_block(filename);
				//----更新B+树
				bplus_tree_put(target, values, temp->offset);
				offset = Insert(a.tup);
				//im.Im_Update_index(table, table.table_name, a.ind.attr_name, a.ind.index_name);
				cout << "insert successfully!" << endl;
				cout << "1 row affected" << endl;
				break;
			}
			offset = Insert(a.tup);
			cout << "insert successfully!" << endl;
			cout << "1 row affected" << endl;

			//插入index
			break;
		case 6:
			//a.delete_tuple();

			table = Read_Table_Info(a.con.tablename);
			if (a.con.attr_name == "all") {
				Delete_all(table);
				break;
			}
			op = Change_str_to_op(a.con.op_type);
			Delete_record(table, a.con.attr_name, op, a.con.cmp_value.c_str());
			break;
		case 7:
			exist = Judge_table_exist(a.con.tablename);
			if (!exist) {
				cout << a.con.tablename << " doesn't exist" << endl;
				continue;
			}
			table = Read_Table_Info(a.con.tablename);
			if (a.con.attr_name == "*" && a.con.cmp_attr_name.empty())		 //表示select *　tablename
			{
				Select_record(table);
			}
			else if (a.con.attr_name == "*" && !a.con.cmp_attr_name.empty()) {
				op = Change_str_to_op(a.con.op_type);
				Select_record(table, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
			}
			else if (a.con.attr_name != "*" && a.con.cmp_attr_name.empty())//表示select attr from tabelname;
			{
				Select_record(table, a.con.attr_name);
			}
			else if (a.con.attr_name != "*" && !a.con.cmp_attr_name.empty())//表示select attr from tabelname where age=18;
			{
				index.attr_name = a.con.cmp_attr_name;
				index.table_name = a.con.tablename;
				index.index_name = Find_index_name(a.con.tablename, a.con.cmp_attr_name);
				exist = Judge_index_exist(index);
				//-------------------test Initial_bplus_tree_from_file--- begin---
				if (exist)
				{
					target = im.Find_empty_bplus();
					indexname = Find_index_name(index.table_name, index.attr_name);
					indexfilename = indexname + index.table_name + "_index.rec";
					im.Initial_bplus_tree_from_file(indexfilename, target);
					//cout << "initial succeessfully!" << endl;
					op = Change_str_to_op(a.con.op_type);
					table = Read_Table_Info(a.con.tablename);
					//index的select--------目前只有=有用到索引,其他没弄
					im.Im_select(target, table, a.con.attr_name, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
					cout << "select successfully!" << endl;
					cout << "0 row affected" << endl;
					break;
				}
				else {
					op = Change_str_to_op(a.con.op_type);
					Select_record(table, a.con.attr_name, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
				}
			}
			break;
			///下面不太懂，我先把上面的调试了///
/*			else															//表示select * from tablename where name = 'awx'
			{
				cout << a.con.tablename << endl;
				cout << a.con.attr_name << endl;
				cout << a.con.op_type << endl;
				cout << a.con.cmp_value << endl;

			}*/
		case 8:
		{
			//Interpreter a;
			streambuf *backup;
			ifstream fin;
			fin.open(a.filename);
			backup = cin.rdbuf();   // back up cin's streambuf
			cin.rdbuf(fin.rdbuf()); // assign file's streambuf to cin
			while (!cin.eof())
			{
				switch (a.getquery())
				{
				case 1:
					exist = Judge_table_exist(a.tab.table_name);
					if (exist) {
						cout << a.tab.table_name << " exists" << endl;
						continue;
					}
					Create_table(a.tab);
					//cout << "-----------bm.Init()----------" << endl;
					bm.Init();
					//cout << "------------Write_all_blocks_to_file--------" << endl;
					filename = a.tab.table_name + "_table.rec";
					bm.Write_all_blocks_to_file(filename);
					cout << "create " << a.tab.table_name << " successfully!" << endl;
					break;
				case 2:
					exist = Judge_table_exist(a.tab.table_name);
					if (!exist) {
						cout << a.tab.table_name << " doesn't exist" << endl;
						continue;
					}
					Drop_table(a.tab.table_name);
					cout << "drop " << a.tab.table_name << " successfully!" << endl;
					break;
				case 3:
					//a.create_index();
					exist = Judge_index_exist(a.ind);
					if (exist) {
						cout << a.ind.index_name << " exists" << endl;
						continue;
					}
					table = Read_Table_Info(a.ind.table_name);
					id = table.searchAttrId(a.ind.attr_name);
					if (id == -1) {
						cout << a.ind.attr_name << "doesn't exist" << endl;
						continue;
					}
					ty = table.attrs[id].attr_type;
					//ii.indexName = a.ind.index_name;
					//ii.tableName = a.tab.table_name;
					//ii.attribute = a.ind.attr_name;
					//ii.type = ty;
					//table = Read_Table_Info(a.ind.table_name);
					im.Im_Create_index(table, table.table_name, a.ind.attr_name, a.ind.index_name);
					cout << "create index " << a.ind.index_name << " successfully" << endl;
					break;
				case 4:
					//a.drop_index();
					//a.ind.table_name = a.tab.table_name;
					exist = Judge_index_exist(a.ind);//ind里加table_name
					if (!exist) {
						cout << a.ind.index_name << " doesn't exist" << endl;
						continue;
					}
					table = Read_Table_Info(a.ind.table_name);
					id = table.searchAttrId(a.ind.attr_name);
					ty = table.attrs[id].attr_type;
					if (ty > 0) {
						ty = table.attrs[id].attr_type;
					}
					//im.DropIndex(a.ind.index_name, ty);
					Drop_index(a.ind);
					cout << "drop index " << a.ind.index_name << " successfully" << endl;
					break;
				case 5:
					//	a.insert_tuple();

					//exist = Judge_table_exist(a.tup.table_name);
					//if (!exist) {
					//	cout << a.tup.table_name << " doesn't exist" << endl;
					//	continue;
					//}
					exist = Judge_primarykey_values(a.tup);
					if (exist) {
						cout << "insert failed" << endl;
						break;
					}
					index.table_name = a.tup.table_name;
					for (int i = 0; i < a.tup.attr_count; i++) {
						index.index_name = "";
						index.attr_name = a.tup.attrs[i].attr_name;
						index.index_name = Find_index_name(a.tup.table_name, index.attr_name);
						if (index.index_name != "") {
							break;
						}
					}

					exist = Judge_index_exist(index);
					//-------------------test Initial_bplus_tree_from_file--- begin---
					if (exist) {
						target = im.Find_empty_bplus();
						indexname = Find_index_name(index.table_name, index.attr_name);
						indexfilename = indexname + index.table_name + "_index.rec";
						im.Initial_bplus_tree_from_file(indexfilename, target);
						//cout << "initial succeessfully!" << endl;
						//op = Change_str_to_op(a.con.op_type);
						//table = Read_Table_Info(a.con.tablename);
						int id = a.tup.searchAttrId(index.attr_name);
						int values = Change_string_to_int(a.tup.attr_values[id]);
						filename= index.table_name + "_table.rec";
						Block* temp = bm.Find_next_empty_block(filename);
						//----更新B+树
						bplus_tree_put(target, values, temp->offset);
						offset = Insert(a.tup);
						//im.Im_Update_index(table, table.table_name, a.ind.attr_name, a.ind.index_name);
						cout << "insert with index successfully!" << endl;
						cout << "1 row affected" << endl;
						break;
					}
					offset = Insert(a.tup);
					cout << "insert successfully!" << endl;
					cout << "1 row affected" << endl;

					//插入index
					break;
				case 6:
					//a.delete_tuple();

					table = Read_Table_Info(a.con.tablename);
					if (a.con.attr_name == "all") {
						Delete_all(table);
						break;
					}
					op = Change_str_to_op(a.con.op_type);
					Delete_record(table, a.con.attr_name, op, a.con.cmp_value.c_str());
					break;
				case 7:
					exist = Judge_table_exist(a.con.tablename);
					if (!exist) {
						cout << a.con.tablename << " doesn't exist" << endl;
						continue;
					}
					table = Read_Table_Info(a.con.tablename);
					if (a.con.attr_name == "*" && a.con.cmp_attr_name.empty())		 //表示select *　tablename
					{
						Select_record(table);
						cout << "select successfully!" << endl;
						cout << "0 row affected" << endl;
					}
					else if (a.con.attr_name == "*" && !a.con.cmp_attr_name.empty()) {
						op = Change_str_to_op(a.con.op_type);
						Select_record(table, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
						cout << "select successfully!" << endl;
						cout << "0 row affected" << endl;
					}
					else if (a.con.attr_name != "*" && a.con.cmp_attr_name.empty())//表示select attr from tabelname;
					{
						Select_record(table, a.con.attr_name);
						cout << "select successfully!" << endl;
						cout << "0 row affected" << endl;
					}
					else if (a.con.attr_name != "*" && !a.con.cmp_attr_name.empty())//表示select attr from tabelname where age=18;
					{
						index.attr_name = a.con.cmp_attr_name;
						index.table_name = a.con.tablename;
						index.index_name = Find_index_name(a.con.tablename, a.con.cmp_attr_name);
						exist = Judge_index_exist(index);
						//-------------------test Initial_bplus_tree_from_file--- begin---
						if (exist)
						{
							target = im.Find_empty_bplus();
							indexname = Find_index_name(index.table_name, index.attr_name);
							indexfilename = indexname + index.table_name + "_index.rec";
							im.Initial_bplus_tree_from_file(indexfilename, target);
							//cout << "initial succeessfully!" << endl;
							op = Change_str_to_op(a.con.op_type);
							table = Read_Table_Info(a.con.tablename);
							//index的select--------目前只有=有用到索引,其他没弄
							//im.Im_select(target, table, a.con.attr_name, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
							Select_record(table, a.con.attr_name, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
							cout << "select with index successfully!" << endl;
							cout << "0 row affected" << endl;
							break;
						}
						else {
							op = Change_str_to_op(a.con.op_type);
							Select_record(table, a.con.attr_name, a.con.cmp_attr_name, op, a.con.cmp_value.c_str());
							cout << "select successfully!" << endl;
							cout << "0 row affected" << endl;
						}
					}
					break;
				case 9:
					cout << "-----------------------------------------------------------" << endl;
					cout << "|                                                         |" << endl;
					cout << "|                   Welcome to MiniSQL                    |" << endl;
					cout << "|                                                         |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|     Command    |                Comments                |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|                |    create table b (                    |" << endl;
					cout << "|                |    id int,                             |" << endl;
					cout << "|  create table  |    name char(12),                      |" << endl;
					cout << "|                |    balance float                       |" << endl;
					cout << "|                |    )                                   |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  create index  |   create index indexname on b(id);     |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  drop table    |   drop table b;                        |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  drop index    |   drop index indexname on b            |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  delete        |   delete from b where id = 1;          |" << endl;
					cout << "|                |   delete * from b;                     |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|                |   select * from b;                     |" << endl;
					cout << "|                |   select * from b where id = 1;        |" << endl;
					cout << "|  select        |   select id from b where name = 'la';  |" << endl;
					cout << "|                |   select id from b;                    |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  insert        |   insert into b values (1,'la',1.1);   |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  execfile      |   execfile test.txt                    |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  help          |   查看帮助                             |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					cout << "|  quit          |   退出程序                             |" << endl;
					cout << "-----------------+-----------------------------------------" << endl;
					break;
				case 10:
					return 0;
				case 0:
					break;
				default:
					//cout << "Sytanx Error!" << endl;
					break;

				}

			}
			cin.rdbuf(backup);	 // restore cin's original streambuf
			break;
		}
		case 9:
			cout << "-----------------------------------------------------------" << endl;
			cout << "|                                                         |" << endl;
			cout << "|                   Welcome to MiniSQL                    |" << endl;
			cout << "|                                                         |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|     Command    |                Comments                |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|                |    create table b (                    |" << endl;
			cout << "|                |    id int,                             |" << endl;
			cout << "|  create table  |    name char(12),                      |" << endl;
			cout << "|                |    balance float                       |" << endl;
			cout << "|                |    )                                   |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  create index  |   create index indexname on b(id);     |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  drop table    |   drop table b;                        |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  drop index    |   drop index indexname on b            |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  delete        |   delete from b where id = 1;          |" << endl;
			cout << "|                |   delete * from b;                     |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|                |   select * from b;                     |" << endl;
			cout << "|                |   select * from b where id = 1;        |" << endl;
			cout << "|  select        |   select id from b where name = 'la';  |" << endl;
			cout << "|                |   select id from b;                    |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  insert        |   insert into b values (1,'la',1.1);   |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  execfile      |   execfile test.txt                    |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  help          |   查看帮助                             |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
			cout << "|  quit          |   退出程序                             |" << endl;
			cout << "-----------------+-----------------------------------------" << endl;
				break;
		case 10:
			return 0;
		case 0:
			break;
		default:
			//cout << "Sytanx Error!" << endl;
			break;
		}
	}
}