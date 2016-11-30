#include "Data-strings.h"
#include "B_tree.h"

int main() {
	B_tree b(3);
	char c;
	int n;

	cin >> n;
	for (int i = 0; i < n; i++) {
		int k;
		cin >> k;
		b.insert(k);
		cout << endl << endl << b << endl << endl;
	}

	cin >> n;
	for (int i = 0; i < n; i++) {
		int k;
		cin >> k;
		b.remove(k);
		cout << endl << endl << b << endl << endl;
	}
}