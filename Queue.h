#pragma once

#include "library.h"

struct QUEUE {

private:
    struct Node {
        string person;
        Node* next;

        Node(string person) : person(person), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    QUEUE();                              
    ~QUEUE();                             

    bool is_empty();                      
    void push(string name);               
    void pop();                           
    void print();  
    void load_from_file(const string& filename);
    void save_to_file(const string& filename, bool overwrite) const;
};

QUEUE::QUEUE() : head(nullptr), tail(nullptr) {}

QUEUE::~QUEUE() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
}

bool QUEUE::is_empty() {
    return head == nullptr;
}

void QUEUE::push(string name) {
    Node* new_node = new Node(name);

    if (is_empty()) {
        head = tail = new_node;
    }
    else {
        tail->next = new_node;
        tail = new_node;
    }
}

void QUEUE::pop() {
    if (is_empty()) {
        throw runtime_error("QUEUE is empty");
    }

    Node* temp = head;
    head = head->next;
    delete temp;

    if (head == nullptr) {
        tail = nullptr;
    }
}

void QUEUE::print() {
    Node* current = head;
    while (current != nullptr) {
        cout << current->person << " ";
        current = current->next;
    }
    cout << "\n";
}

void QUEUE::load_from_file(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                push(line);
            }
        }
        file.close();
    } else {
        cerr << "Error opening the file for reading. \n";
    }
}

void QUEUE::save_to_file(const string& filename, bool overwrite) const {
    ios_base::openmode mode = overwrite ? ios::trunc : ios::app;
    ofstream file(filename, mode);  
    if (!file) {
        cerr << "Error opening a file for writing: " << filename << endl; 
        return; 
    }

    Node* current = head;
    while (current != nullptr) {
        file << current->person << "\n"; 
        current = current->next;
    }
    file.close();
}