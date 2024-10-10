#include "massive_vec.h"
#include "ForwardList.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "hash.h"

// Node structure for a complete binary tree
struct node {
    int32_t key;
    node* leftChild;
    node* rightChild;

    node(int value) : key(value), leftChild(nullptr), rightChild(nullptr) {}
};

template <typename T>
struct Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
    };

    Node* head;
    Node* tail;

public:
    Queue() : head(nullptr), tail(nullptr) {}

    Queue(const Queue& other) : head(nullptr), tail(nullptr) {
        Node* current = other.head;
        while (current != nullptr) {
            enqueue(current->data);  // We copy the elements one by one
            current = current->next;
        }
    }

    ~Queue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool is_empty() const { return head == nullptr; }

    void enqueue(T data) {
        Node* new_node = new Node(data);
        if (is_empty()) {
            head = tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    void dequeue() {
        if (is_empty()) {
            throw runtime_error("Queue is empty");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    T front() const {
        if (is_empty()) {
            throw runtime_error("Queue is empty");
        }
        return head->data;
    }
};

struct FBT {
private:
    node* root;
    Queue<node*> waitlist;  // Queue for adding new nodes
    Queue<node*> allNodes;  // Queue for storing all nodes

public:
    FBT(int value) {
        root = new node(value);
        allNodes.enqueue(root);
        waitlist.enqueue(root);
    }

    ~FBT() {
        // Freeing all nodes
        Queue<node*> tempQueue = allNodes;
        while (!tempQueue.is_empty()) {
            node* currentNode = tempQueue.front();
            tempQueue.dequeue();
            delete currentNode;  // Deleting the node
        }
    }

    void Add(int value) {
        if (waitlist.is_empty()) {
            throw runtime_error("The parent node was not found to which a new child node can be added!!!");
        }

        node* parent = waitlist.front();  // We take the node that is in the queue
        node* newNode = new node(value);
        allNodes.enqueue(newNode);

        if (parent->leftChild == nullptr) {
            parent->leftChild = newNode;
        }
        else if (parent->rightChild == nullptr) {
            parent->rightChild = newNode;
            waitlist.dequeue(); // If the node has both children, we remove it from the queue
        }
        else {
            throw runtime_error("Parent node already has two children.");
        }

        waitlist.enqueue(newNode);  // Adding a new node to the queue for consideration
    }

    void PrintNodes() {
        Queue<node*> tempQueue = allNodes;
        while (!tempQueue.is_empty()) {
            node* currentNode = tempQueue.front();
            cout << currentNode->key << " ";
            tempQueue.dequeue();
        }
        cout << "\n";
    }

    void PrintTree() {
        int totalNodes = 0;  // The total number of nodes in the tree
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) {
            tempQueue.dequeue();
            totalNodes++;
        }

        int level = 0;  // Current tree level
        int printedNodes = 0;  // The number of nodes already printed
        int maxNodesAtLevel = pow(2, floor(log2(totalNodes)) + 1);

        tempQueue = allNodes;  // Returning the queue to its original state for printing the tree

        while (printedNodes < totalNodes) {
            int nodesInLevel = pow(2, level);
            level++;

            int leadingSpaces = maxNodesAtLevel / nodesInLevel - 1;
            int spacingBetweenNodes = maxNodesAtLevel / nodesInLevel - 1;
            string leadSep(leadingSpaces + 3, ' ');
            string endSep(spacingBetweenNodes, ' ');

            for (int i = 0; i < nodesInLevel && printedNodes < totalNodes; i++) {
                cout << leadSep << tempQueue.front()->key << endSep;
                tempQueue.dequeue();
                printedNodes++;
            }

            cout << "\n";
        }
    }

    node* findPlace(int value) {
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) {
            node* currentNode = tempQueue.front();
            if (currentNode->key == value) {
                return currentNode;
            }
            tempQueue.dequeue();
        }

        throw runtime_error("Value was not found!!!");
        return nullptr;
    }

    bool isFull() {
        Queue<node*> tempQueue = allNodes;

        while (!tempQueue.is_empty()) {
            node* currentNode = tempQueue.front();
            if ((currentNode->leftChild == nullptr && currentNode->rightChild != nullptr) ||
                (currentNode->leftChild != nullptr && currentNode->rightChild == nullptr)) {
                return false;
            }
            tempQueue.dequeue();
        }
        return true;
    }

    string remove_spaces(const string& str);
    void save_new_element_to_file(const string& filename, int key) const;
    void load_from_file(const string& filename);
};

string FBT::remove_spaces(const string& str) {
    string result = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != ' ' && str[i] != '|') {
            result += str[i];
        }
    }
    return result;
}

