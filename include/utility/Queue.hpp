#pragma once
#include <iostream>
#include <optional>

template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;

        Node(T v, Node* n) : value(v), next(n) {}
    };

    Node* head;
    Node* tail;
    size_t length;

public:
    Queue() : head(nullptr), tail(nullptr), length{} {}

    bool empty() const {
        return length == 0;
    }

    void enqueue(T value) {
        Node* n = new Node(value, nullptr);
        if (tail != nullptr) {
            tail->next = n;
        }
        tail = n;
        if (head == nullptr) {
            head = n;
        }
        length++;
    }
    
    T* dequeue() {
        if (this->empty()) {
            return nullptr;
        }
        Node* old = head;
        T val = old->value;
        head = old->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete old;
        length--;
        return val;
    }

    const size_t get_length() const {
        return this->length;
    }

    void print() const {
        if (this->empty()) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};