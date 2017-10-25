#ifndef _BufferManager_H_
#define _BufferManager_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <list>
#include "minisql.h"
#include "bplustree.h"
#define BlockSize 4096
#define WrittenSize 8
#define AccessedSize 8
#define RecordSize 4032
#define TablenameSize 32
#define BlankSize 12
#define OffsetSize 4
#define AttrbuteSize 32
using namespace std;

struct Block
{
	char* tablename;   //32 bit
	char* written;		//8  bit
	char* accessed;		//8  bit
	int offset;			//4 bit
	char* blank;        //空出12bit
	char* record;       //block剩下全部的空间，一个attribute占用32bit
};

class MBuffer {
public:
	Block* Buffer[BLOCKNUM];
public:
	MBuffer() {
		int i;
		for (i = 0; i<BLOCKNUM; i++)
			Buffer[i] = NULL;
	};
	bool Init();
	bool Write_all_blocks_to_file(string filename);
	Block* GBlock();
	Block* GetBlock(string filename, int offset);
	Block* Find_next_empty_block(string filename);
	bool Write_index_to_file(string indexfilename, Index index, vector<int>key, vector<int>offset);
	bool Write_block_to_file(string filename,Block* block);
	bool Read_index_from_file(string indexfilename, bplus_tree* tree, vector<int>&key, vector<int>&offset);
	bool Read_block_from_file(string filename);
	bool Show_block(Block* block);
	bool Show_all_blocks(Block* block[BLOCKNUM]);
	bool Delete_buffer(Block* block);
	bool Delete_buffer(string filename,int offset);
};
#endif 
