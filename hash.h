#pragma once

#include "library.h"

using namespace std;

struct Node {
    string key;
    string value;
    Node* next;

    Node(const string& key, const string& value) : key(key), value(value), next(nullptr) {}
};

struct Hashtable {
private:
    Node** table; // An array of pointers to the beginning of the chains
    int size;

    int hashfunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % size; // We use the size of the table
    }

public:
    Hashtable(int size); 
    ~Hashtable();        

    void add(const string& key, const string& value); 
    string get(const string& key);                      
    void remove(const string& key);                    
    void print();     
    void save_new_element_to_file(const string& filename, const string& key, const string& value) const;
    void load_from_file(const string& filename);       
    void save_to_file(const string& filename, bool overwrite) const;                 
};


Hashtable::Hashtable(int size) : size(size) {
    table = new Node * [size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

Hashtable::~Hashtable() {
    for (int i = 0; i < size; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* prev = current;
            current = current->next;
            delete prev;
        }
    }
    delete[] table;
}

void Hashtable::add(const string& key, const string& value) {
    int index = hashfunction(key);
    Node* prev = nullptr;
    Node* current = table[index];

    while (current != nullptr) { // we go through the chain
        if (current->key == key) {
            current->value = value;
            return;
        }
        prev = current;
        current = current->next;
    }

    Node* new_node = new Node(key, value);

    if (prev == nullptr) {
        table[index] = new_node;
    }
    else {
        prev->next = new_node; // Node to the end
    }
}

string Hashtable::get(const string& key) {
    int index = hashfunction(key);

    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    throw runtime_error("The key was not found");
}

void Hashtable::remove(const string& key) {
    int index = hashfunction(key);
    Node* current = table[index];
    Node* prev = nullptr;

    while (current != nullptr) { // We go through the chain and look for an item to delete
        if (current->key == key) {
            if (prev == nullptr) {
                table[index] = current->next; // If the first in the chain
            }
            else {
                prev->next = current->next;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
    throw runtime_error("The key was not found");
}

void Hashtable::print() {
    for (int i = 0; i < size; ++i) {
        cout << i << ": ";
        Node* current = table[i];
        if (current != nullptr) {
            while (current != nullptr) {
                cout << "[" << current->key << ": " << current->value << "]";
                current = current->next;
                if (current != nullptr) {
                    cout << " -> ";
                }
            }
        }
        cout << "\n";
    }
}

void Hashtable::save_new_element_to_file(const string& filename, const string& key, const string& value) const {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << key << ":" << value << "\n"; 
        file.close();
    } else {
        cerr << "Error opening the file for writing.\n";
    }
}

void Hashtable::load_from_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening the file for reading: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string key = line.substr(0, colon_pos); 
                string value = line.substr(colon_pos + 1); 
                add(key, value); 
            }
        }
    }

    file.close();
}

void Hashtable::save_to_file(const string& filename, bool overwrite) const {
    ios_base::openmode mode = overwrite ? ios::trunc : ios::app;
    ofstream file(filename, mode);
    if (!file) {
        cerr << "Error opening the file for writing: " << filename << endl;
        return;
    }

    for (int i = 0; i < size; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            file << current->key << ":" << current->value << "\n";
            current = current->next;
        }
    }

    file.close();
}