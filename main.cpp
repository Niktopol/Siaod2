#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

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
    auto begin = std::chrono::high_resolution_clock::now();
    int start = 0;
    int end = patients.size() - 1;
    int mid = 0;
    if (patients.empty()){
        std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
        return -1;
    }
    while ((patients[start].cardNum < key) && (key < patients[end].cardNum)){
        mid = start +(key-patients[start].cardNum)/(patients[end].cardNum-patients[start].cardNum)/(end-start);
        if (patients[mid].cardNum < key){
            start = mid+1;
        }else if (patients[mid].cardNum > key){
            end = mid-1;
        }else{
            std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
            return patients[mid].ind;
        }
    }
    if (patients[start].cardNum == key){
        std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
        return patients[start].ind;
    }else if (patients[end].cardNum == key){
        std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
        return patients[end].ind;
    }
    std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
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
    patientInfo info;
    file.read((char*)&info, sizeof(info));
    return(std::to_string(info.cardNum)+ ' ' + info.illness + ' ' +info.doctor);
}

int main(){
    srand(time(nullptr));
    //Задание 2
    int size;
    int cardNum;
    patientInfo info;

    std::cout << "Enter file size" << std::endl;
    std::cin >> size;
    genTxtFile(size);
    genBinFile(size);

    std::cout << "Enter card number" << std::endl;
    std::cin >> cardNum;
    std::ifstream binfin("bininput.bin", std::ios::in | std::ios::binary);
    bool found = false;
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < size; i++){
        binfin.read((char*)&info, sizeof(patientInfo));
        if (info.cardNum == cardNum){
            std::cout << "Found: " <<info.cardNum << ' ' << info.illness << ' ' << info.doctor << std::endl;
            found = true;
            break;
        }
    }
    if (!found){
        std::cout << "Record not found" << std::endl;
    }
    std::cout << "Finished in: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin).count() << " ns\n";
    //Задние 3
    std::cout << "Enter file size" << std::endl;
    std::cin >> size;
    genTxtFile(size);
    genBinFile(size);
    
    std::cout << "Enter card number" << std::endl;
    std::cin >> cardNum;
    if (getByKey(binfin, cardNum, size)){
        std::cout << "Found: " << readFromAdress(binfin) << std::endl;
    }else{
        std::cout << "Record not found" << std::endl;
    };
    binfin.close();

    return 0;
}