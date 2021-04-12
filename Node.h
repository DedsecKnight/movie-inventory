/*
 * Name: Mike Nguyen
 * netID: mtn190005
 * 
 * */
#ifndef NODE_H
#define NODE_H

template<typename T>
class Node {
public:
    Node<T>();
    Node<T>(T);
    ~Node<T>();
    
    Node<T>*& getLeft();
    Node<T>*& getRight();
    T* getPayload();
    
    void setLeft(Node<T>*);
    void setRight(Node<T>*);
    void setPayload(T);
    
private:
    Node<T> *left, *right;
    T payload;
};

#endif

// Default Constructor
template <typename T>
Node<T>::Node() {
    left = nullptr, right = nullptr;
}

// Overloaded Constructor
template <typename T>
Node<T>::Node(T obj) {
    payload = obj;
    left = nullptr, right = nullptr;
}

// Destructor
template <typename T>
Node<T>::~Node() { }

// Accessor
template<class T>
Node<T>*& Node<T>::getLeft() { return left; } 

template<class T>
Node<T>*& Node<T>::getRight() { return right; } 

template<class T>
T* Node<T>::getPayload() { return &payload; }

// Mutator
template<class T> 
void Node<T>::setLeft(Node<T>* l) { left = l; }

template<class T> 
void Node<T>::setRight(Node<T>* r) { right = r; }

template<class T>
void Node<T>::setPayload(T obj) { payload = obj; }