void FBT::save_new_element_to_file(const string& filename, int key) const {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << "mytree:| " << key << " |\n"; 
        file.close();
    } else {
        cerr << "Error opening the file for writing.\n";
    }
}

void FBT::load_from_file(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            size_t pos = line.find("mytree:|"); 
            if (pos != string::npos) {
                string key_str = line.substr(pos + 8);
                key_str = key_str.substr(0, key_str.find('|')); 
                int key = stoi(key_str);
                Add(key); 
            }
        }
        file.close();
    } else {
        cerr << "Error opening the file for writing.\n";
    }
}


struct user_command{
    string command;
    string name_structure;
    string value_fir = "null";
    string value_sec = "null";
};

struct data_structure {
    string values[100];
    int count;
};

void read_file(const string& filename, data_structure* ds){
    ifstream file(filename);
    if(!file.is_open()){
        cerr << "Could not open the file\n";
        return;
    }
    
    string line;
    ds -> count = 0;
    while(getline(file, line) && ds -> count < 100){
        ds -> values[ds -> count] = line;
        ds -> count++;
    }
    file.close();
}

void parse_command(const string& query, user_command* cmd){
    string current = query;
    size_t pos = current.find(" ");
    if(pos != string::npos){
        cmd -> command = current.substr(0, pos);
        current = current.substr(pos + 1);
        pos = current.find(" ");
        if(pos != string::npos){
            cmd -> name_structure = current.substr(0, pos);
            current = current.substr(pos + 1);
            pos = current.find(" ");
            if(pos != string::npos){
                cmd -> value_fir = current.substr(0, pos);
                current = current.substr(pos + 1);
                cmd -> value_sec = current.substr(0, pos);
            }else{
                cmd -> value_fir = current;
            }
        }else{
            cmd -> name_structure = current;
        }
    }else {
        cmd ->command = current;
    }
}


