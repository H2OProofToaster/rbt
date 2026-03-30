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

  Node* getSibling() {

  	if (parent->left == this) {
  		return parent->right == nullptr ? nullNode : parent->right;
  	}
  	else {
  		return parent->left == nullptr ? nullNode : parent->left;
  	}
  }

  bool isInnerChild() {

    return parent->left != this ^ parent->parent->left != parent;
  }

  Node* getNearNephew() {

    if (this->getSibling() != nullptr) {

      if (parent->left == this) { return getSibling()->left; }
      else { return this->getSibling()->right; }
    }
  }

  Node* getFarNephew() {

    if (this->getSibling() != nullptr) {

      if (parent->left == this) { return this->getSibling()->right; }
      else { return this->getSibling()->left; }
    }
  }

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
  
  void remove(Node* remove) {

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

      this->fixDeleteColor(remove, nullptr);
      delete remove;
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

      this->fixDeleteColor(remove, fix);
      delete remove;
    }
    
    //Two child
    else {

      Node* succ = this->succ(remove);

      remove->data = succ->data;
      this->remove(succ);
    }
  }
  
  void fixInsertColor(Node* x) {

    //Empty tree
    if (head == x) { x->black = true; }

    //Black Parent
    else if (x->parent->black == true) {}

    //Red Parent and Uncle
    else if (x->parent->getSibling()->black == false) {

      x->parent->black = true;
      x->parent->getSibling()->black = true;

      x->parent->parent->black = false;

      fixInsertColor(x->parent->parent);
    }

    //Red Parent, Black/Null Uncle
    else {

      if (x->isInnerChild() == true) {
        if (x->parent->left == x) { rightRotate(x->parent); x = x->right;}
        else { leftRotate(x->parent); x = x->left; }
      }

      if (x->parent->left == x) { rightRotate(x->parent->parent); }
      else { leftRotate(x->parent->parent); }

      x->parent->black = !x->parent->black;
      if (x->getSibling() != nullptr) { x->getSibling()->black = !x->getSibling()->black; }

      head->black = true;
    }
  }

  void fixDeleteColor(Node* d, Node* r) {
    //[d]elete
    //[r]eplace
    
    if (d->black ^ (r == nullptr or r->black)) {

	    if (d != nullptr) { r->black = true; }
    }
      
    else if (d->black and (r == nullptr or r->black)) {

      r->doubleBlack = true;

      if (head == r) { r->doubleBlack = false; return; }
      
      while (r->doubleBlack == true) {

        //Red sibling of replace
        if (r->getSibling() != nullptr or r->getSibling()->black == false) {
          //Sibling has to exist if its red, so no nullptr checks needed
          
          //Swap parent and sibling color
          r->parent->black = !s->parent->black;
          r->getSibling()->black = !r->getSibling->black;

          //Rotate towards replace
          if (r->parent->left == r) { rotateLeft(r->parent); }
          { rotateRight(r->parent); }
        }

        //Black sibling of replace, and two nephews
        // (sibling exists and black) and ( (left nephew black) and (right nephew black) )
        if ( (r->getSibling() != nullptr and r->getSibling()->black == true) and
            ( (r->getSibling()->left == nullptr or r->getSibling()->left->black == true) and
              (r->getSibling()->right == nullptr or r->getSibling()->right->black == true) ) ) {
          //Sibling has to exist, so no nullptr checks
          
          r->getSibling()->black = false;

          //Red parent, end
          if (r->parent->black == false) { r->parent->black = true; }
          //Black parent, make double black
          else {
            
            r->parent->doubleBlack = true;
            r->doubleBlack = false;
            r = r->parent;
          }
        }

        //Black sibling of replace, far nephew is black, and near nephew is red
        // (sibling exists and black) and (far nephew black) and (near nephew black)
        if (r->getSibling() != nullptr and 
                 r->getSibling()->black == true and 
                 ( r->getFarNephew() == nullptr or r->getFarNephew()->black == true ) and 
                 ( r->getNearNephew() != nullptr and r->getNearNephew()->black == false) ) {

          r->getSibling()->black = !r->getSibling()->black;
          r->getNearNephew()->black = !r->getNearNephew()->black;

          if (r->parent->left == r and r->getNearNephew()->parent->left == r->getNearNephew()) { rightRotate(); }
          else { leftRotate(); }
        }
      
        //Black sibling of replace, both nephews are black
        // (sibling exists and black) and (far nephew black) and (near nephew black)
        if (r->getSibling() != nullptr and
            r->getSibling()->black == true and
            ( r->getFarNephew() == nullptr or r->getFarNephew()->black == true ) and 
            ( r->getNearNephew() == nullptr or r->getNearNephew()->black == true) ) {

          
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
