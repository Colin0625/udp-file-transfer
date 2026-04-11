#include <iostream>
#include <thread>

#include "utility/Queue.hpp"


void add(int num, Queue<int>& q) {
    q.enqueue(num);
    std::cout << "added " << num << " to queue at " << &q << std::endl;
}





int main() {
    Queue<int> q{};

    std::thread add1(add, 1, std::ref(q));
    std::thread add2(add, 2, std::ref(q));
    std::thread add3(add, 3, std::ref(q));
    std::thread add4(add, 4, std::ref(q));
    std::thread add5(add, 5, std::ref(q));
    std::thread add6(add, 6, std::ref(q));

    add1.join();
    add2.join();
    add3.join();
    add4.join();
    add5.join();
    add6.join();

    q.print();


    return 0;
}