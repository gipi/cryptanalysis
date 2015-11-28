/**
 * Interesting POC inspired from <http://yurichev.com/blog/modulo/>.
 *
 * My aim is to deduce the internal form of the LCG algorithm observing
 * its output.
 *
 * LCG = Linear Congruential Generator is defined by s_{n+1} = a s_n + b mod m,
 *
 * <http://security.stackexchange.com/questions/4268/cracking-a-linear-congruential-generator>
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t state;

uint32_t lcg_rand() {
    state = state * 214013 + 2531011;
    return (state >> 16) & 0x7FFF;
};

int main(int argc, char* argv[]) {
    int iterations = 1;

    if (argc == 2) {
        iterations = atoi(argv[1]);
    }

    unsigned int cycle = 0;
    for (cycle = 0 ; cycle < iterations ; cycle++) {
        unsigned int value = lcg_rand();
        printf("%u\n", value);
    }

    return 0;
}
