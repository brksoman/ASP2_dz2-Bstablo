#include "B_tree.h"
// ------------------------------------------------------------------------------------------------ General -----------------------------------------------------------------------

B_tree::Node::Node(int m) : parent(nullptr), n(1) {
	el = new Elem[m - 1];
	child = new Node*[m];
	for (int i = 0; i < m; i++) child[i] = nullptr;
}

void B_tree::Node::destruct(int m) {
	delete[m - 1] el;
	delete[m] child;
}

B_tree::~B_tree() {
	Node_queue q;
	if (root != nullptr) q.insert(root);

	while (!q.is_empty()) {
		Node * temp = q.remove();

		delete[m - 1] temp->el;

		if (!temp->is_leaf())
			for (int i = 0; i < temp->n; i++)
				q.insert(temp->child[i]);

		delete[m] temp->child;
		delete temp;
	}
}

int B_tree::height() const {
	Node * temp = root;
	int h = 0;
	while (temp) {
		temp = temp->child[0];
		h++;
	}
	return h - 1;
}

int B_tree::num_of_keys() const {
	int num = 0;
	Node_queue q;
	if (root != nullptr) q.insert(root);

	while (!q.is_empty()) {
		Node * temp = q.remove();

		num += temp->n - 1;

		if (!temp->is_leaf())
			for (int i = 0; i < temp->n; i++)
				q.insert(temp->child[i]);
	}

	return num;
}


// ------------------------------------------------------------------------------------------------ Find - Standard / With pointer to Node / In Node ------------------------------

