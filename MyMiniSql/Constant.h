#pragma once
/*
* Constant.h
*
*  Created on: 2014Äê10ÔÂ31ÈÕ
*      Author: darren
*/

#ifndef CONSTANT_H_
#define CONSTANT_H_
#include <string>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
using namespace std;
const string FCATALOG = "catalog";
const string FDATA = "data";
const int INT = 0;
const int FLOAT = 1;
const int CHAR = 2;
const int BLOCK_SIZE = 4096;

const int NORMAL = 0;
const int UNIQUE = 1;
const int PRIMARY = 2;
const int LENGTH_OF_INT = 4;
const int LENGTH_OF_FLOAT = 4;
const int LENGTH_OF_CHAR = 1;

const int FIRST_TABLE_LOCATION = 0x20;
const int DATA_END = 0xee;
const int DELETE = 0xe5;
const int DATA_LOCATION = 0x1;

//constant for buffer manager
const int BUFFER_MANAGER_SIZE = 100;

const int EQ = 1;
const int LT = 2;
const int GT = 3;
const int LE = 4;
const int GE = 5;
const int NE = 6;
//const char* DBFILE = "RecordFile";
#endif /* CONSTANT_H_ */
