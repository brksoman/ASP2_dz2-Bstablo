#ifndef __b_tree_strings_
#define __b_tree_strings_

#include <string>
#include <fstream>
using namespace std;

const int MAX_NODE_STACK_HEIGHT = 100;
const int queue_size = 100;

const int right_bro_give = 1;
const int right_bro_merge = 2;

const int left_bro_give = -1;
const int left_bro_merge = -2;

const bool increment = true;
const bool decrement = false;



class B_tree {
protected:
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
		bool remove_ind(int);

		void destruct(int);
	};

private:
	int m;
	Node * root;

public:
	B_tree(int mm = 3) : m(mm), root(nullptr) {}
	B_tree(B_tree&& t) : m(t.m), root(t.root) { t.root = nullptr; }

	~B_tree();
	int height() const;
	int num_of_keys() const;

	bool find(int) const;
	int find_ind(int) const;
	
	friend ostream& out(ostream&, const B_tree&);

	bool insert(Node::Elem);
	bool insert(int);
	void update(int, bool);
	bool remove(int);

protected:
	Node* succ(Node*, int);
	

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