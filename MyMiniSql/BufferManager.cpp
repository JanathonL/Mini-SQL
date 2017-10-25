#include "BufferManager.h"


bool MBuffer::Init()
{
	for (int i = 0; i < BLOCKNUM; i++) {
		Buffer[i] = GBlock();
		Buffer[i]->offset = i;
		if (Buffer[i] == NULL || Buffer[i]->record == NULL) {
			return false;
		}
	}
	return true;
}

bool MBuffer::Write_all_blocks_to_file(string filename)
{
	remove(filename.c_str());
	fstream out;
	out.open(filename.c_str(), ios::app);
	out.close();
	bool ok = true;
	for (int i = 0; i < BLOCKNUM && ok; i++) {
		ok = Write_block_to_file(filename, Buffer[i]);
	}
	return true;
}

Block * MBuffer::GBlock()
{
	Block* Tmp;
	Tmp = new Block;
	Tmp->tablename = new char[TablenameSize];
	Tmp->written = new char[WrittenSize];
	Tmp->accessed = new char[AccessedSize];
	Tmp->blank = new char[BlankSize];
	Tmp->record = new char[RecordSize];
	Tmp->offset = 0;
	memset(Tmp->tablename, 0, sizeof(Tmp->tablename));
	memset(Tmp->written, 0, sizeof(Tmp->written));
	memset(Tmp->accessed, 0, sizeof(Tmp->accessed));
	memset(Tmp->blank, 0, sizeof(Tmp->blank));
	memset(Tmp->record, 0, sizeof(Tmp->record));
	strcpy(Tmp->tablename, "this is tablename");
	strcpy(Tmp->record, "this is record");
	strcpy(Tmp->written, "0");
	strcpy(Tmp->accessed, "0");

	return Tmp;
}

Block * MBuffer::GetBlock(string filename, int offset)
{
	char FileName[BLOCKNUM];
	memset(FileName, 0, sizeof(FileName));
	for (int i = 0; i < filename.length(); i++) FileName[i] = filename.at(i);
	FILE* fp = fopen(FileName, "rb");
	if (!fp) {
		cout << "can't open " << filename << endl;
		return NULL;
	}
	fseek(fp, BlockSize*offset, SEEK_SET);
	Block* block = GBlock();  //block是新开的一个Block
	fread(block->tablename, TablenameSize, 1, fp);
	fread(block->written, WrittenSize, 1, fp);
	fread(block->accessed, AccessedSize, 1, fp);
	fread(&(block->offset), OffsetSize, 1, fp);
	fread(block->blank, BlankSize, 1, fp);
	fread(block->record, RecordSize, 1, fp);
	block->offset = offset;
	strcpy(block->accessed, "1");
	fclose(fp);
	return block;
}

Block * MBuffer::Find_next_empty_block(string filename)
{
	Block* block;
	int i = 0;
	for (i = 0; i < BLOCKNUM; i++) {
		block = GetBlock(filename, i);
		if (strcmp(block->tablename, "") == 0 || strcmp(block->written, "0") == 0) {
			if (DEBUG) {
				cout << "Find_next_empty_block:fisrt condition is true" << endl;
				cout << "offset:" << i << endl;
			}
			return block;
		}
	}
	//满了
	i = 0;
	Read_block_from_file(filename); //把数据先读进Buffer中
	while (1) {
		//fseek(fp, BlockSize*i, SEEK_CUR);
		block = Buffer[i];
		if (strcmp(block->accessed, "0") == 0) {
			strcpy(block->accessed, "1");
			return block;
		}
		else {
			strcpy(block->accessed, "0");
			//block->accessed = false;
		}
		i++;
		if (i == BLOCKNUM) i = 0;
	}
}

