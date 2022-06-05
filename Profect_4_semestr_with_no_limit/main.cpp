//
//  main.cpp
//  Profect_4_semestr_with_no_limit
//
//  Created by Егор Сипачев on 25.05.2022.
//

#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>
#include <thread>
#define max_int 1000000
using namespace std;
#define stock_memory 100 //определили максимальную оперативную память

int Random_Num() {
    return rand()%max_int;
}

int GetFileSize(char file_name[]) {
    int sizeoffile = 0;
    ifstream in_stream(file_name, ios::binary);
    while(!in_stream.eof()) {
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof( p));
        sizeoffile++;
    }
    sizeoffile--;
    in_stream.close();
    return sizeoffile;
}

void CreateFile(char file_name[], int sizeoffile) {
    ofstream out_stream(file_name, ios::binary);
    for (int i = 0; i < sizeoffile; i++) {
        int p = rand()%10000;
        out_stream.write(reinterpret_cast <char*>(&p), sizeof(p));
    }
    out_stream.close();
}

void PrintVector(vector<int> vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }
}

void PrintVector(vector<char*> vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << endl;
    }
}


void WriteVecInFile(char *file_name, vector<int> nums) {
    //ofstream out_stream(file_name, ios::binary);
    FILE* f;
    f = fopen(file_name,"wb");
    for (int i = 0; i < nums.size(); i++) {
        int p = nums[i];
        fwrite(&p, sizeof(p), 1, f);
    }
    fclose(f);
}

void ReadFileInVector(char file_name[], vector<int> &vec) {
    ifstream in_stream(file_name, ios::binary);
    int sizeoffile = GetFileSize(file_name);
    for (int i = 0; i < sizeoffile; i++) {
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof(p));
        vec[i] = p;
    }
    in_stream.close();
}

void PrintFile(char filename[]) {
    ifstream in_stream(filename, ios::binary);
    int sizeoffile = GetFileSize(filename);
    for (int i = 0; i < sizeoffile; i++){
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof(p));
        cout << p << endl;
    }
    in_stream.close();
}

void SortDevidedVector(vector<int> &vectorbuf, char* filename) { //Принимает на вход вектор, имя файла и записывает туда отсортированный
    sort(vectorbuf.begin(), vectorbuf.end());
    WriteVecInFile(filename, vectorbuf);
}

char* CreateNameOfFile(int i) {
    char stroka[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Profect_4_semestr_with_no_limit/File";
    char str[] = " ";
    sprintf(str,"%d", i);
    char dat[] = ".dat";
    strcat(stroka, str);
    strcat(stroka, dat);
    //cout << stroka << endl;
    unsigned long len = strlen(stroka);
    char* stroka_real = new char(len + 1);
    stroka_real[len] = '\0';
    strcpy(stroka_real, stroka);
    return stroka_real;
}

int GetFirstFromFile(char filename[],int pointer) {
    ifstream in_stream(filename, ios::binary);
    int p = 100000000;
    //fseek(in_stream, pointer*sizeof(p), SEEK_SET);
    in_stream.seekg(pointer*sizeof(p), ios::beg);
    if (not in_stream.eof()) {
        in_stream.read(reinterpret_cast<char*>(&p), sizeof(p));
        in_stream.close();
        return p;
    }
    else {
        in_stream.close();
        p = 100000000;
        return p;
    }
}

bool CheckForMillion(vector<int> nums) {
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] != 100000000) {
            return false;
        }
    }
    return true;
}

