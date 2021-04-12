/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#include <utility>
#ifndef BST_H
#define BST_H
#include "Node.h"

template <typename T>
class BST {
public:
    BST<T>();
    BST<T>(Node<T>*);
    ~BST<T>();
    
    Node<T>*& getRoot();
    
    void setRoot(Node<T>*);
    
    void Insert(T);
    void deleteNode(Node<T>*&);
    void Insert(Node<T>*&, T);
    std::pair<Node<T>*, Node<T>*> Search(T);
    std::pair<Node<T>*, Node<T>*> Search(Node<T>*&, Node<T>*&, T);
    void Delete(T);
    void Delete(Node<T>*&, Node<T>*&, T);
    std::pair<Node<T>*, Node<T>*> findLeftmost(Node<T>*, Node<T>*);
private:
    Node<T> *root, *dummy;
};

#endif

template <typename T>
BST<T>::BST() { root = nullptr, dummy = nullptr; }

// Deallocate node after program ends
template <typename T>
void BST<T>::deleteNode(Node<T>* &curr) {
    // Do nothing if current node is already nullptr
    if (!curr) return;
    
    // Deallocate left subtree if neccessary
    if (curr->getLeft()) deleteNode(curr->getLeft());
    curr->setLeft(nullptr);
    
    // Deallocate right subtree if necessary
    if (curr->getRight()) deleteNode(curr->getRight());
    curr->setRight(nullptr);
    
    // Deallocate current node
    delete curr;
    curr = nullptr;
}

template <typename T>
BST<T>::~BST() { deleteNode(root); }

template <typename T>
BST<T>::BST(Node<T>* r) { root = r, dummy = nullptr; }

template <typename T>
Node<T>*& BST<T>::getRoot() { return root; }

template <typename T>
void BST<T>::setRoot(Node<T>* r) { root = r; }

template<typename T>
void BST<T>::Insert(T obj) { Insert(this->root, obj); }

// Insert an object into BST
template <typename T>
void BST<T>::Insert(Node<T>* &curr_node, T obj) {
    // If tree is empty, object will become root
    if (curr_node == nullptr) {
        root = new Node<T>(obj);
        return;
    }
    // If obj < payload in current node, then object will belong to left subtree
    if (obj < *curr_node->getPayload()) {
        // If left child is occupied, call recursive function on left child
        if (curr_node->getLeft()) Insert(curr_node->getLeft(), obj);
        
        // If not, then object will become left child
        else curr_node->setLeft(new Node<T>(obj));
    }
    // Else, then object will belong to right subtree
    else {
        // If right child is occupied, then call recursive function on right child
        if (curr_node->getRight()) Insert(curr_node->getRight(), obj);
        
        // If not, then obj will becom eright child
        else curr_node->setRight(new Node<T>(obj));
    }
}

template <typename T>
std::pair<Node<T>*, Node<T>*> BST<T>::Search(T obj) {
    return Search(dummy, root, obj);
}

// Search for the node in the BST that contains data that matches target 
template <typename T>
std::pair<Node<T>*, Node<T>*> BST<T>::Search(Node<T>*& parent, Node<T>*& curr_node, T target) {
    // If current node is found or tree is null, return current node along with its parent
    if (!curr_node || *curr_node->getPayload() == target) return std::make_pair(parent, curr_node);
    
    // If target < payload in current node, call recursive function on left child
    if (target < *curr_node->getPayload()) return Search(curr_node, curr_node->getLeft(), target);
    
    // Else call recursive function on right child
    return Search(curr_node, curr_node->getRight(), target);
}

template <typename T>
void BST<T>::Delete(T target) { Delete(dummy, root, target); }


// Delete a node from BST that matches target
template <typename T>
void BST<T>::Delete(Node<T>*& par, Node<T>*& curr, T target) {
    // Do nothing if tree is null or pointers go out of bounds
    if (!curr) return;
    
    // If target is found
    if (*curr->getPayload() == target) {
        // If target node has 2 children
        if (curr->getLeft() && curr->getRight()) {
            // Find successor of target node
            std::pair<Node<T>*, Node<T>*> successor = findLeftmost(curr, curr->getRight());
            
            // Copy data from successor to current node
            curr->setPayload(*successor.second->getPayload());
            
            // Delete successor
            Delete(successor.first, successor.second, *curr->getPayload());
        }
        
        // If target node has 1 child
        else if (curr->getLeft() || curr->getRight()) {
            // Create a pointer that points to target's child
            Node<T>* temp = (curr->getLeft() ? curr->getLeft() : curr->getRight());
            // If target has no parent (target is root), then child will become root
            if (!par) {
                delete curr;
                curr = temp;
            }
            // If target has parent
            else {
                // Check if target is its parent's left child
                bool isLeft = (par->getLeft() == curr);
                
                // Deallocate target node
                delete curr;
                curr = nullptr;
                
                // Set parent's pointer based on isLeft variable
                if (isLeft) par->setLeft(temp);
                else par->setRight(temp);
                
            }
        }
        
        // If target node has no children
        else {
            // If target node has parent, set parent's pointer that pointed to target to nullptr
            if (par){
                if (par->getLeft() == curr) par->setLeft(nullptr);
                else par->setRight(nullptr);
            }
            // Deallocate target node
            delete curr;
            curr = nullptr;
        }
    }
    // If target < payload in current node, call recursive function on left child
    else if (target < *curr->getPayload()) Delete(curr, curr->getLeft(), target);
    // Else call recursive function on right child
    else Delete(curr, curr->getRight(), target);
}

// Find the leftmost node of a subtree along with its parent
template <typename T>
std::pair<Node<T>*, Node<T>*> BST<T>::findLeftmost(Node<T>* par, Node<T>* curr) {
    // If current node is null, then parent is also null
    if (!curr) return std::make_pair(nullptr, nullptr);
    
    // If current node is the leftmost node, return current node and its parent
    if (!curr->getLeft()) return std::make_pair(par, curr);

    // If not, call recursive function of left child
    return findLeftmost(curr, curr->getLeft());
}
