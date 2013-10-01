/*
 * A basic substitution-permutation network (SPN) cipher.
 *
 * We have four rounds with 5 subkey mixing steps.
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<inttypes.h>
#include<assert.h>

#define BUF_SIZE 256

static const uint8_t sbox_matrix[0x10] = {
    0x0e, 0x04, 0x0d, 0x01,
    0x02, 0x0f, 0x0b, 0x08,
    0x03, 0x0a, 0x06, 0x0c,
    0x05, 0x09, 0x00, 0x07
};

char sbox(uint8_t input) {
    assert((input & 0xf0) == 0x00);

    return sbox_matrix[input];
}

static const uint8_t permutation_matrix[0x10] = {
    0x00, 0x04, 0x08, 0x0a,
    0x01, 0x05, 0x09, 0x0d,
    0x02, 0x06, 0x0a, 0x0e,
    0x03, 0x07, 0x0b, 0x0f
};

uint16_t permutation(uint16_t input) {
    uint16_t output = 0;

    int cycle;
    for (cycle = 0 ; cycle < 0x10 ; cycle++) {
        // is bit set?
        int is = (1 << cycle) & input;
        if (is)
            output |= 1 << permutation_matrix[cycle];
    }

    return output;
}

uint16_t four_sboxes(uint16_t input) {
        uint16_t
        result = sbox(input & 0x000f);
        result |= sbox((input & 0x00f0) >> 4) << 4;
        result |= sbox((input & 0x0f00) >> 8) << 8;
        result |= sbox((input & 0xf000) >> 12) << 12;

        return result;
}

uint16_t complete_round(uint16_t input, uint16_t key) {
    input ^= key;

    return permutation(four_sboxes(input));
}

uint16_t last_round(uint16_t input, uint16_t key) {
    return four_sboxes(input ^ key);
}

void usage(char* progname) {
    fprintf(stderr,
        "usage: %s -key <80bits key>\n\n"
        "crypt the stdin"
        , progname);
}

int main(int argc, char* argv[]) {
    int isRunning = 1;
    uint16_t input = 0;
    // the key is 64bit, i.e. 8 bytes
    uint64_t key  = 0x0000000000000000;
    uint16_t key_tail = 0x0000;

    if (argc < 2) {
        usage(argv[0]);
        exit(1);
    }

    while (isRunning) {
        int n = read(0, &input, 2);

        /*
         * We are reading one/two bits at times, so we can have only three
         * outcomes: error, eof or correct read.
         */
        switch (n) {
            case -1:
                perror("stdin");
            case 0:
                isRunning = 0;
                continue;
                break;
            default:
                break;
        }

        int round;
        uint16_t result;
        uint64_t mask = 0x000000000000ffff;
        for (round = 0 ; round < 3 ; round++) {
            uint16_t subkey = key & mask;

            result = complete_round(input, subkey);

            mask <<= 16;
        }

        result = last_round(input, key_tail);

        write(1, &result, 2);
    }

    return 0;
}
