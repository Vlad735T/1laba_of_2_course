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
    void load_from_file(const string& filename, const string& name_structure);
    void save_to_file(const string& filename, const string& name_structure) const;
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


void ForwardList::load_from_file(const string& filename, const string& name_structure) {
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
                value.erase(0, value.find_first_not_of(" \t"));  // Удаляем пробелы перед значениями
                addtail(value);  
            }
            break;  
        }
    }


    file.close();
}


void ForwardList::save_to_file(const string& filename, const string& name_structure) const {
    ifstream read_file(filename);
    ForwardList lines;
    string line;
    bool structure_found = false;

    // Читаем файл построчно и сохраняем все строки во временный список
    if (read_file.is_open()) {
        while (getline(read_file, line)) {
            lines.addtail(line);
        }
        read_file.close();
    } else {
        cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    // Открываем файл для записи (перезапись)
    ofstream write_file(filename);
    if (!write_file.is_open()) {
        cerr << "Ошибка открытия файла для записи.\n";
        return;
    }

    Node* current = lines.head;

    while (current != nullptr) {
        string& existing_line = current->person;
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
                write_file << data_node->person;
                first = false;
                data_node = data_node->next;
            }
            write_file << endl;
        } else {
            // Записываем все остальные строки как есть
            write_file << existing_line << endl;
        }
        current = current->next;
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
            data_node = data_node->next;
        }
        write_file << endl;
    }

    write_file.close();
}

