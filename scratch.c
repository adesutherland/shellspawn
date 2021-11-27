//
// Created by adria on 10/11/2021.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


size_t buffer_size(size_t value) {
    size_t i;
    if (value <= 32) return 32;

    --value;
    for (i = 1; i < sizeof(size_t); i *= 2)
        value |= value >> i;
    return value + 1;
}


int main() {
    size_t in;
    size_t out;

    in = 10; out = buffer_size(in); printf("%llu -> %llu\n", in, out);

    in = 200; out = buffer_size(in); printf("%llu -> %llu\n", in, out);
    in = 40; out = buffer_size(in); printf("%llu -> %llu\n", in, out);
    in = 5000; out = buffer_size(in); printf("%llu -> %llu\n", in, out);
    in = 100; out = buffer_size(in); printf("%llu -> %llu\n", in, out);


}
