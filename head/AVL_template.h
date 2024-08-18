#ifndef AVL_TEMPLATE_H
#define AVL_TEMPLATE_H
#define ll long long // long long类型
using namespace std;
/**
 * AVL平衡树
 * 特点 ：
 * 1.必须是二叉树
 * 2.左子树和右子树的高度差不超过1
 * 3.满足二叉搜索树的性质 （左孩子小于父亲，右孩子大于父亲）
 * 平衡因子bf(balance factor)∈[-1,1]：**左子树高度减去右子树高度** 称为平衡因子
 *
 * 左子树失衡就右旋，右子树失衡就左旋
 */
template <typename T>
class AVLTreeNode
{
public:
    AVLTreeNode(T value, AVLTreeNode *left, AVLTreeNode *right) : key(value), lchild(left), rchild(right) {};
    T key;
    size_t height;
    AVLTreeNode<T> *lchild;
    AVLTreeNode<T> *rchild;
};
template <typename T>
class AVLTree
{
public:
    AVLTree() : root(nullptr) {};
    ~AVLTree()
    {
        cout<<"destroy tree"<<endl;
        destroy();
    };

    void preOrder();  // 先序遍历
    void inOrder();   // 中序遍历
    void postOrder(); // 后序遍历

    void destroy();   // 销毁树

    void insert(T value); // 插入节点
    void remove(T value); // 删除节点

    AVLTreeNode<T> *search_recurse(T key);  // 递归查找
    AVLTreeNode<T> *search_iterator(T key); // 迭代查找

    T findMin(); // 查找最小值
    T findMax(); // 查找最大值

    int getHeight(); // 获取树高
private:
    AVLTreeNode<T> *root; // 树根

    void preOrder(AVLTreeNode<T> *pnode) const;
    void inOrder(AVLTreeNode<T> *pnode) const;
    void postOrder(AVLTreeNode<T> *pnode) const;

    void printTree(AVLTreeNode<T> *pnode, T key, int direction) const;
    void destroy(AVLTreeNode<T> *&pnode);

    AVLTreeNode<T> *insert(AVLTreeNode<T> *&pnode, T key);
    AVLTreeNode<T> *remove(AVLTreeNode<T> *&pnode, T key); 

    AVLTreeNode<T> *search_recurse(AVLTreeNode<T> *pnode, T key) const;
    AVLTreeNode<T> *search_iterator(AVLTreeNode<T> *pnode, T key) const;

    AVLTreeNode<T> *findMin(AVLTreeNode<T> *pnode) const;
    AVLTreeNode<T> *findMax(AVLTreeNode<T> *pnode) const;

    int getHeight(AVLTreeNode<T> *pnode);

    // 旋转 LL LR RL RR 类型
    AVLTreeNode<T> *LL_Rotation(AVLTreeNode<T> *pnode); // 右旋
    AVLTreeNode<T> *LR_Rotation(AVLTreeNode<T> *pnode); // 先左旋后右旋
    AVLTreeNode<T> *RL_Rotation(AVLTreeNode<T> *pnode); // 先右旋后左旋
    AVLTreeNode<T> *RR_Rotation(AVLTreeNode<T> *pnode); // 左旋
};
/**
 * 获取节点高度
 * BF值可以透过左右子树的高度差计算
 */
template <typename T>
int AVLTree<T>::getHeight(AVLTreeNode<T> *pnode)
{
    if (pnode != nullptr)
    {
        return pnode->height;
    }
    return 0;
}
template <typename T>
int AVLTree<T>::getHeight()
{
    return getHeight(root);
}