int B_tree::Node::find(int k) const {
	if (n == 1) { return 0; }
	int low = 0, high = n - 2;

	while (low <= high) {
		int mid = (low + high) / 2;

		if (k == el[mid].key) {
			return mid;
		}
		else if (k > el[mid].key) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	return low;				// Returns index above the spot where x should be
}

bool B_tree::find_with_pointer(int k, Node* & loc) const {
	Node * cur = root;

	while (cur) {				// while current is legit and not leaf
		loc = cur;
		int index = cur->find(k);

		if (k == cur->el[index].key) { return true; }
		cur = cur->child[index];
	}

	return false;
}

bool B_tree::find(int k) const {
	Node * temp;
	return find_with_pointer(k, temp);
}

int B_tree::find_ind(int k) const {
	Node * temp;
	if (find_with_pointer(k, temp) == false) { return -1; }

	return temp->el[temp->find(k)].ind;
}

// ------------------------------------------------------------------------------------------------ Insert / Node splitting -------------------------------------------------------

bool B_tree::Node::insert(Elem new_el, Node* new_child) {			// Add new_el to Node ; Doesn't check if Node is full
	int index = find(new_el.key);
	if (new_el.key == el[index].key) { return false; }

	n++;
	for (int i = n - 2; i > index; i--) {
		el[i] = el[i - 1];
		child[i + 1] = child[i];
	}

	el[index] = new_el;
	child[index + 1] = new_child;
	if (new_child != nullptr) { new_child->parent = this; }
	return true;
}

bool B_tree::insert(Node::Elem new_el) {
	Node * cur = root;
	if (find_with_pointer(new_el.key, cur)) { return false; }

	if (!cur) { root = cur = new Node(m); }

	Node * last_split = nullptr;

	while ((cur != nullptr) && (cur->n == m)) {
		last_split = split_node(cur, new_el, last_split);
		
		if (cur->parent != nullptr) {
			cur = cur->parent;
		}
		else {
			Node * temp = new Node(m);
			temp->child[0] = cur;
			cur->parent = last_split->parent = temp;
			root = cur = temp;
		}
	}

	cur->insert(new_el, last_split);
	return true;
}

bool B_tree::insert(int x) {
	Node::Elem new_el(x, 0);
	return insert(new_el);
}

B_tree::Node* B_tree::split_node(Node* cur, Node::Elem& new_el, Node* last_split) {
	Node * temp = new Node(m + 1);
	bool new_is_added = false;

	for (int i = 0; i < m; i++) {
		if (!new_is_added) {
			if ((i == m - 1) || (cur->el[i].key > new_el.key)) {
				temp->el[i] = new_el;
				temp->child[i + 1] = last_split;
				new_is_added = true;
			}
			else {
				temp->el[i] = cur->el[i];
			}
			temp->child[i] = cur->child[i];
		}
		else {
			temp->el[i] = cur->el[i - 1];
			temp->child[i + 1] = cur->child[i];
		}
	}

	int cut = m / 2;
	new_el = temp->el[cut];
	Node* new_node = new Node(m);

	for (int i = 0; i < cut; i++) {
		cur->el[i] = temp->el[i];
		cur->child[i] = temp->child[i];
		if (cur->child[i] != nullptr) { cur->child[i]->parent = cur; }
	}
	cur->child[cut] = temp->child[cut];
	if (cur->child[cut] != nullptr) { cur->child[cut]->parent = cur; }

	for (int i = cut + 1; i < m; i++) {
		new_node->el[i - cut - 1] = temp->el[i];
		new_node->child[i - cut - 1] = temp->child[i];
		if (new_node->child[i - cut - 1] != nullptr) { new_node->child[i - cut - 1]->parent = new_node; }
		
		cur->child[i] = nullptr;
	}
	new_node->child[m - cut - 1] = temp->child[m];
	if (new_node->child[m - cut - 1] != nullptr) { new_node->child[m - cut - 1]->parent = new_node; }

	cur->n = cut + 1;
	new_node->n = m - cut;

	temp->destruct(m + 1);
	delete temp;

	return new_node;
}

// Update higher indexes after insert
void B_tree::update(int k, bool direction) {
	Node_queue q;
	int shift = (direction) ? (1) : (-1);

	if (root != nullptr) { q.insert(root); }

	while (!q.is_empty()) {
		Node * temp = q.remove();

		for (int i = 0; i < temp->n - 1; i++) { if (temp->el[i].key > k) { temp->el[i].key += shift; } }
		if (!temp->is_leaf()) { for (int i = 0; i < temp->n; i++) { q.insert(temp->child[i]); } }
	}
}

// ------------------------------------------------------------------------------------------------ Remove / Merge Nodes ----------------------------------------------------------

bool B_tree::Node::remove_ind(int location) {
	if ((location > n - 2) || (location < 0)) { return false; }

	n--;
	for (int i = location; i < n - 1; i++) {
		el[i] = el[i + 1];
		child[i + 1] = child[i + 2];
	}
	child[n] = nullptr;
}

bool B_tree::Node::remove(int x) {
	int location = find(x);
	if (el[location].key != x) { return false; }
	return remove_ind(location);
}

bool B_tree::remove(int x) {
	Node * cur;
	if (!find_with_pointer(x, cur)) { return false; }
	int divider = cur->find(x);

	if (cur->is_leaf() == false) {
		Node * temp = succ(cur, divider);

		cur->el[divider] = temp->el[0];
		cur = temp;
		divider = 0;
	}

	while (cur != nullptr) {
		cur->remove_ind(divider);

		if ((cur->n == 1) && (cur == root)) {
			if (cur->child[0] != nullptr) { cur->child[0]->parent = nullptr; }
			root = cur->child[0];
			cur->destruct(m);
			delete cur;
			break;
		}

		if (cur->n >= m / 2 + m % 2) { break; }

		Node * bro;
		int q = check_bros(cur, bro, divider);

		if ((q == right_bro_give) || (q == left_bro_give)) {
			give(cur, bro, divider, q);
			break;
		}

		merge(cur, bro, divider, q);
		cur = cur->parent;
	}
	return true;
}


int B_tree::check_bros(Node* cur, Node* & bro, int& divider) {
	Node * par = cur->parent, * bro1 = nullptr, * bro2 = nullptr;

	divider = par->find(cur->el[0].key);

	if (divider + 1 < par->n) bro1 = par->child[divider + 1];
	if (divider - 1 >= 0) bro2 = par->child[divider - 1];

	int threshold = m / 2 + m % 2;

	if (bro1 && (bro1->n > threshold)) { bro = bro1;			return right_bro_give; }
	if (bro2 && (bro2->n > threshold)) { bro = bro2; divider--; return left_bro_give; }
	if (bro1) { bro = bro1;			   return right_bro_merge; }
	if (bro2) { bro = bro2; divider--; return left_bro_merge; }
}

void B_tree::give(Node* cur, Node* bro, int divider, int q) {
	if ((q != right_bro_give) && (q != left_bro_give)) { return; }

	Node * par = cur->parent;
	cur->n++;
	bro->n--;

	if (q == right_bro_give) {
		cur->el[cur->n - 2] = par->el[divider];
		par->el[divider] = bro->el[0];
		cur->child[cur->n - 1] = bro->child[0];
		if (cur->child[cur->n - 1] != nullptr) { cur->child[cur->n - 1]->parent = cur; }


		for (int i = 0; i < bro->n - 1; i++) {
			bro->el[i] = bro->el[i + 1];
			bro->child[i] = bro->child[i + 1];
		}
		bro->child[bro->n - 1] = bro->child[bro->n];
		bro->child[bro->n] = nullptr;
	}
	else {
		for (int i = 1; i < cur->n - 1; i++) {
			cur->el[i] = cur->el[i - 1];
			cur->child[i] = cur->child[i - 1];
		}
		cur->child[cur->n - 1] = cur->child[cur->n - 2];

		cur->el[0] = par->el[divider];
		par->el[divider] = bro->el[bro->n - 1];
		cur->child[0] = bro->child[bro->n];
		if (cur->child[0] != nullptr) { cur->child[0]->parent = cur; }
		bro->child[bro->n] = nullptr;
	}
	
}

void B_tree::merge(Node*& cur, Node*& bro, int divider, int q) {
	if ((q != right_bro_merge) && (q != left_bro_merge)) { return; }

	Node * par = cur->parent;

	if (q == left_bro_merge) {
		Node * temp = cur;
		cur = bro;
		bro = temp;
	}

	cur->el[cur->n - 1] = par->el[divider];

	for (int i = 0; i < bro->n - 1; i++) {
		cur->el[cur->n + i] = bro->el[i];
		cur->child[cur->n + i] = bro->child[i];
		if (cur->child[cur->n + i] != nullptr) { cur->child[cur->n + i]->parent = cur; }
		bro->child[i] = nullptr;
	}
	cur->child[cur->n + bro->n - 1] = bro->child[bro->n - 1];
	if (cur->child[cur->n + bro->n - 1] != nullptr) { cur->child[cur->n + bro->n - 1]->parent = cur; }
	bro->child[bro->n - 1] = nullptr;

	cur->n += bro->n;

	bro->destruct(m);
	delete bro;
	par->child[divider + 1] = nullptr;
}


B_tree::Node* B_tree::succ(Node* cur, int location) {
	Node * temp = cur->child[location + 1];
	while (temp->is_leaf() == false) { temp = temp->child[0]; }
	return temp;
}


// ------------------------------------------------------------------------------------------------ Output ------------------------------------------------------------------------

ostream& out(ostream& out, const B_tree& b) {
	B_tree::Node_queue q;
	if (b.root != nullptr) q.insert(b.root);
	int level_nodes = 0;
	int total_level = 1;
	int total_next_level = 0;

	while (!q.is_empty()) {
		B_tree::Node * temp = q.remove();

		for (int i = 0; i < temp->n - 1; i++) {
			out << '(' << temp->el[i].key << ',' << temp->el[i].ind << ") ";
		}

		out << "  ";
		level_nodes++;

		total_next_level += temp->n;
		if (!temp->is_leaf()) { for (int i = 0; i < temp->n; i++) { q.insert(temp->child[i]); } }

		if (level_nodes == total_level) {
			level_nodes = 0;
			total_level = total_next_level;
			total_next_level = 0;
			out << endl;
		}
	}

	return out;
}

