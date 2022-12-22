#include "solution.h"

#include <vector>

bool isContains(const Node *node, int key) {
    if (!node)
        return false;

    if (node->key > key)
        return isContains(node->left, key);
    else if (node->key < key)
        return isContains(node->right, key);

    return true;
}

Node *insert(Node *node, int value) {
    if (!node)
        return new Node(value);
    if (node->key > value)
        node->left = insert(node->left, value);
    else if (node->key < value)
        node->right = insert(node->right, value);

    return node;
}

Node *getMax(Node *node) {
    if (!node)
        return nullptr;

    if (!node->right)
        return node;

    return getMax(node->right);
}

Node *getMin(Node *node) {
    if (!node)
        return nullptr;

    if (!node->left)
        return node;

    return getMin(node->left);
}


Node *remove(Node *node, int key) {
    if (!node) return nullptr;

    if (key == node->key && !node->left && !node->right) {
        delete node;
        return nullptr;
    }

    if (key == node->key) {
        Node *temp;
        if (!node->left) {
            temp = node->right;
            delete node;
        } else if (!node->right) {
            temp = node->left;
            delete node;
        } else {
            temp = getMin(node->right);

            Node *newNode = new Node(temp->key);
            newNode->left = node->left;
            newNode->right = node->right;

            delete node;
            newNode->right = remove(newNode->right, temp->key);
            return newNode;
        }

        return temp;
    }

    if (key < node->key)
        node->left = remove(node->left, key);
    if (key > node->key)
        node->right = remove(node->right, key);

    return node;
}

Node *removeGreaterThanX(Node *tree, int X) {
    if (!tree)
        return nullptr;

    while (tree->key > X)
        tree = remove(tree, tree->key);

    return tree;
}

int getHeight(Node *tree) {
    if (!tree) return 0;

    return 1 + std::max(getHeight(tree->left), getHeight(tree->right));
}

bool isBalanced(Node *tree) {
    if (!tree) return true;

    if (std::abs(getHeight(tree->left) - getHeight(tree->right) > 1))
        return false;

    return isBalanced(tree->left) && isBalanced(tree->right);
}

std::vector<int> toArray(const Node *tree) {
    std::vector<int> array;

    if (!tree)
        return array;

    toArray(tree->left);
    array.push_back(tree->key);
    toArray(tree->right);

    return array;
}

std::vector<int> intersectionHelper(const Node *l, const Node *r) {
    std::vector<int> left(toArray(l));
    std::vector<int> right(toArray(r));

    size_t size = std::min(left.size(), right.size());

    size_t leftIt = 0;
    size_t rightIt = 0;

    std::vector<int> inter;

    while (leftIt != size && rightIt != size) {
        if (left.at(leftIt) == right.at(rightIt)) {
            inter.push_back(left.at(leftIt));
            ++leftIt;
            ++rightIt;
        } else if (left.at(leftIt) < right.at(rightIt))
            ++leftIt;
        else ++rightIt;
    }

    return inter;
}

Node *section(const Node *l, const Node *r) {
    std::vector<int> intersection(intersectionHelper(l, r));
    size_t size = intersection.size();

    if (!size)
        return nullptr;

    Node *tree = new Node(intersection.at(0));

    for (size_t i = 1; i < size; i++)
        tree = insert(tree, intersection.at(i));

    return tree;
}

size_t level(const Node *tree, int key) {
    if (tree->key == key)
        return 1;
    else if (tree->key > key)
        return 1 + level(tree->left, key);
    else return 1 + level(tree->right, key);
}

const Node *findParent(const Node *tree, int key) {
    if (tree->key > key) {

        if (tree->left) {
            if (tree->left->key == key)
                return tree;
            else return findParent(tree->left, key);
        } else return nullptr;

    } else if (tree->key < key) {

        if (tree->right) {
            if (tree->right->key == key)
                return tree;
            else return findParent(tree->right, key);
        } else return nullptr;

    } else return nullptr;
}

bool areCousins(const Node *tree, int key1, int key2) {
    if (!tree)
        return false;

    if (!isContains(tree, key1) || !isContains(tree, key2))
        return false;

    if (findParent(tree, key1) == findParent(tree, key2))
        return false;

    if (level(tree, key1) == level(tree, key2))
        return true;
    else return false;
}

int findLCA(const Node *tree, int key1, int key2) {
    if ((key1 <= tree->key && key2 >= tree->key) || (key1 >= tree->key && key2 <= tree->key))
        return tree->key;
    else if (key1 > tree->key && key2 > tree->key)
        return LCA(tree->right, key1, key2);
    else if (key1 < tree->key && key2 < tree->key)
        return LCA(tree->left, key1, key2);
}

int LCA(const Node *tree, int key1, int key2) {
    if (!isContains(tree, key1) || !isContains(tree, key2))
        return -1;

    return findLCA(tree, key1, key2);
}