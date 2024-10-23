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
    void load_from_file(const string& filename, const string& name_structure);
    void save_to_file(const string& filename, const string& name_structure) const;
    
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

void LinkedList::load_from_file(const string& filename, const string& name_structure) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    string line;
    bool structure_found = false;

    while (!is_empty()) {
        delhead();
    }

    while (getline(file, line)) {
        // Проверяем, является ли строка началом новой структуры
        if (line.find(name_structure + " : ") != string::npos) {
            structure_found = true; // Найдена структура с указанным именем
            size_t pos = line.find(':');
            string values = line.substr(pos + 1); 
            stringstream ss(values);
            string value;

            // Парсим значения, разделенные запятыми, и добавляем их в список
            while (getline(ss, value, ',')) {
                value.erase(0, value.find_first_not_of(" \t"));  
                addtail(value);  
            }
            break;  
        }
    }

    file.close();
}


void LinkedList::save_to_file(const string& filename, const string& name_structure) const {
    ifstream read_file(filename);
    LinkedList lines;  
    string line;
    bool structure_found = false;

    if (read_file.is_open()) {
        while (getline(read_file, line)) {
            lines.addtail(line);
        }
        read_file.close();
    } else {
        cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    ofstream write_file(filename);
    if (!write_file.is_open()) {
        cerr << "Ошибка открытия файла для записи.\n";
        return;
    }

    Node* current = lines.head;

    while (current != nullptr) {
        string& existing_line = current->person;  // Получаем строку из временного списка
        if (existing_line.find(name_structure + " : ") == 0) {
            structure_found = true;

            // Заменяем строку структуры новыми значениями из списка
            write_file << name_structure << " : ";
            Node* data_node = head;  
            bool first = true;
            while (data_node != nullptr) {
                if (!first) {
                    write_file << ", "; 
                }
                write_file << data_node->person;  // Записываем элемент
                first = false;
                data_node = data_node->next; 
            }
            write_file << endl; 
        } else {
            // Записываем все остальные строки как есть
            write_file << existing_line << endl;
        }
        current = current->next;  // Переходим к следующему элементу во временном списке
    }

    // Если структура не найдена, добавляем её в конец файла
    if (!structure_found) {
        write_file << name_structure << " : ";
        Node* data_node = head;  
        bool first = true;
        while (data_node != nullptr) {
            if (!first) {
                write_file << ", "; 
            }
            write_file << data_node->person; 
            first = false;
            data_node = data_node->next;  // Переходим к следующему элементу
        }
        write_file << endl; 
    }

    write_file.close();  
}

