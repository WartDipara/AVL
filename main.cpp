#include <iostream>
#include "./head/AVL_template.h"
using namespace std;

void test(){
    AVLTree<int> a;
    for (int i = 0; i < 10; i++)
        a.insert(i);
    cout << "The height of the tree: " << a.getHeight() << endl;
    a.preOrder();
    cout << endl;
    a.inOrder();
    cout << endl;
    a.postOrder();
    cout << endl;
    cout << "delete element:10 "<<endl;
    a.remove(10);

    auto findOrNot = [](const AVLTreeNode<int>* node) {
        if(node)
            cout<<node->key<<endl;
        else
            cout<<"not found such element"<<endl;   
    };
    AVLTreeNode<int>* b = a.search_iterator(10);
    AVLTreeNode<int>* c = a.search_iterator(7);
    findOrNot(b);
    findOrNot(c);
}
int main(){
    test();
    return 0;
}