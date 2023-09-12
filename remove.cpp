#ifndef __PROJ_FOUR_LEVEL_BALANCED_TREE_HPP
#define __PROJ_FOUR_LEVEL_BALANCED_TREE_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>

#include <iostream>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};

template<typename Key, typename Value>
class Node {
  public:
    Key k;
    Value v;
    Node<Key, Value> *right;
    Node<Key, Value> *left;
	int height;
    bool null;
	Node<Key, Value> *parent;

    Node(Key key, Value value) {
        k = key;
        v = value;
        right = nullptr;
        left = nullptr;
		parent = nullptr;
        null = false;
    }
    Node() {
        right = nullptr;
        left = nullptr;
		parent = nullptr;
        null = true;
    }
};



template<typename Key, typename Value>
class LevelBalancedTree
{
private:

	//Node<Key, Value> *head;
	int node_amount;

public:
	Node<Key, Value> *head;
	LevelBalancedTree();

	// In general, a copy constructor and assignment operator
	// are good things to have.
	// For this quarter, I am not requiring these. 
	//	LevelBalancedTree(const LevelBalancedTree & st);
	//	LevelBalancedTree & operator=(const LevelBalancedTree & st);


	// The destructor is required. 
	//~LevelBalancedTree();

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	// contains() returns true if and only if there
	//  is a (key, value) pair in the tree
	//	that has the given key as its key.
	bool contains(const Key & k) const noexcept; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const LevelBalancedTrees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the balancing operation(s) if needed
	// operation, as described in lecture. 
	// If the key already exists in the tree, 
	// you may do as you please (no test cases in
	// the grading script will deal with this situation)
	void insert(const Key & k, const Value & v);


	// Deletes the given key from the tree
	// and performs the balancing operation(s) if needed.
	// If the key does not exist in the tree, 
	// do not modify the tree.
	void remove(const Key &k);


	// The following three functions all return
	// the set of keys in the tree as a standard vector.
	// Each returns them in a different order.
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;

	void preorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const ;
	void inorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const ;
	void postorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const;

	void recursiveDelete(Node<Key, Value>* n);

	Node<Key, Value>* insert_helper(Node<Key, Value>* copy, const Key k, const Value v);
	Node<Key, Value>* remove_helper(Node<Key, Value>* copy, Key k);

	int count();

	void printBT(Node<Key,Value>* node);
    void printBT1(const std::string& prefix, const Node<Key,Value>* node, bool isLeft);

	int heightDif(Node<Key,Value> *parent, Node<Key,Value> *node);
	void balanceHelper(Node<Key,Value> *parent, Node<Key,Value> *node);
	bool isShape(int nodeLeft, int leftVal, int nodeRight, int rightVal);


	Node<Key,Value>* findSuccessor(Node<Key,Value>* copy);
	Node<Key,Value>* findPredecessor(Node<Key,Value>* copy);
	Node<Key, Value>* removeSpotFinder(Node<Key,Value>* copy, const Key & key);
	void removeBalanceHelper(Node<Key,Value> *curr, int leftLevel, int rightLevel);
};




/*
template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::recursiveDelete(Node<Key, Value>* n) {
	if (n != nullptr) {
		recursiveDelete(n->left);
		recursiveDelete(n->right);
		delete n;
	}
}
*/
template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::LevelBalancedTree()
{
	head = new Node<Key,Value>();
	node_amount = 0;

}
/*
template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::~LevelBalancedTree()
{
	recursiveDelete(head);
}
*/
template<typename Key, typename Value>
size_t LevelBalancedTree<Key, Value>::size() const noexcept
{
	return node_amount;
}

template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::isEmpty() const noexcept
{
	if (head->null == true) {
		return true;
	}
	return false;
}


template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::contains(const Key &k) const noexcept
{
	Node<Key,Value>* copy = head;
	while (copy != nullptr) {
		if (copy->k == k) {
			return true;
		}
		if (copy->k < k) {
			copy = copy->right;
		} else {
			copy = copy->left;
		}
	}

	return false;
}

template<typename Key, typename Value>
Value & LevelBalancedTree<Key, Value>::find(const Key & k)
{
	Node<Key,Value>* copy = head;
	while (copy != nullptr) {
		if (copy->k == k) {
			return copy->v;
		}
		if (copy->k < k) {
			copy = copy->right;
		} else {
			copy = copy->left;
		}
	}

	throw ElementNotFoundException("not found");
}

template<typename Key, typename Value>
const Value & LevelBalancedTree<Key, Value>::find(const Key & k) const
{
	Node<Key,Value>* copy = head;
	while (copy != nullptr) {
		if (copy->k == k) {
			return copy->v;
		}
		if (copy->k < k) {
			copy = copy->right;
		} else {
			copy = copy->left;
		}
	}

	throw ElementNotFoundException("not found");
}

