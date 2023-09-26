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

int interpSearch(int target, std::vector<int> &A){
    int left = 0;
    int right = A.size() - 1;
    int mid = 0;
    while ((A[right] != A[left]) && (A[left] <= target) && (target <= A[right])){
        mid = left + (target-A[left])/(A[right]-A[left])/(right-left);
        if (target == A[mid]){
            return mid;
        }else if (target < A[mid]){
            right = mid-1;
        }else{
            left = mid+1;
        }
    }
    if (target == A[left]){
        return left;
    }
    return -1;
    
}

int main(){
    std::vector<int> patients;
    int size;
    std::cin >> size;
    for (int i = 0; i < size; i ++){
        patients.push_back(1000000+rand()%9000000);
    }
    std::sort(patients.begin(), patients.end(), [](int a, int b)
                                  {
                                      return a <= b;
                                  });
    for (int i = 0; i < size; i ++){
        std::cout << i << " " << patients[i] << std::endl;
    }
    std::cin >> size;
    std::cout << interpSearch(size, patients);
    return 0;
}