/**
 * 左旋操作 RR型
 * pnode为最小失衡根节点
 * 返回旋转后,新的根节点
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::RR_Rotation(AVLTreeNode<T> *proot)
{
    /** 旋转操作 */
    AVLTreeNode<T> *newRoot = proot->rchild;
    proot->rchild = newRoot->lchild;
    newRoot->lchild = proot;
    /** 更新节点高度 */
    proot->height = max(getHeight(proot->lchild), getHeight(proot->rchild)) + 1;
    newRoot->height = max(getHeight(newRoot->lchild), getHeight(newRoot->rchild)) + 1;
    return newRoot;
}
/**
 * 右旋操作 LL型
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::LL_Rotation(AVLTreeNode<T> *proot)
{
    /** 旋转操作 */
    AVLTreeNode<T> *newRoot = proot->lchild;
    proot->lchild = newRoot->rchild;
    newRoot->rchild = proot;
    /** 更新节点高度 */
    proot->height = max(getHeight(proot->lchild), getHeight(proot->rchild)) + 1;
    newRoot->height = max(getHeight(newRoot->lchild), getHeight(newRoot->rchild)) + 1;
    return newRoot;
}
/**
 * 先(这里以右子树为根节点)左旋后进行右旋
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::LR_Rotation(AVLTreeNode<T> *proot)
{
    proot->rchild = RR_Rotation(proot->rchild); // 先对右子树做左旋操作
    return LL_Rotation(proot);                  // 最后做右旋操作
}

/**
 * 先(这里以左子树为根节点)右旋后进行左旋
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::RL_Rotation(AVLTreeNode<T> *proot)
{
    proot->lchild = LL_Rotation(proot->lchild); // 先对左子树做右旋操作
    return RR_Rotation(proot);                  // 最后做左旋操作
}

/**
 * 插入操作
 * 递归插入
 * return 完成插入操作后的根节点
 */
