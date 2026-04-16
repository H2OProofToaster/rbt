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

  	if (parent->left == this) { return parent->right; }
  	else { return parent->left; }
  }

  bool isInnerChild() {

    return parent->left != this ^ parent->parent->left != parent;
  }

  Node* getNearNephew() {

    if (this->getSibling() != nullptr) {

      if (parent->left == this) { return getSibling()->left; }
      else { return this->getSibling()->right; }
    }

    return nullptr;
  }

  Node* getFarNephew() {

    if (this->getSibling() != nullptr) {

      if (parent->left == this) { return this->getSibling()->right; }
      else { return this->getSibling()->left; }
    }

    return nullptr;
  }

};

struct RBT {

  Node* head = nullptr;

  RBT() { /*cout << "Head: " << head << endl; */}

  //RECURSIVE DELETE
  ~RBT() { delete head; }

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

      //Root deletion
      if (this->head == remove) {

	      this->head = nullptr;
	      delete remove;
      }

      //Insert null node
      else {

	      Node* nullNode = new Node(true);
	      nullNode->parent = remove->parent;

	      if (remove->parent->left == remove) { remove->parent->left = nullNode; }
	      else { remove->parent->right = nullNode; }

        this->fixDeleteColor(remove, nullNode);

        //Detach null node
        if (nullNode->parent->left == nullNode) { nullNode->parent->left = nullptr; }
        else { nullNode->parent->right = nullptr; }

        delete nullNode;
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

      this->fixDeleteColor(remove, fix);
      delete remove;
    }
    
    //Two child
    else {

      Node* succ = this->succ(remove);

      remove->data = succ->data;

      //cout << "Before two child recursion: " << endl;
      //this->print();
      
      this->remove(succ);
    }
  }
  
  void fixInsertColor(Node* x) {

    //Empty tree
    if (head == x) { x->black = true; }

    //Black Parent
    else if (x->parent->black == true) {}

    //Red Parent and Uncle
    else if (x->parent->getSibling() != nullptr and
	     x->parent->getSibling()->black == false) {

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
    //[r]eplace is never nullptr

    //cout << "Delete: " << d->data << " Replace: " << r->data << endl;
    
    //Both red is not possible
    //(delete red) xor (replace red)
    if (d->black == false xor r->black == false) { r->black = true; }

    //Delete and replace are black
    //(delete black) and (replace black)
    else if (d->black == true and r->black == true) {

      r->doubleBlack = true;
      
      while (r->doubleBlack == true) {

	//Replace is root
	if (head == r) { r->doubleBlack = false; return; }
	
        //Save parent if replace is null
        Node* parent = r->parent;

        //Get r->sibling
        Node* sibling = nullptr;
        sibling = r->getSibling();
        
        //Red sibling of replace
        if (sibling != nullptr and sibling->black == false) {
          //Sibling has to exist if it is red, so no nullptr checks needed
          
          //Swap parent and sibling color
          parent->black = !parent->black;
          sibling->black = !sibling->black;

	  //Rotate towards r
	  if (parent->left == r) { leftRotate(parent); }
	  else { rightRotate(parent); }

	  //Get new sibling after rotation
	  sibling = r->getSibling();
	  
	  //Move on to back sibling case
	  //cout << "Red sibling after changing to black sibling" << endl;
	  //this->print();
        }

        //Black sibling of replace, and two nephews
        // (sibling exists and black) and ( (left nephew black) and (right nephew black) )
        if ( (sibling != nullptr and sibling->black == true) and
                  (sibling->left == nullptr or sibling->left->black == true) and
                  (sibling->right == nullptr or sibling->right->black == true) ) {
          //Sibling has to exist, so no nullptr checks
          
          sibling->black = false;

          //Red parent, end
          if (parent->black == false) {

	          parent->black = true;
	          r->doubleBlack = false;
	          return;
	        }

          //Black parent, make double black
          parent->doubleBlack = true;
          r->doubleBlack = false;
          r = parent;
	  //this->print();
	  continue; //I have no idea about this continue, but I wouldn't touch it
        }

        //Black sibling of replace, far nephew is black, and near nephew is red
        // (sibling exists and black) and (far nephew black) and (near nephew red)
        else if (sibling != nullptr and
                 sibling->black == true and 
                 (r->getFarNephew() == nullptr or r->getFarNephew()->black == true) and
                 r->getNearNephew() != nullptr and r->getNearNephew()->black == false) {

          sibling->black = !sibling->black;
          r->getNearNephew()->black = !r->getNearNephew()->black;

          if (parent->left == r) { rightRotate(sibling); }
          else { leftRotate(sibling); }

	        //Far red nephew case

          //Get again after rotation
          sibling = r->getSibling();

          sibling->black = parent->black;
          parent->black = true;
          r->getFarNephew()->black = true;

          if (parent->left == r) { leftRotate(parent); }
          else { rightRotate(parent); }

          r->doubleBlack = false;
        }
      
        //Black sibling of replace, far nephew red         *DOESN'T MATTER-------*
        // (sibling exists and black) and (far nephew red) and (near nephew black)
        else if (sibling != nullptr and
		             sibling->black == true and
		             r->getFarNephew() != nullptr and r->getFarNephew()->black == false) {

          sibling->black = parent->black;
          parent->black = true;
          r->getFarNephew()->black = true;

          if (parent->left == r) { leftRotate(parent); }
          else { rightRotate(parent); }

          r->doubleBlack = false;
        }

        //Log that no case happened
        else { cout << "No recursive case executed" << endl; }

        //Base case (null replace)
        break;
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
