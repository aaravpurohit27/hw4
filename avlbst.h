#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rLeft(AVLNode<Key,Value>* node);
    void rRight(AVLNode<Key, Value>* node);
    void findBalance(AVLNode<Key,Value>* node);


};

template<class Key, class Value>
void AVLTree<Key, Value>::findBalance(AVLNode<Key,Value>* node){
  if(node==NULL){
    return;
  }
  int lHeight = this->height(node->getLeft());
  int rHeight = this->height(node->getRight());
  node->setBalance(lHeight-rHeight);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rLeft(AVLNode<Key, Value>* node){
  if(node == NULL){
    return;
  }
  AVLNode<Key, Value>* right = node->getRight();
  if(right == NULL){
    return;
  }

  Node<Key, Value>* nprev = node->getParent();

  Node<Key,Value>* rleft = right->getLeft();
  node->setRight(rleft);
  if(rleft != NULL){
    rleft->setParent(node);
  }

  right->setLeft(node);
  node->setParent(right);

  right->setParent(nprev);
  if(nprev == NULL){
    this->root_ = right;
  }
  else{
    if(nprev->getLeft() == node){
      nprev -> setLeft(right);
    }  
    else{
      nprev->setRight(right);
    }
    
  }

  findBalance(node);
  findBalance(right);

}

template<class Key, class Value>
void AVLTree<Key, Value>::rRight(AVLNode<Key, Value>* node){
  if(node == NULL){
    return;
  }
  AVLNode<Key, Value>* left = node->getLeft();
  if(left == NULL){
    return;
  }

  Node<Key, Value>* nprev = node->getParent();

  Node<Key,Value>* lright = left->getRight();
  node->setLeft(lright);
  if(lright != NULL){
    lright -> setParent(node);
  }

  left->setRight(node);
  node->setParent(left);

  left->setParent(nprev);
  if(nprev == NULL){
    this->root_ = left;
  }
  else{
    if(nprev->getLeft()==node){
      nprev->setLeft(left);
    }
    else{
      nprev->setRight(left);
    }
  }

  findBalance(node);
  findBalance(left);

}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
  if(this->root_ == NULL){
    AVLNode<Key, Value>* nNode = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
    this->root_ = nNode;
    return;
  }
  Node<Key, Value>* walk = this->root_;
  Node<Key, Value>* prev = NULL;
  while(walk != NULL){
    prev = walk;
    if(new_item.first == walk->getKey()){
      //check if key already exists and rewrite value
      walk->setValue(new_item.second);
      return;
    }
    //go left if less
    else if(new_item.first < walk->getKey()){
      walk = walk->getLeft();
    }
    //go right if greater
    else{
      walk = walk->getRight();
    }
  }

  //new AVLNode
  AVLNode<Key, Value>* nNode = new AVLNode<Key, Value>(new_item.first, new_item.second, static_cast<AVLNode<Key, Value>*>(prev));
  //figure out if it goes on left or right
  if(new_item.first < prev->getKey()){
    prev->setLeft(nNode);
  }
  else{
    prev->setRight(nNode);
  }

  //balance
  AVLNode<Key, Value>* node = nNode->getParent();
  while(node != NULL){
    findBalance(node);
    int8_t b = node->getBalance();
    if(b==0){
      break;
    }
    else if(b == 1){
      nNode = node;
      node = node->getParent();
    }
    else if(b==-1){
      nNode = node;
      node = node->getParent();
    }
    else if(b == 2){
      //left is bigger
      AVLNode<Key, Value>* l = node->getLeft();
      if(l->getBalance() >= 0){
        rRight(node);
      }
      else{
        rLeft(l);
        rRight(node);
      }
      break;
    }
    else if(b==-2){
      //right is bigger
      AVLNode<Key, Value>* r = node->getRight();
      if(r->getBalance() <= 0){
        rLeft(node);
      }
      else{
        rRight(r);
        rLeft(node);
      }
      break;
    }
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* target = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(target == NULL){
      return;
    }

    if(target->getLeft() != NULL && target->getRight() != NULL){
      Node<Key, Value>* p = BinarySearchTree<Key,Value>::predecessor(target);
      AVLNode<Key,Value>* tarprev = static_cast<AVLNode<Key, Value>*>(p);
      nodeSwap(target, tarprev);
    }

    AVLNode<Key, Value>* next = NULL;
    if(target->getLeft() != NULL){
      next = target->getLeft();
    }
    else if(target->getRight() != NULL){
      next = target->getRight();
    }

    AVLNode<Key, Value>* prev = target->getParent();

    if(next != NULL){
      next->setParent(prev);
    }


    if(prev == NULL){
      this->root_ = next;
    }
    else if(prev->getLeft() == target){
      prev->setLeft(next);
    }
    else{
      prev->setRight(next);
    }
    delete target;

    AVLNode<Key, Value>* node =prev;

    while(node != NULL){
      int lHeight = this->height(node->getLeft());
      int rHeight = this->height(node->getRight());
      node->setBalance(lHeight - rHeight);

      if(node->getBalance()==2){
        AVLNode<Key, Value>* l = node->getLeft();
        int leftb = this->height(l->getLeft())-this->height(l->getRight());
        if(leftb >= 0){
          rRight(node);
        }
        else{
          rLeft(l);
          rRight(node);
        }
      }
      else if(node->getBalance() == -2){
        AVLNode<Key, Value>* r = node->getRight();
        int rightb = this->height(r->getLeft())- this->height(r->getRight());
        if(rightb <= 0){
          rLeft(node);
        }
        else{
          rRight(r);
          rLeft(node);
        }
      }
      node = node->getParent();
    }


}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
