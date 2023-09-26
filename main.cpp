#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::string genName(int);

struct patient{
    int cardNum;
    int ind;
    patient(int cardNum, int ind){
        this->cardNum = cardNum;
        this->ind = ind;
    }
};
struct patientInfo{
    int cardNum;
    char illness[8];
    char doctor[8];
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
            std::string data = std::to_string(1000000+rand()%9000000)+' ' + char(65+rand()%26) + std::to_string(10+rand()%90) + ' ' + char(65+rand()%26)+genName(6);
            fout << data << std::endl;
        }
    }
    fout.close();
}
void genBinFile(int size){
    std::ifstream fin("input.txt");
    std::ofstream binfout("bininput.bin", std::ios::trunc | std::ios::out | std::ios::binary);
    if (fin.is_open() && binfout.is_open()){
        patientInfo info;
        for (int i = 0; i < size; i++){
            fin >> info.cardNum;
            fin >> info.illness;
            fin >> info.doctor;
            binfout.write((char*)&info, sizeof(info));
        }
    }
    binfout.close();
    fin.close();
}

int interpSearch(int key, std::vector<patient> &patients){
    int start = 0;
    int end = patients.size() - 1;
    int k = 0;
    int mid = 0;
    while ((patients[end].cardNum != patients[start].cardNum) && (patients[start].cardNum <= key) && (key <= patients[end].cardNum)){
        k = (patients[end].cardNum-patients[start].cardNum)/(end-start);
        mid = start +(key-patients[start].cardNum)/k;
        if (key == patients[mid].cardNum){
            return patients[mid].ind;
        }else if (key < patients[mid].cardNum){
            end = mid-1;
        }else{
            start = mid+1;
        }
    }
    if (key == patients[start].cardNum){
        return patients[start].ind;
    }
    return -1;
}

bool getByKey(std::ifstream &file, int key, int size){
    file.seekg(0, std::ios::beg);
    patientInfo info;
    std::vector<patient> patients;
    for(int i = 0; i < size; i++){
        file.read((char*)&info, sizeof(info));
        patients.push_back(patient(info.cardNum, i));
    }
    std::sort(patients.begin(), patients.end(), [](patient a, patient b)
                                  {
                                      return a.cardNum <= b.cardNum;
                                  });
    int index = interpSearch(key, patients);
    if (index == -1){
        return false;
    }else{
        file.seekg(sizeof(info)*index, std::ios::beg);
        return true;
    }
}
std::string readFromAdress(std::ifstream &file){
    
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

    std::ifstream binfin2("bininput.bin", std::ios::in | std::ios::binary);
    patientInfo info;
    
    std::cout << "Enter card number" << std::endl;
    std::cin >> indToSearch;
    getByKey(binfin2, indToSearch, size);


    binfin2.close();
    return 0;
}