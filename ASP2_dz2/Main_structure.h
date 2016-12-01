#ifndef _main_structure_h_
#define _main_structure_h_

#include "Data-strings.h"
#include "B_tree.h"

class Data_w_tree : public Data, public B_tree {

public:
	Data_w_tree(int mm = 3) : B_tree(mm) {}
	Data_w_tree(Data_w_tree&) = delete;

	Data_w_tree& operator += (string s);
	Data_w_tree& operator -= (string s);

	int find(string s);
};



#endif
