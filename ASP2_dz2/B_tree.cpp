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

bool B_tree::find_with_pointer(int x, Node* & loc) const {
	Node * cur = root;

	while (cur) {				// while current is legit and not leaf
		loc = cur;
		int index = cur->find(x);

		if (x == cur->el[index].key) { return true; }
		cur = cur->child[index];
	}

	return false;
}

bool B_tree::find(int x) const {
	Node * temp;
	return find_with_pointer(x, temp);
}

int B_tree::Node::find(int x) const {
	if (n == 1) { return 0; }
	int low = 0, high = n - 2;

	while (low <= high) {
		int mid = (low + high) / 2;

		if (x == el[mid].key) { 
			return mid;
		}
		else if (x > el[mid].key) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}

	return low;				// Returns index above the spot where x should be
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

/*B_tree::Node* B_tree::split_node_alt(Node* cur, Node::Elem& new_el, Node * last_split) {		// Returns the Node on the right (after split) and updates new_el ; used in B_tree::insert as last_split
	int mid = m / 2;					// el[mid] - First member of the new Node
	Node * temp_child = last_split;		// Potential first child of the new Node

	if (!((cur->el[mid - 1].key < new_el.key) && (new_el.key < cur->el[mid].key))) {		// If new_el is not the median (if it is, than new_member is the new new_member)
		Node::Elem temp_el = new_el;					// new_el is readied to be placed into the current Node
		int new_location = cur->find(temp_el.key);	// Location of new_el (one index above)

		temp_child = cur->child[mid];		// Actual first child of the new Node

		if (new_el.key < cur->el[mid - 1].key) {		// If new_el is below the median
			new_el = cur->el[mid - 1];		// el[mid - 1] should be sent to the parent

			for (int i = new_location + 1; i < mid; i++) {		// Make space for new_el in the current Node
				cur->el[i] = cur->el[i - 1];
				cur->child[i + 1] = cur->child[i];
			}

		}
		else {									// If new_el is above the median
			new_location--;
			new_el = cur->el[mid];			// el[mid] should be sent to the parent

			for (int i = new_location - 1; i >= mid; i--) {		// Make space for new_ in the current Node
				cur->el[i] = cur->el[i + 1];
				cur->child[i + 1] = cur->child[i + 2];
			}
			cur->child[mid] = cur->child[mid + 1];				// child[mid] isn't reached in loop
		}

		cur->el[new_location] = temp_el;					// Put new_el into the Node
		cur->child[new_location + 1] = last_split;			// Add last_split as its right child
	}

	cur->loc_m = mid + 1;				// New value: there is No(mid) members => there is No(mid + 1) children
	Node * temp = new Node(m);			// Initialise new Node
	temp->loc_m = m - cur->loc_m + 1;	// cur->loc_m + temp->loc_m = m + 1

	for (int i = 0; i < temp->loc_m - 1; i++) {			// Put members[mid .. m - 1] into new Node
		temp->el[i] = cur->el[mid + i];
		temp->child[i + 1] = cur->child[mid + i + 1];
		temp->child[i + 1]->parent = temp;
		cur->child[mid + i + 1] = nullptr;
	}
	temp->child[0] = temp_child;			// First child of new Node

	return temp;		// return new Node (right Node of split)
}*/

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

	for (int i = cut + 1; i < m; i++) {
		new_node->el[i - cut - 1] = temp->el[i];
		new_node->child[i - cut - 1] = temp->child[i];
		if (new_node->child[i - cut - 1] != nullptr) { new_node->child[i - cut - 1]->parent = new_node; }
		
		cur->child[i] = nullptr;
	}
	new_node->child[m - cut - 1] = temp->child[m];

	cur->n = cut + 1;
	new_node->n = m - cut;

	temp->destruct(m + 1);
	delete temp;

	return new_node;
}

bool B_tree::insert(int x) {
	Node::Elem new_el(x, 0);
	return insert(new_el);
}

// ------------------------------------------------------------------------------------------------ Remove / Merge Nodes ----------------------------------------------------------

bool B_tree::remove(int x) {
	Node * cur;
	if (!find_with_pointer(x, cur)) { return false; }

	while (cur != nullptr) {
		cur->remove(x);

		if (cur->n >= m / 2 + m % 2) { break; }

		Node * bro;
		int divider;
		int q = check_bros(cur, bro, divider);

		if ((q == right_bro_give) || (q == left_bro_give)) {
			give(cur, bro, divider, q);
			break;
		}

		merge(cur, bro, divider, q);
		x = divider;
		cur = cur->parent;
	}
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

bool B_tree::Node::remove(int x) {
	int location = find(x);
	if (el[location].key != x) { return false; }

	n--;
	for (int i = location; i < n - 1; i++) {
		el[i] = el[i + 1];
		child[i + 1] = child[i + 2];
	}
	child[n] = nullptr;
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
		cur->child[cur->n - 1]->parent = cur;


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
		cur->child[0]->parent = cur;
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
		cur->child[cur->n + i]->parent = cur;
		bro->child[i] = nullptr;
	}
	cur->child[cur->n + bro->n - 1] = bro->child[bro->n - 1];
	cur->child[cur->n + bro->n - 1]->parent = cur;
	bro->child[bro->n - 1] = nullptr;

	cur->n += bro->n;

	bro->destruct(m);
	delete bro;
	par->child[divider + 1] = nullptr;
}

// ------------------------------------------------------------------------------------------------ Output ------------------------------------------------------------------------

ostream& operator << (ostream& out, const B_tree& b) {
	B_tree::Node_queue q;
	if (b.root != nullptr) q.insert(b.root);
	int level_nodes = 0;
	int total_level_nodes = 1;

	while (!q.is_empty()) {
		B_tree::Node * temp = q.remove();

		for (int i = 0; i < temp->n - 1; i++) {
			out << '(' << temp->el[i].key << ',' << temp->el[i].ind << ") ";
		}

		out << "  ";
		level_nodes++;

		if (level_nodes == total_level_nodes) {
			level_nodes = 0;
			total_level_nodes *= temp->n;
			out << endl;
		}

		if (!temp->is_leaf()) {
			for (int i = 0; i < temp->n; i++) { q.insert(temp->child[i]); }
		}
	}

	return out;
}

