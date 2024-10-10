#pragma once

#include "library.h"

struct LinkedList {

private:
    struct Node {
        string person;
        Node* next;
        Node* prev;

        Node(string person) : person(person), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    LinkedList();                          
    ~LinkedList();                         

    bool is_empty();                       
    void addhead(string name);             
    void addtail(string name);             
    void delhead();                        
    void deltail();                        
    void remove(string name);              
    void search(string name);              
    void print();   
    void save_to_file(const string& filename, bool overwrite) const;
    void load_from_file(const string& filename);       
};


LinkedList::LinkedList() : head(nullptr), tail(nullptr) {}

LinkedList::~LinkedList() {
    Node* current = head;

    while (current != nullptr) {
        Node* next_node = current->next;
        delete current;
        current = next_node;
    }
    head = tail = nullptr;
}

bool LinkedList::is_empty() {
    return head == nullptr;
}

void LinkedList::addhead(string name) {
    Node* newnode = new Node(name);

    if (head == nullptr) {
        head = newnode;
        tail = newnode;
    }
    else {
        newnode->next = head;
        head->prev = newnode;
        head = newnode;
    }
}

void LinkedList::addtail(string name) {
    Node* newnode = new Node(name);

    if (head == nullptr) {
        head = newnode;
        tail = newnode;
    }
    else {
        tail->next = newnode;
        newnode->prev = tail;
        tail = newnode;
    }
}

void LinkedList::delhead() {
    if (is_empty()) {
        throw runtime_error("LinkedList is empty");
    }
    Node* next_node = head->next;

    if (next_node != nullptr) {
        next_node->prev = nullptr;
    }
    else {
        tail = nullptr;
    }

    delete head;
    head = next_node;
}

void LinkedList::deltail() {
    if (is_empty()) {
        throw runtime_error("LinkedList is empty");
    }

    Node* prev_node = tail->prev;

    if (prev_node != nullptr) {
        prev_node->next = nullptr;
    }
    else {
        head = nullptr;
    }

    delete tail;
    tail = prev_node;
}


void LinkedList::remove(string name) {
    if (is_empty()) {
        throw runtime_error("LinkedList is empty");
    }

    Node* current = head;
    bool found = false;

    while (current != nullptr) {
        if (current->person == name) {
            if (current == head) {
                head = current->next;
                if (head != nullptr) {
                    head->prev = nullptr;
                } else {
                    tail = nullptr;
                }
            } else if (current == tail) {
                tail = current->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next; // The next node for the previous one
                current->next->prev = current->prev; // The previous node for the next one
            }

            delete current; // Deleting the node
            found = true;
            break;
        }
        current = current->next;
    }

    if (!found) {
        cout << "Element \"" << name << "\" not found in LinkedList!!!" << "\n";
    }
}



void LinkedList::search(string name) {
    if (is_empty()) {
        throw runtime_error("LinkedList is empty");
    }

    Node* current = head;

    while (current != nullptr) {
        if (current->person == name) {
            cout << "Element \"" << current->person << "\" was found in LinkedList!!!" << "\n";
            return;
        }
        current = current->next;
    }
    throw runtime_error("Element not found in LinkedList!!!");
}

void LinkedList::print() {
    Node* current = head;
    while (current != nullptr) {
        cout << current->person << " ";
        current = current->next;
    }
    cout << "\n";
}

void LinkedList::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening the file for reading: " << filename << endl;
        return;
    }

    string name;
    while (getline(file, name)) {
        if (!name.empty()) {
            addtail(name);  // Добавляем элемент в конец списка
        }
    }

    file.close();
}

void LinkedList::save_to_file(const string& filename, bool overwrite) const {
    ios_base::openmode mode = overwrite ? ios::trunc : ios::app;
    ofstream file(filename, mode);
    if (!file) {
        cerr << "Error opening the file for writing: " << filename << endl;
        return;
    }

    Node* current = head;
    while (current != nullptr) {
        file << current->person << "\n";  // Записываем данные в файл
        current = current->next;
    }

    file.close();
}