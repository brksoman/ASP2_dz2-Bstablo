#include "Main_structure.h"


Data_w_tree& Data_w_tree::operator += (string s) {
	if (Data::insert(s) == false) { return *this; }
	if (s.length() == 0) { return *this; }

	int location = Data::find(s), len = s.length();

	B_tree::Node::Elem new_el(len, location);

	B_tree::insert(new_el);
	B_tree::update(len, increment);

	return *this;
}

Data_w_tree& Data_w_tree::operator -= (string s) {
	if (Data::remove(s) == false) { return *this; }

	int location = Data::find(s), len = s.length();

	if (str[location - 1].length() == len) {
		B_tree::update(len, decrement);
	}
	else if (str[location].length() == len) {
		B_tree::update(len - 1, decrement);
	}
	else {
		B_tree::remove(len);
		B_tree::update(len, decrement);
	}

	return *this;
}

int Data_w_tree::find(string s) {
	int location = B_tree::find_ind(s.length());
	int len = s.length();

	if (location == -1) return -1;

	while ((len < n) && (str[location].length() == len)) {
		if (str[location] == s) return location;
		location++;
	}

	return -1;
}