template<typename T>
void AVLTree<T>::insert(T value){
    root=insert(root, value);
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::insert(AVLTreeNode<T> *&pnode, T key)
{
    if (pnode == nullptr)
    {
        // 找到符合的插入点
        pnode = new AVLTreeNode<T>(key, nullptr, nullptr);
    }
    else if (key < pnode->key)
    {
        // 根据定义，需要找左子树区域
        pnode->lchild = insert(pnode->lchild, key);
        if (getHeight(pnode->lchild)- getHeight(pnode->rchild) == 2)
        {
            // 左子树失衡
            if (key < pnode->lchild->key)
            {
                // 插入到了左子树的左孩子上，LL型
                pnode = LL_Rotation(pnode);
            }
            else if (key > pnode->lchild->key)
            {
                // 插入到了左子树的右孩子上，LR型
                pnode = LR_Rotation(pnode);
            }
        }
    }
    else if (key > pnode->key)
    {
        // 根据定义，需要找右子树区域
        pnode->rchild = insert(pnode->rchild, key);
        if (getHeight(pnode->rchild)- getHeight(pnode->lchild) == 2)
        {
            // 右子树失衡
            if (key > pnode->rchild->key)
            {
                // 插入到了右子树的右孩子上，RR型
                pnode = RR_Rotation(pnode);
            }
            else if (key < pnode->rchild->key)
            {
                // 插入到了右子树的左孩子上，RL型
                pnode = RL_Rotation(pnode);
            }
        }
    }
    pnode->height = max(getHeight(pnode->lchild), getHeight(pnode->rchild)) + 1;
    return pnode;
}
/**
 * 删除指定元素
 */
template<typename T>
void AVLTree<T>::remove(T key){
    remove(root, key);
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::remove(AVLTreeNode<T> *&pnode, T key)
{
    if (pnode != nullptr)
    {
        if (key == pnode->key)
        { // 找到删除的节点
            if (pnode->lchild && pnode->rchild)
            { // 该节点存在左右子树
                if (getHeight(pnode->lchild) > getHeight(pnode->rchild))
                {
                    // 左子树更高,选择左子树的节点进行替换
                    AVLTreeNode<T> *dummyNode = findMax(pnode->lchild);    // 找到左子树的最大节点
                    pnode->key = dummyNode->key;                           // 最大节点的值覆盖掉当前节点
                    pnode->lchild = remove(pnode->lchild, dummyNode->key); // 递归删除最大节点
                }
                else
                {
                    // 右子树更高,选择右子树的节点进行替换
                    AVLTreeNode<T> *dummyNode = findMin(pnode->rchild);    // 找到右子树的最小节点
                    pnode->key = dummyNode->key;                           // 最小节点的值覆盖掉当前节点
                    pnode->rchild = remove(pnode->rchild, dummyNode->key); // 递归删除最小节点
                }
            }
            else
            {
                AVLTreeNode<T> *dummyNode = pnode;
                if (pnode->lchild)
                {
                    pnode = pnode->lchild;
                }
                else if (pnode->rchild)
                    pnode = pnode->rchild;
                delete dummyNode;
                return nullptr;
            }
        }
        else if (key > pnode->key)
        {
            pnode->rchild = remove(pnode->rchild, key);
            // 删除节点后，如果右子树失衡，需要进行旋转
            if (getHeight(pnode->rchild) - getHeight(pnode->lchild) == 2)
            {
                if (getHeight(pnode->lchild->lchild) > getHeight(pnode->lchild->rchild))
                {
                    // LL型
                    pnode = LL_Rotation(pnode->lchild);
                }
                else
                {
                    // LR型
                    pnode = LR_Rotation(pnode->lchild);
                }
            }
        }
        else
        {
            // 剩下key < pnode->key的情况
            // 进行左子树删除操作
            pnode->lchild = remove(pnode->lchild, key);
            // 删除节点后，如果左子树失衡，需要进行旋转
            if (getHeight(pnode->lchild) - getHeight(pnode->rchild) == 2)
            {
                if (getHeight(pnode->rchild->rchild) > getHeight(pnode->rchild->lchild))
                {
                    // RR型
                    pnode = RR_Rotation(pnode->rchild);
                }
                else
                {
                    // RL型
                    pnode = RL_Rotation(pnode->rchild);
                }
            }
        }
        return pnode;
    }
    return nullptr;
}

/**
 * 递归查找
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::search_recurse(T key)
{
    return search_recurse(root, key);
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::search_recurse(AVLTreeNode<T> *pnode, T key) const
{
    if (pnode)
    {
        if (key == pnode->key)
        {
            // 找到了
            return pnode;
        }
        else if (key > pnode->key)
        {
            // 向右递归
            return search_recurse(pnode->rchild, key);
        }
        else
        {
            // 向左递归
            return search_recurse(pnode->lchild, key);
        }
    }
    return nullptr;
}

/**
 * 迭代查找
 */
template <typename T>
AVLTreeNode<T> *AVLTree<T>::search_iterator(T key)
{
    return search_iterator(root, key);
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::search_iterator(AVLTreeNode<T> *pnode, T key) const
{
    while (pnode)
    {
        if (key == pnode->key)
        {
            // 找到了
            return pnode;
        }
        else if (key > pnode->key)
        {
            // 向右找
            pnode = pnode->rchild;
        }
        else
        {
            // 向左找
            pnode = pnode->lchild;
        }
    }
    return nullptr;
}
/**
 * 先序遍历
 */
template <typename T>
void AVLTree<T>::preOrder()
{
    cout << "preOrder: ";
    preOrder(root);
}
template <typename T>
void AVLTree<T>::preOrder(AVLTreeNode<T> *pnode) const
{
    if (pnode)
    {
        cout << pnode->key << " ";
        preOrder(pnode->lchild);
        preOrder(pnode->rchild);
    }
}

/**
 * 中序遍历
 */
template <typename T>
void AVLTree<T>::inOrder()
{
    cout << "inOrder: ";
    inOrder(root);
}
template <typename T>
void AVLTree<T>::inOrder(AVLTreeNode<T> *pnode) const
{

    if (pnode)
    {
        inOrder(pnode->lchild);
        cout << pnode->key << " ";
        inOrder(pnode->rchild);
    }
}
/**
 * 后序遍历
 */
template <typename T>
void AVLTree<T>::postOrder()
{
    cout << "postOrder: ";
    postOrder(root);
}
template <typename T>
void AVLTree<T>::postOrder(AVLTreeNode<T> *pnode) const
{
    if (pnode)
    {
        postOrder(pnode->lchild);
        postOrder(pnode->rchild);
        cout << pnode->key << " ";
    }
}

/**
 * 用深搜的方式销毁树
 */
template <typename T>
void AVLTree<T>::destroy()
{
    destroy(root);
}
template <typename T>
void AVLTree<T>::destroy(AVLTreeNode<T> *&pnode)
{
    if (pnode)
    {
        destroy(pnode->lchild);
        destroy(pnode->rchild);
        delete pnode;
        pnode = nullptr;
    }
}
/**
 * 求最大值
 */
template <typename T>
T AVLTree<T>::findMax()
{
    AVLTreeNode<T> *presult = findMax(root);
    if (presult)
    {
        return presult->key;
    }
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::findMax(AVLTreeNode<T> *pnode) const
{
    if (pnode)
    {
        while (pnode->rchild)
        {
            pnode = pnode->rchild;
        }
        return pnode;
    }
    return nullptr;
}

/**
 * 求最小值
 */
template <typename T>
T AVLTree<T>::findMin()
{
    AVLTreeNode<T> *presult = findMin(root);
    if (presult)
    {
        return presult->key;
    }
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::findMin(AVLTreeNode<T> *pnode) const
{
    if (pnode)
    {
        while (pnode->lchild)
        {
            pnode = pnode->lchild;
        }
        return pnode;
    }
    return nullptr;
}

#endif