#pragma once

template <typename T>
class ListNode {
public:
    T value;
    ListNode* next;

    ListNode(T val) {
        value = val;
        next = nullptr;
    }
};

template <typename T>
class List {
private:
    ListNode<T>* head;
    size_t size;
public:
    List() {
        head = nullptr;
        size = 0;
    }

    ~List() {
        clear();
    }

    size_t getSize() const {
        return size;
    }

    void add(const T& item) {
        ++size;
        ListNode<T>* newNode = new ListNode<T>(item);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        ListNode<T>* end = head;
        while (end->next != nullptr) {
            end = end->next;
        }

        end->next = newNode;
    }

    bool get(const size_t index, T& item) {
        if (head == nullptr || index < 0 || index >= size) return false;

        ListNode<T>* current = head;
        for (int i = 0; i < index; ++i) {
            if (current == nullptr) return false;  // Safety check
            current = current->next;
        }
        
        if (current == nullptr) return false;  // Safety check before accessing value
        item = current->value;
        return true;
    }

    bool removeAt(size_t index) {
        if (head == nullptr || index < 0 || index >= size) return false;

        ListNode<T>* previous = nullptr;
        ListNode<T>* removeNode = head;
        
        for (size_t i = 0; i < index; ++i) {
            previous = removeNode;
            removeNode = removeNode->next;
        }
        
        if (previous != nullptr) {
            previous->next = removeNode->next;
        }
        else {
            head = removeNode->next;
        }
        delete removeNode;
        --size;

        return true;
    }

    void clear() {
        if (head == nullptr) return;

        size = 0;
        ListNode<T>* node = head;
        while (node->next != nullptr) {
            ListNode<T>* temp = node;
            node = node->next;
            delete temp;
        }
    }
};