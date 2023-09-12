#include <fstream>
#include <iostream>

#include "remove2.hpp"

int main()
{
    LevelBalancedTree<int, std::string> tree;
    tree.insert(1 , "boo");
    tree.insert(2 , "boo");
    tree.insert(3 , "boo");
    // tree.insert(1, "foo");
    // tree.insert(2, "foo");
    // tree.insert(3, "foo");
    // tree.insert(5, "foo");
    // tree.insert(12, "twelve");
	// tree.insert(10, "bar");
    // // tree.insert(13, "bar");
    // // tree.insert(23, "bar");
    // // tree.insert(17, "bar");
    // // tree.insert(6, "bar");
    tree.remove(2);
    tree.remove(1);
    tree.remove(3);
    // tree.remove(5);
    


    std::cout << tree.size() << std::endl;


    return 0;
}


//  g++ main.cpp -std=c++11 -o p4
