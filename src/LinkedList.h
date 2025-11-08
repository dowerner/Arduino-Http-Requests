/*
 * Arduino-Http-Requests Library
 * File: LinkedList.h
 * 
 * Copyright (c) 2025 Dominik Werner
 * https://github.com/dowerner/Arduino-Http-Requests
 *
 * This file is part of the Arduino-Http-Requests library and is licensed
 * under the MIT License. See LICENSE file for details.
 */

#pragma once

template <typename TClient>
class ListNode {
public:
    TClient value;
    ListNode* next;

    ListNode(TClient val) {
        value = val;
        next = nullptr;
    }
};

template <typename TClient>
class List {
private:
    ListNode<TClient>* head;
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

    void add(const TClient& item) {
        ++size;
        ListNode<TClient>* newNode = new ListNode<TClient>(item);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        ListNode<TClient>* end = head;
        while (end->next != nullptr) {
            end = end->next;
        }

        end->next = newNode;
    }

    bool get(const size_t index, TClient& item) {
        if (head == nullptr || index < 0 || index >= size) return false;

        ListNode<TClient>* current = head;
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

        ListNode<TClient>* previous = nullptr;
        ListNode<TClient>* removeNode = head;
        
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
        ListNode<TClient>* node = head;
        while (node->next != nullptr) {
            ListNode<TClient>* temp = node;
            node = node->next;
            delete temp;
        }
    }
};