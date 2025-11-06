#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
  // Add your code below
  bool equal = false;
  if(root == nullptr){
    equal = true;
  }
  else if(root->left == nullptr && root->right == nullptr){
    equal = true;
  }
  else if(root->left == nullptr){
    if(get_height(root->right)==1){
      equal = true;
    }
    else{
      equal = false;
    }
  }
  else if(root->right == nullptr){
    if(get_height(root->left)==1){
      equal = true;
    }
    else{
      equal = false;
    }
  }
  else if(get_height(root->left)-get_height(root->right)==0){
    equal = true;
  }
  else{
    equal = false;
  }
  return equal;
}

//helper funct
int get_height(Node * root){
  if(root == nullptr){
    return 0;
  }
  else{
    if(get_height(root->left)>get_height(root->right)){
      return 1+ get_height(root->left);
    }
    else{
      return 1+ get_height(root->right);
    }
  }  
}

