#ifndef __PROJ_FOUR_LEVEL_BALANCED_TREE_HPP
#define __PROJ_FOUR_LEVEL_BALANCED_TREE_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>
#include <utility>
using namespace std;

class ElementNotFoundException : public RuntimeException
{
public:
    ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};

template <typename Key, typename Value>
struct Node{
    Node *left;
    Node *right;
    Key key;
    Value value;
    int level;
    Node *parent;
};

template<typename Key, typename Value>
class LevelBalancedTree
{
private:
    // fill in private member data here
    // If you need to declare private functions, do so here too.
    int treeSize;
    //Node<Key, Value> *root;

public:
    Node<Key, Value> *root;
    LevelBalancedTree();
    // In general, a copy constructor and assignment operator
    // are good things to have.
    // For this quarter, I am not requiring these.
    //	LevelBalancedTree(const LevelBalancedTree & st);
    //	LevelBalancedTree & operator=(const LevelBalancedTree & st);


    // The destructor is required.
    ~LevelBalancedTree();

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
    Node<Key, Value>* insertSpotFinder(Node<Key,Value>*head, const Key & k,const Value & v);
    bool isShape(int nodeLeft, int leftVal, int nodeRight, int rightVal);
    void balanceTheTree(const Key & k, Node<Key,Value> *child, Node<Key,Value> *parent);
    int getLevel(Node<Key,Value> *parent, Node<Key,Value> *child);
    void insert(const Key & k, const Value & v);
    void printBT(Node<Key,Value>* node);
    void printBT1(const std::string& prefix, const Node<Key,Value>* node, bool isLeft);


    // Deletes the given key from the tree
    // and performs the balancing operation(s) if needed.
    // If the key does not exist in the tree,
    // do not modify the tree.
    void remove(const Key &k);


    // The following three functions all return
    // the set of keys in the tree as a standard vector.
    // Each returns them in a different order.
    std::vector<Key> inOrder() const;
    void inOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const;
    void preOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const;
    void postOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const;
    std::vector<Key> preOrder() const;
    std::vector<Key> postOrder() const;
};


template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::LevelBalancedTree()
{
    treeSize = 0;
    root = nullptr;
}

template<typename Key, typename Value>
LevelBalancedTree<Key,Value>::~LevelBalancedTree()
{
//    if (root == nullptr) {
//        return;
//    }
//    ~LevelBalancedTree();
//    ~LevelBalancedTree();
//    delete root;

}

template<typename Key, typename Value>
size_t LevelBalancedTree<Key, Value>::size() const noexcept
{
    return treeSize;
}

template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::isEmpty() const noexcept
{
    return treeSize == 0;
}


