template<typename T>
void Myvector<T>::save_to_file(const string& filename, bool overwrite) const {
    ios_base::openmode mode = overwrite ? ios::trunc : ios::app;
    ofstream file(filename, mode);  // Открываем файл для перезаписи или добавления
    if (!file) {
        cerr << "Error opening a file for writing: " << filename << endl; 
        return; 
    }

    for (int i = 0; i < size_of_Vec; i++) {
        file << "myvector:| " << arr[i] << " |\n"; 
    }
    file.close();
}