template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::isShape(int nodeL, int v1, int nodeR, int v2){
	if (nodeL == v1 && nodeR == v2) {
		return true;
	} else {
		return false;
	}
}


template<typename Key, typename Value>
Node<Key, Value>* LevelBalancedTree<Key, Value>::insert_helper(Node<Key, Value>* copy, const Key k, const Value v) {
	// if (copy == nullptr) {
	// 	Node<Key, Value>* new_node = new Node<Key, Value>(k,v);
	// 	new_node->height = 1;
	// 	return new_node;
	// } else {
	// 	if (copy->k < k) {
	// 		copy->right = insert_helper(copy->right, k, v);
	// 		copy->right->parent = copy;
	// 		return copy->right;
	// 	} else {
	// 		copy->left = insert_helper(copy->left, k, v);
	// 		copy->left->parent = copy;
	// 		return copy->left;
	// 	}
	// }
	// return copy;
	// if (copy == nullptr){
    //     copy = new Node<Key,Value>(k,v);
	// 	copy->height = 1;
    //     this->head = copy;
    //     return copy;
    // }

    if (k > copy->k){
        if (copy->right != nullptr){
			copy = insert_helper(copy->right,k,v);
        } else {
			copy->right = new Node<Key,Value>(k,v);
			copy->right->height = 1;
			copy->right->parent = copy;
            return copy->right;
        }
    } else {
        if (copy->left != nullptr){
			copy = insert_helper(copy->left,k,v);
        } else {
			copy->left = new Node<Key,Value>(k,v);
			copy->left->height = 1;
			copy->left->parent = copy;
            return copy->left;
        }
    }
    return copy;
}

