//Most parts of this exercise are taken over from my last years submission of the self assessment!

#include "bstree.h"

struct bstree{
    struct bstree *left, *right, *root;
    int32_t size;
    value_t value;
};

value_t bstree_get_value(bstree* t){
  return t->value;
}

bstree* bstree_create(){
    bstree* t = (bstree*) malloc(sizeof(*t));
    t->root = t;
    t->size = 0;
    t->left = NULL;
    t->right = NULL;
    t->value = -1; // empty tree -> data = -1
    return t; 
}

//destroys all children and itself 
void bstree_destroy(bstree* t){
  if(t!= NULL){
      bstree_destroy(t->left);
	    bstree_destroy(t->right);
  }
  free(t);
  t = NULL;
}

//auxilary function that creates the new nodes for bstree_insert
bstree* bstree_insert_auxilary(bstree* parent, value_t  d){
  if( parent->value == -1){ //check if tree is empty
    parent->value = d;
	parent->root->size = parent->root->size +1;

	return NULL;
  }
  else{
  bstree* child = bstree_create();
  child->value = d;
  child->left = NULL;
  child->right = NULL;
  if(parent == NULL){
	  child->root = child;
	  child->size = 1;
  } 
  else{
	  child->root = parent->root;
	  parent->root->size = parent->root->size +1;

  }
  
  return child;
  }
}

void bstree_insert(bstree* t, value_t v){
  if ( t == NULL){
    bstree_insert_auxilary(NULL, v);
    }
  if(t->value == -1){
    bstree_insert_auxilary(t,v);
  }
	  
  //if t is not null or empty recursively call their children until a node is free
  else{ 
	if ( v < t->value){
		if ( t->left == NULL){
			t->left = bstree_insert_auxilary(t, v);
		}
		else{
			bstree_insert(t->left, v);
		}
	}
	else if (v > t->value){
	    if ( t->right == NULL){
			t->right = bstree_insert_auxilary(t, v);
		}
		else{
			bstree_insert(t->right, v);
		}
	}
  }	  

}


void bstree_deleteNodeWithValue(bstree* t, value_t d){
  if(bstree_contains(t,d)){
    if(t->value ==d){
      t->root->size = t->root->size - 1;
      bstree_destroy(t);
    }
    else if( t->value > d){
      bstree_deleteNodeWithValue(t->left,d);
    }
    else{
      bstree_deleteNodeWithValue(t->right,d);
    }
  }
}

//doesn't work properly, "segmentation fault" after using this function
void bstree_remove(bstree* t, value_t v){
  if(bstree_contains(t,v)){
    if(t->value == v){
      if( t->left == NULL && t->right == NULL){
        bstree_destroy(t);
      }
      else if(t->left == NULL && t->right != NULL){
        t->value = bstree_get_value(t->right);
        t->right = NULL;
        bstree_destroy(t->right);
      }
      else if(t->left != NULL && t->right == NULL){
        t->value = bstree_get_value(t->left);
        t->left = NULL;
        bstree_destroy(t->left);
      }
      else{
        value_t maximum = bstree_maximum(t->left);
        t->value = maximum;
        bstree_deleteNodeWithValue(t,maximum);
      }
    }
    if(t->value > v){
      bstree_remove(t->left,v);
    }
    if(t->value < v){
      bstree_remove(t->right,v);
    }
  }

}

bool bstree_contains(const bstree* t, value_t v){
    if ( t != NULL){
        if (t->value == v){
            return true;
        } else if (v < t->value){
            return bstree_contains(t->left, v);
        } else {
            return bstree_contains(t->right,v);
        }
    } else return false;
}

value_t bstree_minimum(const bstree* t){
    if ( t->left != NULL){
	    return bstree_minimum(t->left);
	}
	else {
		return t->value;
	}
}

value_t bstree_maximum(const bstree* t){
    if ( t->right != NULL){
	    return bstree_minimum(t->right);
	}
	else {
		return t->value;
	}
}


// auxilary function for tree depth with an additional parameter to save the depth in the recursive call.
int32_t bstree_depth_auxilary(const bstree* t, value_t v, int32_t depth){
    if (t->value == v){
        return depth;
    }
    if (!bstree_contains(t, v)){
        return -1;
    }
    else{
        if(bstree_contains(t->left, v)){
            return bstree_depth_auxilary(t->left,v,depth+1);
        }
        else{
            return bstree_depth_auxilary(t->right,v,depth+1);
        }
    }
}

int32_t bstree_depth(const bstree* t, value_t v){
    if (t->value == v){
      return 0;
    }
    if (!bstree_contains(t, v)){
        return -1;
    }
    else{
        return bstree_depth_auxilary(t,v,0);
    }
}

int32_t bstree_size(const bstree* t){
    return t->root->size;
}

//auxilary function for bstree_print
static void bstree_print_auxilary(bstree* n, FILE* out){
  fprintf(out, "[");
  if(n->left){
    bstree_print_auxilary(n->left, out);
    fprintf(out, ", ");
  }
  fprintf(out, VALUE_T_FORMAT, n->value);
  if(n->right){
    fprintf(out, ", ");
    bstree_print_auxilary(n->right, out);
  }
  fprintf(out, "]");
}

 //* printf("Value: " VALUE_T_FORMAT "\n", my_value);
void bstree_print(const bstree* t, FILE* out){
  if(t->value == -1){
        fprintf(out, "[ NIL ] : %d\n", t->size);
  } else {
    fprintf(out, "[");
    if((t->root)->left){
      bstree_print_auxilary((t->root)->left, out);
      fprintf(out, ", ");
    }
    fprintf(out, VALUE_T_FORMAT, (t->root)->value);
    if((t->root)->right){
      fprintf(out, ", ");
      bstree_print_auxilary((t->root)->right, out);
    }
    fprintf(out, "] : %d\n", t->size);
  }
}