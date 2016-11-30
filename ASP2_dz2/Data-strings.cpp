#include "Data-strings.h"



// ------------------------------------------------------------------------------------------------ General -----------------------------------------------------------------------

Data::Data(string * s, int nn) : n(nn), str(s) {
	len = new int[n];
	for (int i = 0; i < n; i++) len[i] = s[i].length();
	sort_data();
}

int Data::find(const string& s) const {
	int low = 0, high = n - 1;

	while (low <= high) {
		int mid = (low + high) / 2;
		if (s == str[mid])
			return mid;
		else if (is_greater(s, mid))
			low = mid + 1;
		else
			high = mid - 1;
	}
	if (is_greater(s, low)) low++;
	return low;
}

bool Data::insert(const string& s) {
	int ind = find(s);
	if (!non_legit(ind) && (s == str[ind])) return false;

	n++;
	string * nstr = new string[n];
	int * nlen = new int[n];

	for (int i = 0; i < ind; i++) {
		nstr[i] = str[i];
		nlen[i] = len[i];
	}
	for (int i = ind + 1; i < n; i++) {
		nstr[i] = str[i - 1];
		nlen[i] = len[i - 1];
	}

	nstr[ind] = s;
	nlen[ind] = s.length();

	delete[n - 1] str;
	delete[n - 1] len;
	str = nstr;
	len = nlen;

	return true;
}

bool Data::remove(const string& s) {
	int ind = find(s);
	if (non_legit(ind) || (s != str[ind])) return false;

	n--;
	string * nstr = new string[n];
	int * nlen = new int[n];

	for (int i = 0; i < ind; i++) {
		nstr[i] = str[i];
		nlen[i] = len[i];
	}
	for (int i = ind; i < n; i++) {
		nstr[i] = str[i + 1];
		nlen[i] = len[i + 1];
	}

	delete[n + 1] str;
	delete[n + 1] len;
	str = nstr;
	len = nlen;

	return true;
}


// ------------------------------------------------------------------------------------------------ Input / Output ----------------------------------------------------------------

istream& operator >> (istream& in, Data& d) {
	string s = " ";
	while (true) {
		getline(in, s);
		if (s == "") break;

		d.insert(s);
	}
	return in;
}

ostream& operator << (ostream& out, const Data& d) {
	for (int i = 0; i < d.n; i++) {
		out << d.str[i] << endl;
	}
	out << endl;
	return out;
}


// ------------------------------------------------------------------------------------------------ Quicksort ---------------------------------------------------------------------

int Data::partition(int low, int high) {			// Pivot is Data[high]. Put all lower than pivot left of pivot, and all higher than pivot right of pivot.
	int i = low;

	for (int j = low; j < high; j++) {
		if (is_greater(high, j)) {
			swap(i, j);
			i++;
		}
	}
	swap(i, high);
	return i;
}

void Data::sort_data() {
	Stack s;
	int low = 0, high = n - 1;

	s.push(low);
	s.push(high);

	while (!s.is_empty()) {
		high = s.pop();
		low = s.pop();

		int pivot = partition(low, high);
		// If there are elements lower than pivot, push that subarray to stack.
		if (pivot - 1 > low) {
			s.push(low);
			s.push(pivot - 1);
		}
		// If there are elements higher than pivot, push that subarray to stack.
		if (pivot + 1 < high) {
			s.push(pivot + 1);
			s.push(high);
		}
	}
}
