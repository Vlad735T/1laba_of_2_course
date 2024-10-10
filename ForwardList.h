#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct ForwardList {
private:
    struct Node {
        string person;
        Node* next;
        Node(string string_per) : person(string_per), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    ForwardList();
    ~ForwardList();

    bool is_empty() const;
    void addtail(string name);
    void addhead(string name);
    void delhead();
    void deltail();
    void remove(string name);
    bool search(string name);
    void print() const;
    void load_from_file(const string& filename);
    void save_to_file(const string& filename, bool overwrite) const;
};

ForwardList::ForwardList() : head(nullptr), tail(nullptr) {}

ForwardList::~ForwardList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next_node = current->next;
        delete current;
        current = next_node;
    }
    head = nullptr;
}

bool ForwardList::is_empty() const {
    return head == nullptr;
}

void ForwardList::addtail(string name) {
    Node* newNode = new Node(name);
    if (is_empty()) {
        head = newNode;
        tail = newNode;
        return;
    }
    tail->next = newNode;
    tail = newNode;
}

void ForwardList::addhead(string name) {
    Node* newNode = new Node(name);
    if (is_empty()) {
        head = newNode;
        tail = newNode;
        return;
    }
    newNode->next = head;
    head = newNode;
}

void ForwardList::delhead() {
    if (is_empty()) {
        throw runtime_error("ForwardList is empty");
    }
    if (head == tail) {
        delete tail;
        head = tail = nullptr;
        return;
    }
    Node* node = head;
    head = node->next;
    delete node;
}

void ForwardList::deltail() {
    if (is_empty()) {
        throw runtime_error("ForwardList is empty");
    }
    if (head == tail) {
        delete tail;
        head = tail = nullptr;
        return;
    }
    Node* node = head;
    while (node->next != tail) {
        node = node->next;
    }
    delete tail;
    tail = node;
    tail->next = nullptr;
}

void ForwardList::remove(string name) {
    if (is_empty()) {
        throw runtime_error("ForwardList is empty");
    }
    // Если удаляем голову
    if (head->person == name) {
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        return;
    }
    // Поиск узла с данным значением
    Node* current = head;
    Node* prev = nullptr;
    while (current != nullptr && current->person != name) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Value \"" << name << "\" not found in the list." << endl;
        return;
    }

    if (current == tail) {
        tail = prev;
    }

    if (prev != nullptr) {
        prev->next = current->next;
    }
    delete current;
}




bool ForwardList::search(string name) {
    if (is_empty()) {
        throw runtime_error("ForwardList is empty");
    }

    Node* current = head;
    while (current != nullptr) {
        if (current->person == name) {
            cout << "Value \"" << name << "\" found in the list." << endl;
            return true;
        }
        current = current->next;
    }

    cout << "Value \"" << name << "\" not found in the list." << endl;
    return false;
}

void ForwardList::print() const {
    if (is_empty()) {
        cout << "List is empty" << endl;
        return;
    }
    Node* current = head;

    while (current != nullptr) {
        cout << current->person << " ";
        current = current->next;
    }
    cout << "\n";
}

void ForwardList::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening the file for reading: " << filename << endl;
        return;
    }

    string name;
    while (getline(file, name)) {
        if (!name.empty()) {
            addtail(name);  // Добавление элементов из файла
        }
    }

    file.close();
}

void ForwardList::save_to_file(const string& filename, bool overwrite) const {
    ios_base::openmode mode = overwrite ? ios::trunc : ios::app;
    ofstream file(filename, mode);
    if (!file) {
        cerr << "Error opening the file for writing: " << filename << endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        file << current->person << "\n";
        current = current->next;
    }

    file.close();
}