int main() {
    char filename[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Profect_4_semestr_with_no_limit/Data.dat";
    //char path[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Project_semestr_4";
    //1) Нам нужно взять какой то размер файла за эталон (в котором удобно работать) допустим 50 байт
    //2) Нам нужно определить размер текущего файла (в байтах)
    //3) Нам нужно поделить GlobalSize на эталонный
    //4) Получим количество формируемых файлов AmountMaxOfFiles
    //unsigned int amount_max_of_threads = thread::hardware_concurrency();
    ofstream outstream;
    int amount_of_nums = 1000;
    int ideal_filesize = 50;
    int amount_of_files = 0;
    if (amount_of_nums%ideal_filesize == 0) {
        amount_of_files = amount_of_nums/ideal_filesize;
    }
    else {
        amount_of_files = amount_of_nums/ideal_filesize + 1;
    }
    CreateFile(filename, amount_of_nums);
    int bufsize = GetFileSize(filename);
    vector<int> lengths(amount_of_files);
    int type_length = 0;
    int last_length = 0;
    type_length  = bufsize/(amount_of_files);
    last_length = type_length + bufsize%(amount_of_files);
    cout << type_length << endl;
    cout << last_length << endl;
    for (int i = 0; i < amount_of_files - 1; i++) {
        lengths[i] = type_length;
    }
    lengths[amount_of_files - 1] = last_length;
    ifstream in_stream(filename, ios::binary);
    vector<char*> filenames;
    for(int i = 0; i < amount_of_files; i++) {
        vector<int> vector_read_buf(lengths[i]);
        for(int j = 0; j < lengths[i]; j++) {
            int p;
            in_stream.read(reinterpret_cast<char*>(&p),sizeof(p));
            vector_read_buf[j] = p;
        }
        //сформировать имя файла
        char* namefile = CreateNameOfFile(i);
        filenames.push_back(namefile);
        cout << i << endl;
        cout << namefile << endl;
        //SortDevidedVector(vector_read_buf, namefile);
        thread t(SortDevidedVector, ref(vector_read_buf), (namefile));
        t.join();
        // запустить поток с данным вектором и данным именем файла
        //переход к новому циклу
    }
    in_stream.close();
    PrintVector(filenames);
    vector<int> buf_nums(amount_of_files);
    for (int i = 0; i < amount_of_files; i++) {
        buf_nums[i] = 0;
    }
    for (int i = 0; i < amount_of_files; i++) {
        cout << "File" << i << " Ниже" << endl;
        PrintFile(filenames[i]);
    }
    vector<int> index_of_place(amount_of_files);
    vector<bool> flags(amount_of_files);
    vector<int> indexes_of_files;
    //здесь будет большой цикл
    for (int i = 0; i < amount_of_files; i++) {
        flags[i] = true;
        index_of_place[i] = -1;
    }
    FILE *fp;
    fp = fopen("/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Profect_4_semestr_with_no_limit/OutData.dat", "wb"); //открыли файл для записи бинарной
   while (not CheckForMillion(buf_nums)) {
        for (int i = 0; i < amount_of_files; i++) {
            if (flags[i] == true) {
                index_of_place[i]++;
                buf_nums[i] = GetFirstFromFile(filenames[i],index_of_place[i]);
            }
        }
        int minimum = buf_nums[0];
        int imin = 0;
        for (int i = 1; i < amount_of_files; i++) {
            if (buf_nums[i] < minimum) {
                minimum = buf_nums[i];
                imin = i;
            }
        }
        //после цикла мы знаем минимальный и номер минимального
        cout << minimum << "номер минимального: " << imin << endl;
        for (int i = 0; i < amount_of_files; i++) {
            flags[i] = false;
        }
        flags[imin] = true;
       if (minimum != 100000000) {
        fwrite(&buf_nums[imin], sizeof(int), 1, fp);
       }
        cout << "Числа из вектора" << endl;
        PrintVector(buf_nums);
    }
    fclose(fp);
    cout << "Итоговый файл: _________" << endl;
    PrintFile("/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Profect_4_semestr_with_no_limit/OutData.dat");
    //Алгоритм в общем случае сортировка:
    //1) Считываем в цикле первые записи из файлов в вектор
    //2) Определяем из них минимальный
    //3) Не меняя индексов в тех файлах откуда мы не перезаписывали, меняем индекс только у того, который минимальный
    //4) массив фалгов, меняем на true только на том индексе, соответствующем файлу, из которого выбрали минимальный на каждой итерации
    //5) минимальное записать в большой файл
    
    /*int length1 = 0;
    int length2 = 0;
    if (bufsize%2 == 0) {
        //length1 = bufsize/2;
        length2 = length1 = bufsize/2;
    }
    else {
        length1 = bufsize/2 + 1;
        length2 = bufsize - length1;
    }
    //cout << length1 << " " << length2 << endl;
    vector<int> vectorbuf(length1); //
    //vector<int> vec2(length2);
    //ifstream in_stream(filename, ios::binary);
    for (int i = 0; i < length1; i++) {
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof( p));
        vectorbuf[i] = p;
    }
    /*for (int i = 0; i < length2; i++) {
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof( p));
        vec2[i] = p;
    }*/
    //sort(vectorbuf.begin(), vectorbuf.end());
    /*PrintVector(vectorbuf); //этот отсортированный вектор записать в файл
    char file1[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Project_semestr_4/File1.dat";
    thread t1(SortDevidedVector, ref(vectorbuf), ref(file1));
    t1.join();*/
    //SortDevidedVector(vectorbuf, file1);
    //WriteVecInFile(file1, vectorbuf);
    /*vector<int> vec_out(length1);
    ReadFileInVector(file1, vec_out);
    PrintVector(vec_out);
    cout << "------------" <<endl;*/
    /*vectorbuf.resize(length2);
    for (int i = 0; i < length2; i++) {
        int p;
        in_stream.read(reinterpret_cast <char*>(&p), sizeof( p));
        vectorbuf[i] = p;
    }*/
    //sort(vectorbuf.begin(), vectorbuf.end());
    /*char file2[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Project_semestr_4/File2.dat";
    thread t2(SortDevidedVector, ref(vectorbuf), ref(file2));
    t2.join();*/
    //SortDevidedVector(vectorbuf, file2);
    //WriteVecInFile(file2, vectorbuf);
    //Алгоритм слияния:
    //1) Считываем всегда первые числа из файлов
    //2) Сравниваем
    //3) Записываем наименьшее в третий файл
    //4) Из файла в котором большее число не производим считывание (чтобы не смещать индекс)
    //5) 1 - 4 пока не закончатся файлы
    /*char file3[] = "/Users/egorsipachev/Desktop/STUDY/МГУ УЧЕБА/Project_semestr_4/File3.dat";
    ofstream out3(file3, ios::binary);
    ifstream in1(file1, ios::binary);
    ifstream in2(file2, ios::binary);
    int n1 = 0;
    int n2 = 0;
    //флаги - признак того, что было считано (n1 или n2)
    bool flag1 = true;
    bool flag2 = true; //(добавить сравнение длин)
    while((!in1.eof()) && (!in2.eof())) {
        if (flag1 == true) {
            in1.read(reinterpret_cast<char*>(&n1),sizeof(n1));
        }
        if (flag2 == true) {
            in2.read(reinterpret_cast<char*>(&n2),sizeof(n2));
        }
        if (in1.eof() || in2.eof()) {
            break;
        }
        cout << "1oe: " <<n1 << " 2oe: " << n2 << endl;
        if (n1 > n2) {
            out3.write(reinterpret_cast <char*>(&n2), sizeof(n2));
            flag2 = true;
            flag1 =false;
        }
        else {
            out3.write(reinterpret_cast <char*>(&n1), sizeof(n1));
            flag1 = true;
            flag2 = false;
        }
    }
    in1.close();
    in2.close();
    out3.close();
    cout << "File3 ниже ->" << endl;
    PrintFile(file3);
    cout << "Длина третьего файла: ";
    cout << GetFileSize(file3) << endl;*/
    return 0;
}


