#include "massive_vec.h"
#include "ForwardList.h"
#include "LinkedList.h"
#include "Queue.h"
#include "Stack.h"
#include "hash.h"
#include "FBT.h"


struct user_command{
    string command;
    string name_structure;
    string value_fir = "null";
    string value_sec = "null";
};


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
                cmd -> value_sec = current;
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


void process_command(const user_command& cmd, const string& name_file, const string& structure_name) { 

    if (structure_name == "massive") {

    Myvector<string> vector;  
    vector.load_from_file(name_file, cmd.name_structure);
        if (cmd.command == "MPUSH") {

            if (cmd.value_sec != "null") {
                int index = stoi(cmd.value_sec);
                vector.MPUSH(cmd.value_fir, index);
            } else {
                vector.MPUSH(cmd.value_fir);
            }
            vector.save_to_file(name_file, cmd.name_structure);  
        } else if (cmd.command == "MDEL") {

            int index = stoi(cmd.value_fir);
            vector.MDEL(index);
            vector.save_to_file(name_file, cmd.name_structure); 
        } else if (cmd.command == "MGET") {

            int index = stoi(cmd.value_fir);
            cout << vector.MGET(index) << endl; 
        } else if (cmd.command == "MSET") {

            int index = stoi(cmd.value_sec);
            vector.MSET(cmd.value_fir, index); 
            vector.save_to_file(name_file, cmd.name_structure);  
        } else if (cmd.command == "SIZE") {

            cout << vector.size() << endl;       
        } else if (cmd.command == "MPRINT") {
            vector.print();                      
        }  
    }
    else if (structure_name == "flist") {

        ForwardList flist;  
        flist.load_from_file(name_file, cmd.name_structure);
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
        flist.save_to_file(name_file, cmd.name_structure);  
    }
    else if (structure_name == "llist") {

        LinkedList llist; 
        llist.load_from_file(name_file, cmd.name_structure);
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
        llist.save_to_file(name_file, cmd.name_structure);  
    }
    else if (structure_name == "queue") { 

        QUEUE queue; 
        queue.load_from_file(name_file, cmd.name_structure);
        if (cmd.command == "QPUSH") {
            queue.push(cmd.value_fir);
            queue.save_to_file(name_file, cmd.name_structure); 

        } else if (cmd.command == "QPOP") {
            queue.pop();
            queue.save_to_file(name_file, cmd.name_structure); 
        } else if (cmd.command == "QPRINT") {
            queue.print();
        } 
    }
    else if (structure_name == "stack") {

        Stack stack;
        stack.load_from_file(name_file, cmd.name_structure);

        if (cmd.command == "SPUSH") {

            stack.push(cmd.value_fir);
            stack.save_to_file(name_file, cmd.name_structure);
        } else if (cmd.command == "SPOP") {

            stack.pop();
            stack.save_to_file(name_file, cmd.name_structure);
        } else if (cmd.command == "SPRINT") {
            stack.print();
        } 
    }
    else if (structure_name == "hash") {

        Hashtable hash(25);  
        hash.load_from_file(name_file, cmd.name_structure);

        if (cmd.command == "HADD") {
            
            hash.add(cmd.value_fir, cmd.value_sec);
            hash.save_to_file(name_file, cmd.name_structure);
        } else if (cmd.command == "HGET") {

            try {
                string value = hash.get(cmd.value_fir);
                cout << "Value: " << value << endl;
            } catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "HREMOVE") {

            try {
                hash.remove(cmd.value_fir);
                hash.save_to_file(name_file, cmd.name_structure);
            } catch (const runtime_error& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "HPRINT") {
            hash.print();
        } 
    }
    else if (structure_name == "FBT") {

        FBT tree(15);  
        tree.load_from_file(name_file, cmd.name_structure);

        if (cmd.command == "TADD") {

            tree.Add(stoi(cmd.value_fir));
            tree.save_to_file(name_file, cmd.name_structure);
        } else if (cmd.command == "TPRINT") {

            tree.PrintNodes();
        } else if (cmd.command == "TREEPRINT") {

            tree.PrintTree();
        } else if (cmd.command == "TFIND") {

            try {
                node* foundNode = tree.findPlace(stoi(cmd.value_fir));
                cout << "Node found: " << foundNode->key << endl;
            } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
            }
        } else if (cmd.command == "TFULL") {

            bool isFull = tree.isFull();
            cout << "Tree is " << (isFull ? "full" : "not full") << endl;
        }
    }   

}


string extract_structure(const string& filename){
    size_t pos = filename.find(".");
    if(pos == string::npos){
        throw runtime_error("Error: Invalid file name. The format is expected file.structure.");
    }
    return filename.substr(pos + 1);
}


bool check_structure(const string& structure_name) {
    Myvector<string> structure{ "massive", "llist", "flist", "queue", "stack", "hash", "FBT" };
    bool found = false;

    for (size_t i = 0; i < structure.size(); i++) {
        string check = structure.MGET(i);

        if (structure_name == check) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Unknown structure: " << structure_name << "!\n";
        return false;
    }
    return true;
}


int main(int argc, char *argv[]){
    try {

        if (argc != 5) {
            throw runtime_error("The number of arguments is incorrect. Use: ./main --file <filename> --query '<command>'");
        }

        string name_file = argv[2];
        string query = argv[4];

        string structure_name = extract_structure(name_file);

        if(!check_structure(structure_name)){
            cout << "Structure " << structure_name << " not found.\n";
            return 0;
        }
        
        user_command cmd;
        parse_command(query, &cmd);
        process_command(cmd, name_file, structure_name);
    }
    catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
