#ifndef STRUCTS_CPP
#define STRUCTS_CPP

#include <iostream>

using namespace std;

struct Node {

  int data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  Node() {}
  Node(int d) : data(d) {}

  ~Node() { delete left; delete right; }
};

struct BST {

  Node* head = nullptr;

  BST() { /*cout << "Head: " << head << endl; */}
  //BST(BST* i) { this->insert(i); }

  //RECURSIVE DELETE
  ~BST() { delete head; }

  //Min, succ, and search ripped off from wikipedia
  Node* min(Node* v) {

    while (v->left != nullptr) { v = v->left; }
    return v;
  }
  
  Node* succ(Node* v) {

    v = v->right;

    while (v->left != nullptr) { v = v->left; }

    return v;
  }
  
  Node* search(int v) {

    Node* t = head;
    while (t != nullptr and t->data != v) {

      if (v < t->data) { t = t->left; }
      else { t = t->right; }
    }

    if (t == nullptr) { return new Node(-1); }
    return t;
  }
  
  void insert(int v) {

    Node* z = new Node(v);
    Node* y = nullptr;
    Node* x = this->head;

    while (x != nullptr) {

      y = x;
      if (v < x->data) { x = x->left; }
      else { x = x->right; }
    }

    z->parent = y;

    if (y == nullptr) { head = z; }
    else if (v < y->data) { y->left = z; }
    else { y->right = z; }
  }
  
  void remove(int v) {

    Node* remove = this->search(v);
    if (remove->data == -1) { delete remove; return; }
    
    //Leaf
    if (remove->left == nullptr && remove->right == nullptr) {

      //Fix parents
      if (remove->parent != nullptr) {

	//Left child
	if (remove->parent->left == remove) { remove->parent->left = nullptr; }
	//Right child
	else { remove->parent->right = nullptr; }
      }
      //Root deletion
      else { this->head = nullptr; }

      delete remove;
    }
    //One child
    else if (remove->left == nullptr ^ remove->right == nullptr) {

      //Get correct child
      Node* fix = nullptr;

      //Left
      if (remove->left != nullptr) { fix = remove->left; remove->left = nullptr; }
      //Right
      else { fix = remove->right; remove->right = nullptr;}

      fix->parent = remove->parent;

      //Fix parents
      if (remove->parent != nullptr) {

	if (remove->parent->left == remove) { remove->parent->left = fix; }
	else { remove->parent->right = fix; }
      }
      //Root deletion
      else { this->head = fix; }

      delete remove;
    }
    //Two child
    else {

      Node* succ = this->succ(remove);

      //Right child
      if (remove->right == succ) {

	succ->parent = remove->parent;

	//Fix parents
	if (remove->parent != nullptr) {

	  if (remove->parent->left == remove) { remove->parent->left = succ; }
	  else { remove->parent->right = succ; }
	}
	//Root deletion
	else { this->head = succ; }

	succ->left = remove->left;
	succ->left->parent = succ;
	
	remove->left = nullptr;
	remove->right = nullptr;

	delete remove;
      }
      //Right subtree
      else {

	//Succ replace

	//Succ has subtree
	if (succ->right != nullptr) {
	  
	  succ->right->parent = succ->parent;

	  //Fix parents
	  if (succ->parent->left == succ) { succ->parent->left = succ->right; }
	  else { succ->parent->right = succ->right; }
	}
	//No subtree
	else {

	  if (succ->parent->left == succ) { succ->parent->left = nullptr; }
	  else { succ->parent->right = nullptr; }
	}
	
	//Succ displace

	succ->left = remove->left;
	succ->left->parent = succ;
	succ->right = remove->right;
	succ->right->parent = succ;
	succ->parent = remove->parent;
	
	//Fix parents
	if (remove->parent != nullptr) {

	  if (remove->parent->left == remove) { remove->parent->left = succ; }
	  else { remove->parent->right = succ; }
	}
	//Root deletion
	else { this->head = succ; }

	remove->left = nullptr;
	remove->right = nullptr;

	delete remove;
      }
    }
  }
    
  void print(int indent = 0, Node* i = nullptr) {

    if (this->head == nullptr) { return; }
    
    if (i == nullptr) { i = head; }
    
    if (i->right != nullptr) { this->print(indent + 1, i->right); }

    for (int j = 0; j < indent; j++) { cout << "\t"; }

    if (i->parent != nullptr) {

      cout << "(";
      if (i->parent->data < 100) { cout << "0"; }
      if (i->parent->data < 10) { cout << "00"; }
      cout << i->parent->data;
      cout << ")";
    }
    if (i->data < 100) { cout << "0"; }
    else if (i->data < 10) { cout << "00"; }
    cout << i->data << endl;

    if (i->left != nullptr) { this->print(indent + 1, i->left); }
  }
};

#endif
