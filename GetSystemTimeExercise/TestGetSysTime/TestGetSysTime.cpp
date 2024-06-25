#include <Windows.h>
#include <iostream>

int main() {
    SYSTEMTIME st;

    GetSystemTime(&st);

    std::cout << "System Time: " << st.wHour << ":" << st.wMinute << ":" << st.wSecond << std::endl;

    return 0;
}
