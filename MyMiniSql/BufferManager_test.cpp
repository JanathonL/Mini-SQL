#include "BufferManager.h"
#include "RecordManager.h"
#include "CatalogManager.h"
int main() {
	MBuffer bm;
	bm.Init();
	bm.Write_all_blocks_to_file("table.rec");
//	bm.Find_next_empty_block()
	bm.Read_block_from_file("table.rec");
	bm.Show_all_blocks(bm.Buffer);
	cout << "test Find_next_empty_block" << endl;
	Block* block = new Block;
	block=bm.Find_next_empty_block("table.rec");
	bm.Show_block(block);
	cout << "after Find_next_empty_block" << endl;
	bm.Show_all_blocks(bm.Buffer);
	cout << "Test Insert Tuple" << endl;


}