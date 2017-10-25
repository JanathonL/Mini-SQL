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
	int key[MAX_ENTRIES];              //key �൱������
	int data[MAX_ENTRIES];             //data �൱��offset
};

class bplus_tree {
public:
	int order;
	int entries;
	int level;
	string table_name;
	string attr_name;    //������������
	string index_name;   //������
	bplus_node *root;
	bplus_node *head[MAX_LEVEL];
	~bplus_tree() {
		delete root;
		delete[] head;
	}
};
//չ�ֳ����B+��
void bplus_tree_dump(bplus_tree *tree);
//����key�õ�����
int bplus_tree_get(bplus_tree *tree, int key);
//ɾ��������ӽڵ� data��0 ɾ�� 
int bplus_tree_put(bplus_tree *tree, int key, int data);

void Get_tree_key_and_offset(bplus_tree *tree, vector<int>& key, vector<int>& data);

int bplus_tree_get_range(bplus_tree *tree, int key1, int key2);

//��ʼ��B+��
bplus_tree* bplus_tree_init(int level, int order, int entries, string table_name, string attr_name, string index_name);
void bplus_tree_deinit(bplus_tree *tree);


#endif  /* _BPLUS_TREE_H */
