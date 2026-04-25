#pragma once
#include <iostream>
#include <optional>
#include <mutex>
#include <condition_variable>


template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;

        Node(T v, Node* n) : value(std::move(v)), next(n) {}
    };

    Node* head_;
    Node* tail_;
    size_t length_;
    mutable std::mutex queue_mtx_;
    bool running_;
    std::condition_variable cv_;


    bool prelocked_empty() const {
        return length_ == 0;
    }

public:
    Queue() : head_(nullptr), tail_(nullptr), length_{}, running_(true) {}

    bool empty() const {
        std::lock_guard<std::mutex> qlock(queue_mtx_);
        return length_ == 0;
    }

    void enqueue(T value) {
        std::lock_guard<std::mutex> qlock(queue_mtx_);
        std::cout << "QUEUE: enqueuing new value" << std::endl;
        Node* n = new Node(std::move(value), nullptr);
        if (tail_ != nullptr) {
            tail_->next = n;
        }
        tail_ = n;
        if (head_ == nullptr) {
            head_ = n;
        }
        length_++;
        cv_.notify_one();
    }
    
    T dequeue() {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        std::cout << "QUEUE: Blocking for dequeue" << std::endl;
        cv_.wait(qlock, [this]{ return !prelocked_empty() || !running_; });
        std::cout << "QUEUE: Done blocking" << std::endl;
        // if (this->prelocked_empty()) {
        //     return ;
        // }
        Node* old = head_;
        T val = std::move(old->value);
        head_ = old->next;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        length_--;
        delete old;
        qlock.unlock();
        return val;
    }

    size_t get_length() const {
        std::lock_guard<std::mutex> qlock(queue_mtx_);
        return this->length_;
    }

    void print() const {
        std::lock_guard<std::mutex> qlock(queue_mtx_);
        if (this->prelocked_empty()) {
            std::cout << "QUEUE: Queue is empty" << std::endl;
            return;
        }
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void stop_queue() {
        std::unique_lock<std::mutex> qlock(queue_mtx_);
        running_ = false;
        cv_.notify_all();
        qlock.unlock();
    }
};