#pragma once

#include "massive_vec.h"
#include <cstdint>

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
        uint32_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % size;
    }

public:
    Hashtable(int size); 
    ~Hashtable();        

    void add(const string& key, const string& value); 
    string get(const string& key);                      
    void remove(const string& key);                    
    void print();     
    void load_from_file(const string& filename, const string& name_structure);
    void save_to_file(const string& filename, const string& name_structure) const;
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

// void Hashtable::add(const string& key, const string& value) {
//     int index = hashfunction(key);
//     Node* prev = nullptr;
//     Node* current = table[index];

//     while (current != nullptr) { // we go through the chain
//         if (current->key == key) {
//             current->value = value;
//             return;
//         }
//         prev = current;
//         current = current->next;
//     }

//     Node* new_node = new Node(key, value);

//     if (prev == nullptr) {
//         table[index] = new_node;
//     }
//     else {
//         prev->next = new_node; // Node to the end
//     }
// }



void Hashtable::add(const string& key, const string& value) {
    int index = hashfunction(key);
    Node* current = table[index];


    // Проходим по цепочке, чтобы найти существующий ключ
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value; // Обновляем значение, если ключ уже существует
            return;
        }
        current = current->next;
    }

    // Если ключ не найден, создаем новый узел и добавляем его в начало цепочки
    Node* new_node = new Node(key, value);
    new_node->next = table[index];
    table[index] = new_node;
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



void Hashtable::load_from_file(const string& filename, const string& name_structure) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error opening the file for reading.\n";
        return;
    }

    string line;
    bool structure_found = false;

    while (getline(file, line)) {
        // Проверяем, является ли строка началом структуры хеш-таблицы
        if (line.find(name_structure + " : ") != string::npos) {
            structure_found = true; // Нашли хеш-таблицу с указанным именем
            size_t pos = line.find(':');
            string values = line.substr(pos + 1); 
            stringstream ss(values);
            string pair;

            // Парсим ключ-значение, разделенные запятыми
            while (getline(ss, pair, ',')) {
                size_t start = pair.find('[');
                size_t middle = pair.find(':');
                size_t end = pair.find(']');

                if (start != string::npos && middle != string::npos && end != string::npos) {
                    string key = pair.substr(start + 1, middle - start - 1);  // Получаем ключ
                    string value = pair.substr(middle + 2, end - middle - 2); // Получаем значение

                    // Добавляем пару ключ-значение в хеш-таблицу
                    add(key, value);
                }
            }
            break;
        }
    }

    file.close();

}


void Hashtable::save_to_file(const string& filename, const string& name_structure) const {
    // Читаем существующие данные из файла
    ifstream read_file(filename);
    Myvector<string> lines;
    string line;
    bool structure_found = false;

    if (read_file.is_open()) {
        while (getline(read_file, line)) {
            lines.MPUSH(line);
        }
        read_file.close();
    } else {
        cerr << "Error opening the file for reading.\n";
        return;
    }

    ofstream write_file(filename);
    if (!write_file.is_open()) {
        cerr << "Ошибка открытия файла для записи.\n";
        return;
    }

    // Проверяем наличие структуры с именем name_structure
    for (auto& existing_line : lines) {
        if (existing_line.find(name_structure + " : ") == 0) {
            structure_found = true;

            // Заменяем строку структуры новыми значениями
            write_file << name_structure << " : ";
            
            // Проходим по всем элементам хеш-таблицы
            bool first_entry = true;
            for (int i = 0; i < size; ++i) {
                Node* current = table[i];
                while (current != nullptr) {
                    if (!first_entry) {
                        write_file << ", "; // Добавляем разделитель
                    }
                    write_file << "[" << current->key << ": " << current->value << "]";
                    first_entry = false;
                    current = current->next;
                }
            }
            write_file << endl;
        } else {
            write_file << existing_line << endl; // Сохраняем остальные строки
        }
    }

    // Если структура не найдена, добавляем её в конец файла
    if (!structure_found) {
        write_file << name_structure << " : ";
        bool first_entry = true;
        for (int i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                if (!first_entry) {
                    write_file << ", "; // Добавляем разделитель
                }
                write_file << "[" << current->key << ": " << current->value << "]";
                first_entry = false;
                current = current->next;
            }
        }
        write_file << endl;
    }

    write_file.close();
}