bool MBuffer::Write_index_to_file(string indexfilename, Index index, vector<int> key, vector<int> offset)
{
	ofstream out;
	out.open(indexfilename, iostream::app);
	out << index.index_name << " " << index.table_name << " " << index.attr_name<<endl;
	for (int i = 0; i < key.size(); i++) {
		out << key[i] << " " << offset[i] << endl;
	}

	return true;
}

bool MBuffer::Write_block_to_file(string filename, Block * block)
{
	int offset = block->offset;

	FILE* fp = fopen(filename.c_str(), "rb+");
	if (!fp) {
		printf("can't open %s\n", filename.c_str());
		return false;
	}
	fseek(fp, BlockSize*offset, SEEK_SET);
	fwrite(block->tablename, TablenameSize, 1, fp);
	fwrite(block->written, WrittenSize, 1, fp);
	fwrite(block->accessed, AccessedSize, 1, fp);
	fwrite(&(block->offset), OffsetSize, 1, fp);
	fwrite(block->blank, BlankSize, 1, fp);
	fwrite(block->record, RecordSize, 1, fp);
	fclose(fp);
	return true;
}

bool MBuffer::Read_index_from_file(string indexfilename, bplus_tree * tree, vector<int>& key, vector<int> &offset)
{

	ifstream in;
	string index_rec = indexfilename;
	in.open(index_rec, ios::in);
	if (!in) {
		cout << "can't open " << index_rec << endl;
		return false;
	}
	int i = 0;
	in >> tree->index_name;
	in >> tree->table_name;
	in >> tree->attr_name;
	while (!in.eof()) {
		int key_value, data_value;
		in >> key_value;
		in >> data_value;
		key.push_back(key_value);
		offset.push_back(data_value);
	}
	return true;
}

bool MBuffer::Read_block_from_file(string filename)
{
	for (int i = 0; i < BLOCKNUM; i++) {
		Buffer[i] = GetBlock(filename, i);
		if (Buffer[i] == NULL) {
			return false;
		}
	}
	return true;
}

bool MBuffer::Show_block(Block * block)
{
	if (block != NULL) {
		cout << "tablename:" << block->tablename;
		cout << "\twritten:" << block->written;
		cout << "\taccessed:" << block->accessed;
		cout << "\toffset:" << block->offset << endl;
		cout << "record:" << block->record << endl;
		return true;
	}
	return false;
}

bool MBuffer::Show_all_blocks(Block * block[BLOCKNUM])
{
	bool ok = true;
	for (int i = 0; i < BLOCKNUM && ok; i++) {
		ok = Show_block(block[i]);
		if (!ok) return false;
	}
	return true;
}

bool MBuffer::Delete_buffer(Block* block)
{
	int offset = block->offset;
	string filename = block->tablename;
	filename += "_table.rec";
	FILE* fp = fopen(filename.c_str(), "rb+");
	if (!fp) {
		printf("can't open %s\n", filename.c_str());
		return false;
	}
	fseek(fp, offset, SEEK_SET);
	Block* taget = Buffer[offset];
	Buffer[offset] = GBlock();
	Block* new_block = Buffer[offset];
	new_block->offset = offset;
	Write_block_to_file(filename, new_block);
	delete[] taget->record;
	delete[] taget->blank;
	delete[] taget->accessed;
	delete[] taget->written;
	delete[] taget->tablename;
	delete taget;
	return false;
}

bool MBuffer::Delete_buffer(string filename, int offset)
{
	FILE* fp = fopen(filename.c_str(), "rb+");
	if (!fp) {
		printf("can't open %s\n", filename.c_str());
		return false;
	}
	fseek(fp, offset, SEEK_SET);
	Block* taget = Buffer[offset];
	Buffer[offset] = GBlock();
	Block* new_block = Buffer[offset];
	new_block->offset = offset;
	Write_block_to_file(filename, new_block);
	delete[] taget->record;
	delete[] taget->blank;
	delete[] taget->accessed;
	delete[] taget->written;
	delete[] taget->tablename;
	delete taget;
	return true;
}


