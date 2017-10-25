#pragma once
/*
* Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
*/
#include<string>
#include<vector>
using namespace std;
#ifndef _BPLUS_TREE_H
#define _BPLUS_TREE_H

#define MIN_ORDER        3
#define MAX_ORDER        BLOCKNUM
#define MAX_ENTRIES      BLOCKNUM
#define MAX_LEVEL        10
#define BLOCKNUM 1000


typedef class bplus_non_leaf {
public:
	int type;
	bplus_non_leaf *parent;
	bplus_non_leaf *next;
	int children;
	int key[MAX_ORDER - 1];
	class bplus_node *sub_ptr[MAX_ORDER];
}*non_leaf;

typedef class bplus_node {
public:
	int type;
	non_leaf parent;
};


class bplus_leaf {
public:
	int type;
	bplus_non_leaf *parent;
	bplus_leaf *next;
	int entries;
	int key[MAX_ENTRIES];              //key 相当于数据
	int data[MAX_ENTRIES];             //data 相当于offset
};

class bplus_tree {
public:
	int order;
	int entries;
	int level;
	string table_name;
	string attr_name;    //索引的属性名
	string index_name;   //索引名
	bplus_node *root;
	bplus_node *head[MAX_LEVEL];
	~bplus_tree() {
		delete root;
		delete[] head;
	}
};
//展现出这课B+树
void bplus_tree_dump(bplus_tree *tree);
//根据key得到数据
int bplus_tree_get(bplus_tree *tree, int key);
//删除或者添加节点 data：0 删除 
int bplus_tree_put(bplus_tree *tree, int key, int data);

void Get_tree_key_and_offset(bplus_tree *tree, vector<int>& key, vector<int>& data);

int bplus_tree_get_range(bplus_tree *tree, int key1, int key2);

//初始化B+树
bplus_tree* bplus_tree_init(int level, int order, int entries, string table_name, string attr_name, string index_name);
void bplus_tree_deinit(bplus_tree *tree);


#endif  /* _BPLUS_TREE_H */
