#ifndef _data_strings_h_
#define _data_strings_h_

#include <iostream>
#include <fstream>
#include <string>
#include "Stack.h"

using namespace std;


class Data {
protected:
	string * str;
	int * len;
	int n;

public:
	Data() : n(0), str(nullptr), len(nullptr) {}
	Data(string * s, int nn);
	Data(Data&& d) : n(d.n), str(d.str), len(d.len) {
		d.str = nullptr;
		d.len = nullptr;
	}

	~Data() { delete[n] str; delete[n] len; }

	string get(int ind) const {
		if (non_legit(ind)) return "";
		return str[ind];
	}
	int get_num() const { return n; }

	int find(const string&) const;
	bool insert(const string&);
	bool remove(const string&);

	friend istream& operator >> (istream&, Data&);
	friend ostream& operator << (ostream&, const Data&);

private:
	bool non_legit(int ind) const {
		if ((ind < 0) || (ind >= n)) return true;
		return false;
	}
	bool swap(int i, int j) {
		if (non_legit(i) || non_legit(j)) return false;

		str[i].swap(str[j]);
		int p = len[i];
		len[i] = len[j];
		len[j] = p;

		return true;
	}
	bool is_greater(int i, int j) const {
		if (non_legit(i) || non_legit(j)) return false;

		if (len[i] > len[j]) return true;
		else if (len[i] < len[j]) return false;
		else if (str[i] > str[j]) return true;
		else return false;
	}
	bool is_greater(const string& s, int i) const {
		if (non_legit(i)) return false;

		int l = s.length();
		if (l > len[i]) return true;
		else if (l < len[i]) return false;
		else if (s > str[i]) return true;
		else return false;
	}

	int partition(int, int);
	void sort_data();
};



#endif
