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

	Node<Key, Value> *head;
	int node_amount;

public:
	//Node<Key, Value> *head;
	LevelBalancedTree();
    ~LevelBalancedTree();
	size_t size() const noexcept;
	bool isEmpty() const noexcept;
	bool contains(const Key & k) const noexcept; 

	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	void insert(const Key & k, const Value & v);

	void remove(const Key &k);

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
	//void printTree(Node<Key,Value>* node);


	int heightDif(Node<Key,Value> *parent, Node<Key,Value> *node);
	void balanceHelper(Node<Key,Value> *parent, Node<Key,Value> *node);
	bool shape_check(int nodeL, int v1, int nodeR, int v2);


	Node<Key,Value>* successorHelper(Node<Key,Value>* copy);
	Node<Key,Value>* predecessorHelper(Node<Key,Value>* copy);
	Node<Key, Value>* removeHelper(Node<Key,Value>* copy, Key key);
	void removeBalanceHelper(Node<Key,Value> *curr, int leftLevel, int rightLevel);
};





template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::recursiveDelete(Node<Key, Value>* n) {
	if (n != nullptr) {
		recursiveDelete(n->left);
		recursiveDelete(n->right);
		delete n;
	}
}

template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::LevelBalancedTree()
{
	head = new Node<Key,Value>();
	node_amount = 0;

}

template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::~LevelBalancedTree()
{
	recursiveDelete(head);
}

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
    if (head->null == true) {
        return false;
    }
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
    if (head->null == true) {
        throw ElementNotFoundException("not found");
    }
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
    if (head->null == true) {
        throw ElementNotFoundException("not found");
    }
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
bool LevelBalancedTree<Key, Value>::shape_check(int nodeL, int v1, int nodeR, int v2){
	if (nodeL != v1 || nodeR != v2) {
		return false;
	} else {
		return true;
	}
}

