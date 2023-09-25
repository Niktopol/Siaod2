#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct patient{
    int cardNum;
    int ind;
    patient(int cardNum, int ind){
        this->cardNum = cardNum;
        this->ind = ind;
    }
};

std::string genName(int len){
    std::string s = "";
    for (int i = 0; i < len; i++){
        s += char(65+rand()%26 + 32);
    }
    return s;
}

void genTxtFile(int size){
    std::ofstream fout("input.txt", std::ios::trunc | std::ios::out);
    if (fout.is_open()){
        for (int i = 0; i < size; i++){
            std::string data = std::to_string(1000000+rand()%9000000)+' ' + char(65+rand()%26) + std::to_string(10+rand()%90) + ' ' + char(65+rand()%26)+genName(7);
            fout << data << std::endl;
        }
    }
    fout.close();
}
void genBinFile(int size){
    std::ifstream fin("input.txt");
    std::ofstream binfout("bininput.bin", std::ios::trunc | std::ios::out | std::ios::binary);
    if (fin.is_open() && binfout.is_open()){
        int a;
        char *code = new char[3];
        char *surname = new char[8];
        for (int i = 0; i < size; i++){
            fin >> a;
            fin >> code;
            fin >> surname;
            binfout.write((char*)&a, sizeof(int));
            binfout.write(code, 3);
            binfout.write(surname, 8);
        }
        delete[] code;
        delete[] surname;
    }
    binfout.close();
    fin.close();
}

int interpSearch(int key, std::vector<patient> &patients){
    int low = 0;
    int high = patients.size() - 1;
    int mid;
    while (patients[low].cardNum < key && patients[high].cardNum >= key){
    //интерполирующий поиск производит оценку новой области поиска
    //по расстоянию между ключом поиска и текущим значение элемента
    mid = low + ((key - patients[low].cardNum) * (high - low)) / (patients[high].cardNum - patients[low].cardNum);
    //если значение в ячейке с индексом mid меньше, то смещаем нижнюю границу
    if (patients[mid].cardNum < key)
        low = mid + 1;
    //в случае, если значение больше, то смещаем верхнюю границу
    else if (patients[mid].cardNum > key)
        high = mid - 1;
    //если равны, то возвращаем индекс
    else
        return mid;
    }
    //если цикл while не вернул индекс искомого значения,
    //то проверяем не находится ли оно в ячейке массива с индексом low,
    //иначе возвращаем -1 (значение не найдено)
    if (patients[low].cardNum == key)
        return low;
    else
        return -1;
}

int main(){
    srand(time(nullptr));
    //Задание 2
    int size;
    /*
    std::cout << "Enter file size" << std::endl;
    std::cin >> size;
    genTxtFile(size);
    genBinFile(size);
*/
    int cardNum;
    int indToSearch;
    char *code = new char[3];
    char *surname = new char[8];
/*
    std::cout << "Enter card number" << std::endl;
    std::cin >> indToSearch;
    std::ifstream binfin1("bininput.bin", std::ios::in | std::ios::binary);
    for (int i = 0; i < size; i++){
        binfin1.read((char*)&cardNum, sizeof(int));
        if (cardNum == indToSearch){
            binfin1.read(code, 3);
            binfin1.read(surname, 8);
            std::cout << "Line №" << i+1 << " " << code << " " << surname << std::endl;
            break;
        }
        binfin1.seekg(11, std::ios::cur);
    }
    binfin1.close();*/
    //Задние 3
    std::cout << "Enter file size" << std::endl;
    std::cin >> size;
    genTxtFile(size);
    genBinFile(size);

    std::vector<patient> patients;

    std::cout << "Enter card number" << std::endl;
    std::cin >> indToSearch;
    std::ifstream binfin2("bininput.bin", std::ios::in | std::ios::binary);
    for (int i = 0; i < size; i++){
        binfin2.read((char*)&cardNum, sizeof(int));
        patients.push_back(patient(cardNum, i));
        binfin2.seekg(11, std::ios::cur);
    }
    binfin2.close();
    std::sort(patients.begin(), patients.end(), [](patient a, patient b)
                                  {
                                      return a.cardNum <= b.cardNum;
                                  });
    std::ofstream ft("sorted.txt", std::ios::trunc | std::ios::out);
    for (int i = 0; i < size; i++){
        ft << patients.at(i).ind << " " << patients.at(i).cardNum << std::endl;
    }
    ft.close();
    
    std::cout << interpSearch(indToSearch, patients) << std::endl;
    delete[] code;
    delete[] surname;
    return 0;
}