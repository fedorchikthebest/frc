#include<iostream>
#include<string.h>

unsigned long long int bytes_to_long_int(char* buf_begin){
    unsigned long long int ans;
    memcpy(&ans, buf_begin, sizeof(unsigned long long int));
    return ans;
}

int main(){
    std::cout << true;
}