template<typename Key, typename Value>
int LevelBalancedTree<Key, Value>::heightDif(Node<Key,Value> *parent, Node<Key,Value> *node) {
	if (node == nullptr) {
		return parent->height;
	} else {
		return parent->height - node->height;
	}
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::balanceHelper(Node<Key,Value> *parent, Node<Key,Value> *node) {

	// int nodeShapeL = heightDif(node, node->left);
    // int nodeShapeR = heightDif(node, node->right);
    // int parentShapeL= heightDif(parent,parent->left);
    // int parentShapeR = heightDif(parent,parent->right);

	bool node_1_1 = isShape(heightDif(node, node->left), 1,heightDif(node, node->right),1);
	bool node_1_2 = isShape(heightDif(node, node->left),1,heightDif(node, node->right),2 );
	bool node_2_1 = isShape(heightDif(node, node->left), 2, heightDif(node, node->right),1);
	bool parent_0_1 = isShape(heightDif(parent,parent->left),0,heightDif(parent,parent->right),1);
	bool parent_1_0 = isShape(heightDif(parent,parent->left),1,heightDif(parent,parent->right),0);
	bool parent_2_0 = isShape(heightDif(parent,parent->left),2,heightDif(parent,parent->right),0);
	bool parent_0_2 = isShape(heightDif(parent,parent->left),0,heightDif(parent,parent->right),2);

	if ((node_1_1 && parent_0_1) || (node_1_1 && parent_1_0)|| (node_2_1 && parent_0_1) || (node_1_2 && parent_0_1) || (parent_1_0 && node_1_2) || (parent_1_0 && node_2_1)){
        parent->height++;
		return;
    }

	if ((node_1_2 && parent_0_2) && (parent->left == node)){
        std::cout << "1" << std::endl;
        parent->height--; 
        node->parent = parent->parent; 
        if (parent->parent != nullptr){
            if (parent->parent->left == parent) {
                parent->parent->left = node;
            } else { 
				parent->parent->right = node;
			}
        } else{
            this->head = node; 
        }

        
        if (node->right != nullptr){
            node->right->parent = parent;
            parent->left = node->right;
        } else{
            node->right = parent; 
            parent->parent = node;
            parent->left = nullptr;
        }
        node->right = parent;
        parent->parent = node;
    }
	else if ((node_2_1 && parent_2_0) && (parent->right == node)){  
        std::cout << "2" << std::endl;
        parent->height--; 
        node->parent = parent->parent;

        if (parent->parent != nullptr){
            if (parent->parent->left == parent) {
                parent->parent->left = node;
            } else { 
				parent->parent->right = node;
			}
        } else{
            this->head = node;
        }
        
        if (node->left != nullptr) {
            node->left->parent = parent;
            parent->right = node->left;
        } else{
            node->left = parent;
            parent->parent = node;
            parent->right = nullptr;
        }
        node->left = parent;
        parent->parent = node;
    }

	else if ((node_2_1 && parent_0_2)) { 


        std::cout << "3" << std::endl;
        node->height--;
        node->parent->height--;
        node->right->height++;

        Node<Key,Value> *old_parent = node->parent; 
        Node<Key,Value> *childs_new_child =  node->right->left;

        node->right->parent = parent->parent;
        if(parent->parent != nullptr) {
            if (parent->parent->left == parent) {
                parent->parent->left = node->right;
            } else {
                parent->parent->right = node->right; 
            }

        } else {
            this->head = node->right; 
        }
        node->parent = node->right;
        node->parent->left = node;

        if (childs_new_child != nullptr){
            node->right = childs_new_child;
            childs_new_child->parent = node;
		} else {
            node->right = nullptr;
        }
        node->parent->right = old_parent;
        old_parent->parent = node->parent;
        old_parent->left = nullptr;
    }
	
	else if ((node_1_2 && parent_2_0)){ 
        std::cout << "4 " << node->parent->k << " " << node->k << std::endl;
        node->height--;
        node->parent->height--;
        std::cout<< node->left->k <<  "__"<<std::endl;
        node->left->height++;

        Node<Key,Value> *old_parent = node->parent; 
        Node<Key,Value> *childs_new_child =  node->left->right; 

        node->left->parent = parent->parent;
        if(parent->parent != nullptr){
            if (parent->parent->left == parent){
                parent->parent->left = node->left; 
            } else {
                parent->parent->right = node->left; 
            }

        } else {
            this->head = node->left;
        }
        node->parent = node->left;
        std::cout << node->parent->k << " " << node->k << std::endl;
        node->parent->right = node;

        if (childs_new_child != nullptr) {
            node->left = childs_new_child;
            childs_new_child->parent = node;
		} else {
            node->left = nullptr;
        }
        node->parent->left = old_parent;
        old_parent->parent = node->parent;
        old_parent->right = nullptr;
        //std::cout << old_parent->right->k << std::endl;
    }

}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::insert(const Key & k, const Value & v)
{
    if (contains(k)) {
        return;
    }
    std::cout << "CURRENTLY ON : " << k << std::endl;
	node_amount++;
	if (head->null == true) {
		head->k = k;
		head->v = v;
		head->height = 1;
		head->null = false;
		return;
	}
	Node<Key, Value>* copy = insert_helper(head, k, v);
	while (copy != head) {
		if (copy->height == copy->parent->height) {
			balanceHelper(copy->parent, copy);
		}
		if (copy != head) {
			copy = copy->parent;
		}
	}
	//std::cout << std::endl << std::endl << std::endl;
    //printBT(head);
}















template<typename Key, typename Value>
Node<Key,Value>* LevelBalancedTree<Key, Value>::findSuccessor(Node<Key,Value>* copy) {
    Node<Key,Value>* n = copy->right;
    while(n != nullptr && n->left != nullptr){
        n = n->left;
    }
    return n;
}

template<typename Key, typename Value>
Node<Key,Value>* LevelBalancedTree<Key, Value>::findPredecessor(Node<Key,Value>* copy) {
    Node<Key,Value>* n = copy->left;
    while(n != nullptr && n->right != nullptr){
        n = n->right;
    }
    return n;
}


template<typename Key, typename Value>
Node<Key, Value>* LevelBalancedTree<Key, Value>::removeSpotFinder(Node<Key,Value>* copy, const Key & key) {

    if (copy == nullptr) {
        return copy;
    }


    if (key < copy->k) { 
        copy = removeSpotFinder(copy->left, key);
    } else if (key > copy->k) {  
        copy = removeSpotFinder(copy->right, key);
    }

    else { 

        if(copy->left == nullptr && copy->right == nullptr) {
            Node<Key,Value> *parent = copy->parent;
            if (parent->right == copy){
                parent->right = nullptr;
            } else {
                parent->left = nullptr;
            }
            delete copy;
            return parent;
        }

        else {

            Node<Key,Value>* replacement = findPredecessor(copy);
            if (replacement == nullptr){ 
                replacement = findSuccessor(copy);
            }

            copy->k = replacement->k;
            copy->v = replacement->v;

            if (replacement->left == nullptr && replacement->right == nullptr){
                if(copy->left == replacement){
                    replacement->parent->left = nullptr;
                }else{replacement->parent->right = nullptr;}

                delete replacement;
                return copy;
            }
            if (replacement->left == nullptr){
                replacement->right->parent = replacement->parent;

                if (replacement->parent->left == replacement){
                    replacement->parent->left = replacement->right;
                } else{
                    replacement->parent->right = replacement->right;}
                copy = replacement->parent;
                delete replacement;
                return copy;
            }
            else {   
                replacement->left->parent = replacement->parent;
                if (replacement->parent->left == replacement){
                    replacement->parent->left = replacement->left;
                } else{
                    replacement->parent->right = replacement->left;}
                copy = replacement->parent;
                delete replacement;
                return copy;
            }
        }
    }
    return copy;
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::removeBalanceHelper(Node<Key,Value> *curr, int leftLevel, int rightLevel){
    bool parent_1_3 = isShape(leftLevel, 1, rightLevel,3);
    bool parent_3_1 = isShape(leftLevel, 3, rightLevel,1);
    bool parent_2_2 = isShape(leftLevel, 2, rightLevel,2);

    if ((parent_2_2) || (leftLevel == rightLevel && leftLevel >=3)){
        curr->height--; 
    }

    else if (parent_1_3) {

        bool isSafe = isShape(heightDif(curr->left, curr->left->left), 2,heightDif(curr->left, curr->left->right),2);
        if (isSafe){
            curr->left--;
            curr->height--;
        } else {
            curr->height--; 
            this->balanceHelper(curr->left, curr); 
        }
    }
    else if (parent_3_1) {

        bool isSafe = isShape(heightDif(curr->right, curr->right->left), 2,heightDif(curr->right, curr->right->right),2);
        if (isSafe){ 
            curr->right--;
            curr->height--;
        } else { 
            curr->height--; 
            this->balanceHelper(curr->right, curr); 
        }
    }
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::remove(const Key &k)
{
    std::cout << "REMOVING : " << k << std::endl;
    if (contains(k) == false) { 
        return;
    } else if (head->k == k && head->right == nullptr && head->left == nullptr) {
        Node<Key, Value>* temp = head;
        head = new Node<Key,Value>();
        delete temp;
        node_amount--;
        std::cout << std::endl << std::endl << std::endl;
        printBT(head);
        return;
    }
    Node<Key,Value> * curr = removeSpotFinder(head,k);

    while (curr != nullptr){

        int curr_left = heightDif(curr, curr->left);
        int curr_right = heightDif(curr, curr->right);

        if ((!curr->left && !curr->right && curr->height >= 2) || (curr_left >= 3 || curr_right >= 3) || (curr_left == 2 && curr_right == 2)){
 
            removeBalanceHelper(curr,curr_left,curr_right);
        }
        curr = curr->parent; 
    }
    node_amount--; 
    //std::cout << std::endl << std::endl << std::endl;
    //printBT(head);
}














template<typename Key, typename Value>
int LevelBalancedTree<Key, Value>::count() {

	return node_amount;

}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::inorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const{
	if (copy == nullptr) {
		return;
	}
	if (copy->null == true) {
		return;
	} 
	inorder_helper(copy->left, res);
	res.push_back(copy->k);
	inorder_helper(copy->right, res);
}

template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::inOrder() const
{
	std::vector<Key> res;
	Node<Key,Value>* copy = head;
	inorder_helper(copy, res);
	return res; 
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::preorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const {
	if (copy == nullptr) {
		return;
	}
	if (copy->null == true) {
		return;
	} 
	res.push_back(copy->k);
	preorder_helper(copy->left, res);
	preorder_helper(copy->right, res);
}

template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::preOrder() const
{
	std::vector<Key> res;
	Node<Key,Value>* copy = head;
	preorder_helper(copy, res);
	return res; 
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::postorder_helper(Node<Key, Value>* copy, std::vector<Key> &res) const {
	if (copy == nullptr) {
		return;
	}
	if (copy->null == true) {
		return;
	} 
	postorder_helper(copy->left, res);
	postorder_helper(copy->right, res);
	res.push_back(copy->k);
}

template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::postOrder() const
{
	std::vector<Key> res;
	Node<Key,Value>* copy = head;
	postorder_helper(copy, res);
	return res; 
}

template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::printBT1(const std::string& prefix, const Node<Key,Value>* node, bool isLeft)
{
    if( node != nullptr) {
		std::cout << prefix;

		std::cout << (isLeft ? "├──" : "└──" );

		// print the value of the node
		std::cout << node->height << ": " << node->k << std::endl;

		// enter the next tree level - left and right branch
		printBT1( prefix + (isLeft ? "│   " : "    "), node->left, true);
		printBT1( prefix + (isLeft ? "│   " : "    "), node->right, false);
	}
}

template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::printBT(Node<Key,Value>* node)
{
    printBT1("", node, false);
}





#endif 
