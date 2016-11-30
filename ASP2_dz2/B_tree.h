#ifndef __B_TREE_STRINGS_
#define __B_TREE_STRINGS_

#include <string>

using namespace std;

const int MAX_NODE_STACK_HEIGHT = 100;
const int queue_size = 100;

const int right_bro_give = 1;
const int right_bro_merge = 2;

const int left_bro_give = -1;
const int left_bro_merge = -2;

const int right_bro = 1;
const int left_bro = -1;


class B_tree {
	int m;
	struct Node {
		struct Elem {
			int key;
			int ind;

			Elem(int kk, int ii) : key(kk), ind(ii) {}
			Elem() = default;
		};
		Elem* el;
		int n;
		Node ** child;
		Node * parent;

		Node(int m);
		bool is_leaf() const { return child[0] == nullptr; }

		int find(int) const;
		
		bool insert(Elem, Node*);
		bool remove(int);

		void destruct(int);
	};
	Node * root;

public:
	B_tree(int mm) : m(mm), root(nullptr) {}
	B_tree() = delete;
	B_tree(B_tree&& t) : m(t.m), root(t.root) { t.root = nullptr; }

	~B_tree();
	int height() const;
	int num_of_keys() const;

	bool find(int) const;
	bool insert(Node::Elem);
	bool insert(int);
	bool remove(int);
	
	friend ostream& operator << (ostream&, const B_tree&);


private:
	class Node_queue {
		Node * a[queue_size];
		int first = 0, last = first;

	public:
		bool is_empty() const { return first == last; }
		bool is_full() const { return (first == last + 1) || ((first == 0) && (last == queue_size - 1)); }
		bool insert(Node* x) {
			if (is_full()) { return false; }
			a[last] = x;
			if (last == queue_size - 1) { last = 0; }
			else { last++; }
			return true;
		}
		Node* remove() {
			if (is_empty()) { return nullptr; }
			Node * temp = a[first];
			if (first == queue_size - 1) { first = 0; }
			else { first++; }
			return temp;
		}
	};

	bool find_with_pointer(int, Node* &) const;

	Node* split_node(Node*, Node::Elem&, Node*);

	int check_bros(Node*, Node*&, int&);
	void give(Node*, Node*, int, int);
	void merge(Node*&, Node*&, int, int);
};



#endif
