#include <iostream>

int a[3] = {4,5,6};
int* ap = a;

int b[3][2] = {{3,1},{4,1},{5,9}};
int (*bp)[2] = b;

float testFunc(float inVal) {
    return inVal * 5.0f;
}

int getSystemPinNum(int userPinNum)
{
    return 8 * (userPinNum / 8) + 7 - (userPinNum % 8);
}

int main()
{

    int b1 = bp[2][1];
    
    std::cout << b1 << std::endl;
}