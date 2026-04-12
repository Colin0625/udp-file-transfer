#pragma once
#include <iostream>
#include <optional>
#include <mutex>


template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;

        Node(T v, Node* n) : value(v), next(n) {}
    };

    Node* head_;
    Node* tail_;
    size_t length_;
    mutable std::mutex queue_mtx_;

    bool prelocked_empty() const {
        return length_ == 0;
    }

public:
    Queue() : head_(nullptr), tail_(nullptr), length_{} {}

    bool empty() const {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        return length_ == 0;
    }

    void enqueue(T value) {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        std::cout << "enqueuing new value" << std::endl;
        Node* n = new Node(value, nullptr);
        if (tail_ != nullptr) {
            tail_->next = n;
        }
        tail_ = n;
        if (head_ == nullptr) {
            head_ = n;
        }
        length_++;
    }
    
    T* dequeue() {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        if (this->prelocked_empty()) {
            return nullptr;
        }
        Node* old = head_;
        T val = old->value;
        head_ = old->next;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        length_--;
        return &(old->value);
    }

    size_t get_length() const {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        return this->length_;
    }

    void print() const {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        if (this->prelocked_empty()) {
            std::cout << "Queue is empty" << std::endl;
            return;
        }
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};