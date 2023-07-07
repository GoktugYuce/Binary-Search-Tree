#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <set>

class Node {
public:
    int key;
    Node* left;
    Node* right;

    Node(int key) {
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class BinaryTree {
private:
    Node* root;

public:
    BinaryTree() {
        this->root = nullptr;
    }

    void insert(int key) {
        Node* newNode = new Node(key);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (key < current->key) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (key < parent->key) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }
    }
    Node* search(int key) {
        Node* current = root;
        while (current != nullptr && current->key != key) {
            if (key < current->key) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return current;
    }
    //defining best-case order insertion
public:
    void insertBestCaseOrder(std::vector<int>& keys) {
        root = insertBestCaseOrderHelper(keys, 0, keys.size() - 1);
    }

private:
    Node* insertBestCaseOrderHelper(std::vector<int>& keys, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;
        Node* newNode = new Node(keys[mid]);
        newNode->left = insertBestCaseOrderHelper(keys, start, mid - 1);
        newNode->right = insertBestCaseOrderHelper(keys, mid + 1, end);
        return newNode;
    }
};

class AVLTree {
private:
    struct AVLNode {
        int key;
        int height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(int key) {
            this->key = key;
            this->height = 1;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    AVLNode* root;

    int height(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalance(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

public:
    AVLTree() {
        this->root = nullptr;
    }

    void insert(int key) {
        root = insert(root, key);
    }

    AVLNode* search(int key) {
        return search(root, key);
    }

private:
    AVLNode* search(AVLNode* node, int key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == key) {
            return node;
        }
        if (node->key < key) {
            return search(node->right, key);
        }
        else {
            return search(node->left, key);
        }
    }
    AVLNode* insert(AVLNode* node, int key) {
        if (node == nullptr) {
            return new AVLNode(key);
        }

        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else {
            return node;
        }

        node->height = std::max(height(node->left), height(node->right)) + 1;

        int balance = getBalance(node);
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }
};

int main() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 100000);
    std::vector<int> keys(10000);
    std::set<int> keySet;

    // generate a large set of random keys
    for (auto& key : keys) {
        int newKey = dist(rng);
        while (keySet.find(newKey) != keySet.end()) {
            newKey = dist(rng);
        }
        key = newKey;
        keySet.insert(newKey);
    }

    // insert keys into the binary tree in original random order
    BinaryTree tree;
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        tree.insert(key);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "inserting with original random order took: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    // search for all the keys in the binary tree
    for (const auto& key : keys) {
        tree.search(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "finding keys with original random order took: " << duration.count() << " microseconds" << std::endl;

    // insert keys into the binary tree in best-case order
    std::sort(keys.begin(), keys.end());
    BinaryTree bestCaseTree;
    auto start = std::chrono::high_resolution_clock::now();
    bestCaseTree.insertBestCaseOrder(keys);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "inserting with best-case order took: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    // search for all the keys in the binary tree
    for (const auto& key : keys) {
        bestCaseTree.search(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "finding keys with best-case order took: " << duration.count() << " microseconds" << std::endl;

    // insert keys into the set
    std::set<int> set;
    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        set.insert(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "inserting into std::set took: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    // search for all the keys in the set
    for (const auto& key : keys) {
        set.find(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "finding keys with std::set took: " << duration.count() << " microseconds" << std::endl;

    // insert keys into the AVLTree 
    AVLTree avltree;
    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : keys) {
        avltree.insert(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "inserting into AVLTree took: " << duration.count() << " microseconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    // search keys in the AVLTree 
    for (const auto& key : keys) {
        avltree.search(key);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "finding keys with AVLTree took: " << duration.count() << " microseconds" << std::endl;

    return 0;
}