void process_command(const user_command& cmd, const string& name_file) {
    if (cmd.name_structure == "stack") {
        Stack stack;
        if (cmd.command == "SPUSH") {
            stack.load_from_file(name_file);
            stack.push(cmd.value_fir);
            stack.save_to_file(name_file, true); 
        } else if (cmd.command == "SPOP") {
            stack.load_from_file(name_file);
            stack.pop();
            stack.save_to_file(name_file, true); 
        } else if (cmd.command == "SPRINT") {
            stack.load_from_file(name_file);
            stack.print();
        } 
    } else if (cmd.name_structure == "queue") {
        QUEUE queue; 
        if (cmd.command == "QPUSH") {
            queue.load_from_file(name_file);
            queue.push(cmd.value_fir);
            queue.save_to_file(name_file, true); 
        } else if (cmd.command == "QPOP") {
            queue.load_from_file(name_file);
            queue.pop();
            queue.save_to_file(name_file, true); 
        } else if (cmd.command == "QPRINT") {
            queue.load_from_file(name_file);
            queue.print();
        } 
    } else if (cmd.name_structure == "massive") {
        Myvector<string> vector; 
        if (cmd.command == "MPUSH") {
            vector.load_from_file(name_file);  
            if (cmd.value_sec != "null") {
                int index = stoi(cmd.value_sec);  
                vector.MPUSH(cmd.value_fir, index);  
            } else {
                vector.MPUSH(cmd.value_fir);  
            }
            vector.save_to_file(name_file, true);  
        } else if (cmd.command == "MDEL") {
            vector.load_from_file(name_file);    
            vector.MDEL(stoi(cmd.value_fir));    
            vector.save_to_file(name_file, true); 
        } else if (cmd.command == "MGET") {
            vector.load_from_file(name_file);    
            cout << vector.MGET(stoi(cmd.value_fir)) << endl; 
        } else if (cmd.command == "MSET") {
            vector.load_from_file(name_file);  
            int index = stoi(cmd.value_sec);  
            vector.MSET(cmd.value_fir, index); 
            vector.save_to_file(name_file, true);  
        }else if (cmd.command == "SIZE") {
            vector.load_from_file(name_file);    
            cout << vector.size() << endl;       
        } else if (cmd.command == "MPRINT") {
            vector.load_from_file(name_file);    
            vector.print();                      
        }  
    }else if (cmd.name_structure == "flist") {

        ForwardList flist; 
        flist.load_from_file(name_file); 
        if (cmd.command == "ADDHEAD") {
            flist.addhead(cmd.value_fir); 
        } else if (cmd.command == "ADDTAIL") {
            flist.addtail(cmd.value_fir);  
        } else if (cmd.command == "DELHEAD") {
            flist.delhead();  
        } else if (cmd.command == "DELTAIL") {
            flist.deltail();  
        } else if (cmd.command == "FREMOVE") {
            flist.remove(cmd.value_fir);  
        } else if (cmd.command == "FSEARCH") {
            flist.search(cmd.value_fir); 
        } else if (cmd.command == "FPRINT") {
            flist.print();  
        }

        flist.save_to_file(name_file, true);  
    }else if (cmd.name_structure == "llist") {

        LinkedList llist; 
        llist.load_from_file(name_file); 
        if (cmd.command == "LADDHEAD") {
            llist.addhead(cmd.value_fir); 
        } else if (cmd.command == "LADDTAIL") {
            llist.addtail(cmd.value_fir);  
        } else if (cmd.command == "LDELHEAD") {
            llist.delhead();  
        } else if (cmd.command == "LDELTAIL") {
            llist.deltail();  
        } else if (cmd.command == "LREMOVE") {
            llist.remove(cmd.value_fir);  
        } else if (cmd.command == "LSEARCH") {
            llist.search(cmd.value_fir); 
        } else if (cmd.command == "LPRINT") {
            llist.print();  
        }

        llist.save_to_file(name_file, true); 
    }else if (cmd.name_structure == "hash") {
        Hashtable hashtableInstance(25);  
        if (cmd.command == "HADD") {
            hashtableInstance.load_from_file(name_file);
            hashtableInstance.add(cmd.value_fir, cmd.value_sec);
            hashtableInstance.save_to_file(name_file, true); 
        } else if (cmd.command == "HGET") {
            hashtableInstance.load_from_file(name_file);
            try {
                string value = hashtableInstance.get(cmd.value_fir);
                cout << "Value: " << value << endl;
            } catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "HREMOVE") {
            hashtableInstance.load_from_file(name_file);
            try {
                hashtableInstance.remove(cmd.value_fir);
                hashtableInstance.save_to_file(name_file, true); 
            } catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "HPRINT") {
            hashtableInstance.load_from_file(name_file);
            hashtableInstance.print();
        } 
    }else if (cmd.name_structure == "tree") {
        FBT treeInstance(15);  
        if (cmd.command == "TADD") {
            treeInstance.load_from_file(name_file);
            treeInstance.Add(stoi(cmd.value_fir));
            treeInstance.save_new_element_to_file(name_file, stoi(cmd.value_fir));
        } else if (cmd.command == "TPRINT") {
            treeInstance.load_from_file(name_file);
            treeInstance.PrintNodes();
        } else if (cmd.command == "TREEPRINT") {
            treeInstance.load_from_file(name_file);
            treeInstance.PrintTree();
        } else if (cmd.command == "TFIND") {
            treeInstance.load_from_file(name_file);
            try {
                node* foundNode = treeInstance.findPlace(stoi(cmd.value_fir));
                cout << "Node found: " << foundNode->key << endl;
            } catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "TFULL") {
            treeInstance.load_from_file(name_file);
            bool isFull = treeInstance.isFull();
            cout << "Tree is " << (isFull ? "full" : "not full") << endl;
        }
    }   
}

int main(int argc, char *argv[]){
    try {
        string name_file = argv[2];
        string query = argv[4];

        user_command cmd;
        parse_command(query, &cmd);
        process_command(cmd, name_file);
    }
    catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}