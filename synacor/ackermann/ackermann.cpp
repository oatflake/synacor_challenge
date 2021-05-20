#include <iostream>
#include <array>

/*
 * translated from assembly
 
r7 = 1
def func6027(n, m):
    global r7
    if n == 0:
        return (m + 1) % 32768
    if m == 0:
        return func6027((n + 32767) % 32768, r7)
    return func6027((n + 32767) % 32768, func6027(n, (m + 32767) % 32768))
print(func6027(4, 1))

*/

uint16_t ackermann(uint16_t r7) {
    std::array<uint16_t, 5 * 32768> table;
    for (uint16_t m = 0; m < 32768; m++) {  // if n == 0
        table[m] = (m + 1) % 32768;
    }
    for (uint16_t n = 1; n < 5; n++) {
        table[n * 32768] = table[r7 + (n - 1) * 32768]; // if m == 0
        for (uint16_t m = 1; m < 32768; m++) {
            table[m + n * 32768] = table[table[m - 1 + n * 32768] + (n - 1) * 32768];
        }
    }
    return table[1 + 4 * 32768];
}

int main()
{
    for (uint16_t r7 = 0; r7 < 32768; r7++) {
        if (ackermann(r7) == 6)
            std::cout << r7 << std::endl;       // takes a while, result is 25734
    }
}