#ifndef STRUCTS_CPP
#define STRUCTS_CPP

#include <iostream>

using namespace std;

struct Node {

  int data;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;
  bool black = false;
  bool doubleBlack =  false;
  
  Node() {}
  Node(int d) : data(d) {}
  Node(bool b) : black(b) {}

  ~Node() { delete left; delete right; }
};

struct RBT {

  Node* head = nullptr;
	Node* nullNode = new Node(true);

  RBT() { /*cout << "Head: " << head << endl; */}

  //RECURSIVE DELETE
  ~RBT() { delete head; delete nullNode; }

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

    return t;
  }
  
  void leftRotate(Node* x) {

    Node* y = x->right;

    x->right = y->left;
    if (x->right != nullptr) { x->right->parent = x; }

    y->parent = x->parent;
    if (y->parent != nullptr) {

      if (x->parent->left == x) { x->parent->left = y; }
      else { x->parent->right = y; }
    }
    else { head = y; }

    x->parent = y;
    y->left = x;
  }

  void rightRotate(Node* x) {

    Node* y = x->left;

    x->left = y->right;
    if (x->left != nullptr) { x->left->parent = x; }

    y->parent = x->parent;
    if (y->parent != nullptr) {

      if (x->parent->left == x) { x->parent->left = y; }
      else { x->parent->right = y; }
    }
    else { head = y; }

    x->parent = y;
    y->right = x;
  }

  Node* getSibling(Node* x) {

  	if (x->parent->left == x) {
  		return x->parent->right == nullptr ? nullNode : x->parent->right;
  	}
  	else {
  		return x->parent->left == nullptr ? nullNode : x->parent->left;
  	}
  }

  bool isInnerChild(Node* x) {

    return x->parent->left != x ^ x->parent->parent->left != x->parent;
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

    this->fixInsertColor(z);
  }

  void remove(int v) {

    this->remove(this->search(v));
  }
  
  void remove(Node* remove, bool twoChild = false) {

    if (remove == nullptr) { return; }
    
    //Leaf
    if (remove->left == nullptr && remove->right == nullptr) {

      //Fix parent (if it exists)
      if (remove->parent != nullptr) {
	if (remove->parent->left == remove) { remove->parent->left = nullptr; }
	else { remove->parent->right = nullptr; }
      }
      
      //Root deletion
      else { this->head = nullptr; }

      if (!twoChild) {

	this->fixDeleteColor(remove, nullptr);
	delete remove;
      }
    }
    
    //One child
    else if (remove->left == nullptr ^ remove->right == nullptr) {

      //Get correct child
      Node* fix = nullptr;
      if (remove->left != nullptr) { fix = remove->left; remove->left = nullptr; }
      else { fix = remove->right; remove->right = nullptr;}
      fix->parent = remove->parent;

      //Fix parent (if it exists)
      if (remove->parent != nullptr) {
	if (remove->parent->left == remove) { remove->parent->left = fix; }
	else { remove->parent->right = fix; }
      }
      
      //Root deletion
      else { this->head = fix; }

      if (!twoChild) {

	this->fixDeleteColor(remove, fix);
	delete remove;
      }
    }
    
    //Two child
    else {

      Node* succ = this->succ(remove);
      
      remove->data = succ->data;
      this->remove(succ, true);
      this->fixDeleteColor(remove, succ);
    }
  }
  
  void fixInsertColor(Node* x) {

    //Empty tree
    if (head == x) { x->black = true; }

    //Black Parent
    else if (x->parent->black == true) {}

    //Red Parent and Uncle
    else if (getSibling(x->parent)->black == false) {

      x->parent->black = true;
      getSibling(x->parent)->black = true;

      x->parent->parent->black = false;

      fixInsertColor(x->parent->parent);
    }

    //Red Parent, Black/Null Uncle
    else {

      if (isInnerChild(x) == true) {
	if (x->parent->left == x) { rightRotate(x->parent); x = x->right;}
	else { leftRotate(x->parent); x = x->left; }
      }

      if (x->parent->left == x) { rightRotate(x->parent->parent); }
      else { leftRotate(x->parent->parent); }

      x->parent->black = !x->parent->black;
      if (getSibling(x) != nullptr) { getSibling(x)->black = !getSibling(x)->black; }

      head->black = true;
    }
  }

  Node* getRedChild(Node* x) {

    if (x->right == nullptr and x->left == nullptr) { //No children

      return nullptr;
    }
    
    else if (x->right == nullptr ^ x->left == nullptr) { //One child

      if (x->right != nullptr and x->right->black == false) { return x->right; } //Right exists
      else { return x->left; } //Left exists
    }
    
    else { //Two children

      if (x->parent->right == x) { //x is right child, perfer right child

	if (x->right->black == false) { return x->right; }
	else if (x->left->black == false) { return x->left; }
      }
      else { //x is left child, perfer left child

	if (x->left->black == false) { return x->left; }
	else if (x->right->black == false) { return x->right; }
      }
    }

    return nullptr;
  }
  
  void fixDeleteColor(Node* v, Node* u) {
    
    if (v->black ^ (u == nullptr or u->black)) {

	if (u != nullptr) { u->black = true; }
    }
      
    else if (v->black and (u == nullptr or u->black)) {

      u->doubleBlack = true;

      if (head == u) { u->doubleBlack = false; return; }
      
      while (u->doubleBlack == true and this->head != u) {

	Node* s = this->getSibling(u);
	Node* r = this->getRedChild(s); //Favors outside child
	
	//Black sibling and at least one red nephew
	if (s->black == true and r != nullptr) {

	  if (isInnerChild(r) == true) {
	    if (s->parent->left == s) { rightRotate(s); }
	    else { leftRotate(s); }
	  }

	  if (s->parent->left == s) { rightRotate(s->parent); }
	  else { leftRotate(s->parent); }
	}

	//Black sibling and black nephews
	else if (s->black == true and r == nullptr) {

	  s->black = false;
	  u->doubleBlack = false;

	  if (s->parent->black == true) { s->parent->doubleBlack = true; }
	  else { s->parent->black = true; }

	  u = s->parent;
	}

	//Red sibling
	else if (s->black == false) {

	  if (s->parent->left == s) { rightRotate(s->parent); }
	  else { leftRotate(s->parent); }

	  s->parent->black = !s->parent->black;
	  s->parent->parent->black = !s->parent->parent->black;
	}
      }
    }
  }
    
  void print(int indent = 0, Node* i = nullptr) {

    if (this->head == nullptr) { return; } //Empty tree
    
    if (i == nullptr) { i = head; } //Initial case
    
    if (i->right != nullptr) { this->print(indent + 1, i->right); } //print right subtree

    for (int j = 0; j < indent; j++) { cout << "\t"; } //tab in

    if (i->black == true) { cout << "(B)"; } //print color
    else { cout << "(R)"; }
    
    if (i->parent != nullptr) { //print parent num

      if (i->parent->data < 10) { cout << "(00"; }
      else if (i->parent->data < 100) { cout << "(0"; }
      else { cout << "("; }
      cout << i->parent->data << ")";
    }
    
    if (i->data < 10) { cout << "00"; }
    else if (i->data < 100) { cout << "0"; }
    cout << i->data << endl; //print num

    if (i->left != nullptr) { this->print(indent + 1, i->left); } //left subtree
  }
};

#endif