template<typename Key, typename Value>
Node<Key, Value>* LevelBalancedTree<Key, Value>::insert_helper(Node<Key, Value>* copy, const Key k, const Value v) {

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

	bool node_1_1 = shape_check(heightDif(node, node->left), 1, heightDif(node, node->right), 1);
	bool node_1_2 = shape_check(heightDif(node, node->left), 1, heightDif(node, node->right), 2);
	bool node_2_1 = shape_check(heightDif(node, node->left), 2, heightDif(node, node->right), 1);

    bool parent_2_0 = shape_check(heightDif(parent, parent->left), 2, heightDif(parent, parent->right), 0);
	bool parent_0_2 = shape_check(heightDif(parent, parent->left), 0, heightDif(parent, parent->right), 2);
	bool parent_0_1 = shape_check(heightDif(parent, parent->left), 0, heightDif(parent, parent->right), 1);
	bool parent_1_0 = shape_check(heightDif(parent, parent->left), 1, heightDif(parent, parent->right), 0);

	if ( (node_1_2 && (parent_0_1 || parent_1_0)) || (node_1_1 && (parent_0_1 || parent_1_0)) || (node_2_1 && (parent_0_1 || parent_1_0)) ) {
        parent->height++;
		return; // do nothing its balanced
    }

    if ((parent_2_0 && node_1_2)){ 
        //std::cout << "1 " << node->parent->k << " " << node->k << std::endl;
        node->parent->height--;
        node->height--;
        node->left->height++;
        Node<Key,Value> *new_child =  node->left->right; 
        Node<Key,Value> *old_parent = node->parent;

        node->left->parent = parent->parent;
        if (parent->parent == nullptr) {
            this->head = node->left;
        } else if (parent->parent != nullptr) {
            if (parent->parent->left == parent) {
                parent->parent->left = node->left; 
            } else {
                parent->parent->right = node->left; 
            }
        }
        node->parent = node->left;
        node->parent->right = node;
        if (new_child != nullptr) {
            node->left = new_child;
            new_child->parent = node;
		} else {
            node->left = nullptr;
        }
        node->parent->left = old_parent;
        old_parent->right = nullptr;
        old_parent->parent = node->parent;
        //std::cout << old_parent->right->k << std::endl;
    }

    else if ((parent->left == node) && (parent_0_2 && node_1_2)){
        //std::cout << "2" << std::endl;
        node->parent = parent->parent; 
        parent->height--;
        if (parent->parent == nullptr) {
            this->head = node; 
        } else if (parent->parent != nullptr) {
            if (parent->parent->right == parent) {
                parent->parent->right = node;
            } else { 
				parent->parent->left = node;
			} 
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

	else if ((parent->right == node) && (parent_2_0 && node_2_1)){  
        //std::cout << "3" << std::endl;
        node->parent = parent->parent;
        parent->height--;
        if (parent->parent == nullptr) {
            this->head = node;
        } else if (parent->parent != nullptr) {
            if (parent->parent->left == parent) {
                parent->parent->left = node;
            } else { 
				parent->parent->right = node;
			}
        }
        if (node->left == nullptr) {
            node->left = parent;
            parent->parent = node;
            parent->right = nullptr;
        } else {
            node->left->parent = parent;
            parent->right = node->left;
        }
        node->left = parent;
        parent->parent = node;
    }

	else if ((parent_0_2 && node_2_1)) { 
        //std::cout << "4" << std::endl;
        node->parent->height--;
        node->height--;
        node->right->height++;
        Node<Key,Value> *new_child =  node->right->left;
        Node<Key,Value> *old_parent = node->parent; 
        node->right->parent = parent->parent;
        if (parent->parent == nullptr) {
            this->head = node->right; 
        } else if (parent->parent != nullptr) {
            if (parent->parent->right == parent) {
                parent->parent->right = node->right;
            } else {
                parent->parent->left = node->right; 
            }
        }
        node->parent = node->right;
        node->parent->left = node;
        if (new_child == nullptr) {
            node->right = nullptr;
		} else {
            node->right = new_child;
            new_child->parent = node;
        }
        node->parent->right = old_parent;
        old_parent->left = nullptr;
        old_parent->parent = node->parent;
    }
}



template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::insert(const Key & k, const Value & v)
{
    if (contains(k) == true) {
        return;
    }
    //std::cout << "CURRENTLY ON : " << k << std::endl;
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
    //printTree(head);
}


template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::removeBalanceHelper(Node<Key,Value> *copy, int leftsHeight, int rightsHeight){
    if (shape_check(leftsHeight, 3, rightsHeight,1)) {
        if (shape_check(heightDif(copy->right, copy->right->left), 2,heightDif(copy->right, copy->right->right), 2)) {
            copy->right--;
            copy->height--;
        } else {
            copy->height--;
            balanceHelper(copy->right, copy);
        }
    } else if (shape_check(leftsHeight, 1, rightsHeight,3)) {
        if (shape_check(heightDif(copy->left, copy->left->left), 2, heightDif(copy->left, copy->left->right), 2)) {
            copy->height--;
            copy->left--;
        } else {
            copy->height--;
            balanceHelper(copy->left, copy);
        }
    } else if ((leftsHeight == rightsHeight && rightsHeight > 2) || (shape_check(leftsHeight, 2, rightsHeight, 2))){
        copy->height--;
    }
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::remove(const Key &k)
{
    //std::cout << "REMOVING : " << k << std::endl;
    if (contains(k) == false) { 
        return;
    } else if (head->k == k && head->right == nullptr && head->left == nullptr) {
        Node<Key, Value>* temp = head;
        head = new Node<Key,Value>();
        delete temp;
        node_amount--;
        //std::cout << std::endl << std::endl << std::endl;
        //printTree(head);
        return;
    }

    node_amount--;
    Node<Key,Value>* copy = removeHelper(head,k);
    while (copy != nullptr) {
        if ((copy->height >= 2 && copy->left == nullptr && copy->right == nullptr)){
            removeBalanceHelper(copy, copy->height, copy->height);
        } else if ( (heightDif(copy, copy->left) == 2 && heightDif(copy, copy->right) == 2) || (heightDif(copy, copy->right) >= 3) || (heightDif(copy, copy->left) >= 3) ) {
            removeBalanceHelper(copy, heightDif(copy, copy->left), heightDif(copy, copy->right));
        }
        copy = copy->parent;
    }
    //std::cout << std::endl << std::endl << std::endl;
    //printTree(head);
}

template<typename Key, typename Value>
Node<Key, Value>* LevelBalancedTree<Key, Value>::removeHelper(Node<Key,Value>* copy, Key k) {

    if (copy == nullptr) {
        return copy;
    } else if (copy->k < k) {
        copy = removeHelper(copy->right, k);
    } else if (copy->k > k) {
        copy = removeHelper(copy->left, k);
    } //base case and move on if its no equal
    else {
        if (copy->right != nullptr || copy->left != nullptr) {
            Node<Key,Value>* new_node;
            if (predecessorHelper(copy) == nullptr) {
                new_node = successorHelper(copy);
            } else { 
                new_node = predecessorHelper(copy);
            }
            copy->v = new_node->v;
            copy->k = new_node->k;

            if (new_node->right == nullptr && new_node->left != nullptr){
                new_node->left->parent = new_node->parent;
                if (new_node->parent->right == new_node) {
                    new_node->parent->right = new_node->left;
                } else {
                    new_node->parent->left = new_node->left;
                }
                copy = new_node->parent;
                delete new_node;
                return copy;
            } else if (new_node->left == nullptr && new_node->right != nullptr) {   
                new_node->right->parent = new_node->parent;

                if (new_node->parent->right == new_node) {
                    new_node->parent->right = new_node->right;
                } else {
                    new_node->parent->left = new_node->right;
                }
                copy = new_node->parent;
                delete new_node;
                return copy;
            } else if (new_node->left == nullptr && new_node->right == nullptr){
                if (copy->right == new_node) {
                    new_node->parent->right = nullptr;
                } else {
                    new_node->parent->left = nullptr;
                }
                delete new_node;
                return copy;
            }
        } else {
            Node<Key, Value> *parent = copy->parent;
            if (parent->left == copy) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }

            delete copy;
            return parent;
        }
    }
    return copy;
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
Node<Key,Value>* LevelBalancedTree<Key, Value>::predecessorHelper(Node<Key,Value>* copy) {

    Node<Key,Value>* node = copy->left;
    while(node != nullptr && node->right != nullptr){
        node = node->right;
    }
    return node;
}

template<typename Key, typename Value>
Node<Key,Value>* LevelBalancedTree<Key, Value>::successorHelper(Node<Key,Value>* copy) {

    Node<Key,Value>* node = copy->right;
    while (node != nullptr && node->left != nullptr) {
        node = node->left;
    }
    return node;
}
#endif 
