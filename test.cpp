#include<iostream>
#include<string.h>

unsigned long long int bytes_to_long_int(char* buf_begin){
    unsigned long long int ans;
    memcpy(&ans, buf_begin, sizeof(unsigned long long int));
    return ans;
}

int main(){
    const uint64_t list = bytes_to_long_int("list\x00\x00\x00");
        const uint64_t recursion = bytes_to_long_int("recurse");
        const uint64_t single = bytes_to_long_int("single\x00");
    char a[] = "list\x00\x00\x00\x00";
    std::cout << list << std::endl
        << recursion << std::endl
        << single << std::endl;
}