template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::contains(const Key &k) const noexcept
{
    Node<Key,Value>* current = root;
    while (current != nullptr) {
        if (k == current->key) {
            return true;
        } else if (k < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

template<typename Key, typename Value>
Value & LevelBalancedTree<Key, Value>::find(const Key & k)
{
    if (root == nullptr){
        throw ElementNotFoundException("This key doesn't exist.");
    }
    if (root->key == k){
        return root->value;
    }
    return contains(root->left) || contains(root->right);

}

template<typename Key, typename Value>
const Value & LevelBalancedTree<Key, Value>::find(const Key & k) const
{
    if (root == nullptr){
        throw ElementNotFoundException("This key doesn't exist.");
    }
    if (root->key == k){
        return root->value;
    }
    return contains(root->left) || contains(root->right);

}

template<typename Key, typename Value>
Node<Key, Value>* LevelBalancedTree<Key, Value>::insertSpotFinder(Node<Key,Value>*head,const Key & k,const Value & v){
    // this returns the new node's pointer
    if (head == nullptr){
        head = new Node<Key,Value>{nullptr,nullptr,k,v,1,nullptr};
        this->root = head;
        return head;
    }

    if (k < head->key){
        if (head->left == nullptr){
            head->left = new Node<Key,Value>{nullptr,nullptr,k,v,1,head};
            return head->left;
        }
        else {
            head = this->insertSpotFinder(head->left,k,v);
        }
    }
    else {
        if (head->right == nullptr){
            head->right = new Node<Key,Value>{nullptr,nullptr,k,v,1,head};
            return head->right;
        }
        else{
            head = this->insertSpotFinder(head->right,k,v);
        }
    }
    return head;
}

template<typename Key, typename Value>
int LevelBalancedTree<Key, Value>::getLevel(Node<Key,Value> *parent, Node<Key,Value> *child){
    // returns the level difference by subtracting child from parent
    if (child == nullptr){
        return parent->level;
    }
    return parent->level - child->level;

}

template<typename Key, typename Value>
bool LevelBalancedTree<Key, Value>::isShape(int nodeLeft, int leftVal, int nodeRight, int rightVal){
    // helper function to help confirm the shape of the node (ex. (1,1))
    return nodeLeft == leftVal and nodeRight == rightVal;

}


template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::balanceTheTree(const Key & k, Node<Key,Value> *child, Node<Key,Value> *parent){
    // doing all the work to balance tree to (1,1)
    int child_l_level = getLevel(child, child->left);
    int child_r_level = getLevel(child, child->right);
    int parent_l_level= getLevel(parent,parent->left);
    int parent_r_level= getLevel(parent,parent->right);

    // making base cases more readable with boolean values
    bool child_1_1 = isShape(child_l_level, 1,child_r_level,1);
    bool parent_0_1 = isShape(parent_l_level,0,parent_r_level,1);
    bool parent_1_0 = isShape(parent_l_level,1,parent_r_level,0);
    bool child_1_2 = isShape(child_l_level,1,child_r_level,2 );
    bool child_2_1 = isShape(child_l_level, 2, child_r_level,1);
    bool parent_2_0 = isShape(parent_l_level,2,parent_r_level,0);
    bool parent_0_2 = isShape(parent_l_level,0, parent_r_level,2);

    if ((child_1_1 && parent_0_1) || (child_1_1 && parent_1_0)|| (child_2_1 && parent_0_1) || (child_1_2 && parent_0_1) || (parent_1_0 && child_1_2) || (parent_1_0 && child_2_1)){
        parent->level++;
    }

        // starting rotation without child's children full
    else if ((child_1_2 && parent_0_2) && (parent->left == child)){ // left version of rotate (push parent down)
        parent->level--; // push the parent down!
        child->parent = parent->parent; // setting 12 to 18's parent
        if (parent->parent != nullptr){
            if (parent->parent->left == parent) {
                parent->parent->left = child;
            }
            else{ parent->parent->right = child;}
        }
        else{
            this->root = child; // reset the Tree root
        }

        // child is not empty
        if (child->right != nullptr){
            child->right->parent = parent;
            parent->left = child->right;
        }
        else{
            child->right = parent; // 12's right is 18
            parent->parent = child;
            parent->left = nullptr;
        }
        child->right = parent;
        parent->parent = child;

    }

    else if ((child_2_1 && parent_2_0) && (parent->right == child)){  // right version of no child rotate
        parent->level--; // push the parent down!
        child->parent = parent->parent; // setting 12 to 18's parent

        if (parent->parent != nullptr){
            if (parent->parent->left == parent) {
                parent->parent->left = child;
            }
            else{ parent->parent->right = child;}
        }
        else{
            this->root = child; // reset the Tree root
        }
        // child is not empty
        if (child->left != nullptr){
            child->left->parent = parent;
            parent->right = child->left;
        }
        else{
            child->left = parent; // 12's right is 18
            parent->parent = child;
            parent->right = nullptr;
        }
        child->left = parent;
        parent->parent = child;

    }
    else if ((child_2_1 && parent_0_2)){ // left version of crazy case
        // child node's children present, child's child go up, child and paren v vt go down
        // AKA THE CRAZY CASE
        child->level--;
        child->parent->level--;
        child->right->level++;

        Node<Key,Value> *old_parent = child->parent; // 19
        Node<Key,Value> *childs_new_child =  child->right->left; // 17

        child->right->parent = parent->parent;
        if(parent->parent != nullptr){
            if (parent->parent->left == parent){
                parent->parent->left = child->right; //pointing back to raised node
            }
            else {
                parent->parent->right = child->right; //pointing back to raised node
            }


        }
        else {
            this->root = child->right; // setting new root if it's grandparent is nullptr
        }
        child->parent = child->right;
        child->parent->left = child;

        if (childs_new_child != nullptr){
            child->right = childs_new_child;
            childs_new_child->parent = child;}
        else{
            child->right = nullptr;
        }
        child->parent->right = old_parent;
        old_parent->parent = child->parent;
        old_parent->left = nullptr;

    }
    else if ((child_1_2 && parent_2_0)){ // right version of crazy case
        child->level--;
        child->parent->level--;
        child->left->level++;

        Node<Key,Value> *old_parent = child->parent; // 19
        Node<Key,Value> *childs_new_child =  child->left->right; // 17

        child->left->parent = parent->parent;
        if(parent->parent != nullptr){
            if (parent->parent->left == parent){
                parent->parent->left = child->left; //pointing back to raised node
            }
            else {
                parent->parent->right = child->left; //pointing back to raised node
            }


        }
        else {
            this->root = child->left; // setting new root if it's grandparent is nullptr
        }
        child->parent = child->left;
        child->parent->right = child;

        if (childs_new_child != nullptr){
            child->left = childs_new_child;
            childs_new_child->parent = child;}
        else{
            child->left = nullptr;
        }

        child->parent->left = old_parent;
        old_parent->parent = child->parent;
        old_parent->left = nullptr;
    }
    // done with crazy case


}


template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::insert(const Key & k, const Value & v)
{
    // get pointer to the newly inserted node
    Node<Key,Value>* curr = insertSpotFinder(root,k,v);
    // balancing
    while (curr != root){ // while not reaching the end of the root
        if (curr->level == curr->parent->level){ // needs balance if on same level
            this->balanceTheTree(k,curr, curr->parent);
            }
            if(curr != root){
                curr = curr->parent; // next iteration
            }

        }
    treeSize++;
}

template<typename Key, typename Value>
void LevelBalancedTree<Key, Value>::remove(const Key &k)
{

    treeSize--;
    return;
}

template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::postOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const{
    if (rt == nullptr) {
        return;
    }
    postOrderHelper(rt->left,v);
    postOrderHelper(rt->right,v);
    v.push_back(rt->key);
}

template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::preOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const{
    if (rt == nullptr) {
        return;
    }
    v.push_back(rt->key);
    preOrderHelper(rt->left,v);
    preOrderHelper(rt->right,v);

}

template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::inOrderHelper(Node<Key,Value> *rt, std::vector<Key> &v) const{
    if (rt == nullptr) {
        return;
    }
    inOrderHelper(rt->left,v);
    v.push_back(rt->key);
    inOrderHelper(rt->right,v);

}

template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::inOrder() const
{
    std::vector<Key> inOrderKeys;
    inOrderHelper(this->root,inOrderKeys);
    return inOrderKeys;
}


template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::preOrder() const
{
    std::vector<Key> preOrderKeys;
    preOrderHelper(this->root,preOrderKeys);
    return preOrderKeys;
}


template<typename Key, typename Value>
std::vector<Key> LevelBalancedTree<Key, Value>::postOrder() const
{
    std::vector<Key> postOrderKeys;
    postOrderHelper(this->root, postOrderKeys);
    return postOrderKeys;
}
// PRINT FUNCTION PRINT FUNCTION// PRINT FUNCTION PRINT FUNCTION// PRINT FUNCTION PRINT FUNCTION
template<typename Key, typename Value>
void LevelBalancedTree<Key,Value>::printBT1(const std::string& prefix, const Node<Key,Value>* node, bool isLeft)
{
    if( node != nullptr )
    {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        std::cout << node->level << ": " << node->key << std::endl;

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
