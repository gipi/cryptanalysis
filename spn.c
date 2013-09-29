/*
 * A basic substitution-permutation network (SPN) cipher.
 *
 * We have four rounds with 5 subkey mixing steps.
 */
#include<stdio.h>
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


uint16_t four_sboxes(uint16_t input) {
        uint16_t
        result = sbox(input & 0x000f);
        result |= sbox((input & 0x00f0) >> 4) << 4;
        result |= sbox((input & 0x0f00) >> 8) << 8;
        result |= sbox((input & 0xf000) >> 12) << 12;

        return result;
}

int main(int argc, char* argv[]) {
    int isRunning = 1;
    uint16_t input;

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

        uint16_t result = four_sboxes(input);

        write(1, &result, 2);
    }

    return 0;
}
