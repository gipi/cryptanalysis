/*
 * A basic substitution-permutation network (SPN) cipher.
 *
 * We have four rounds with 5 subkey mixing steps.
 */
#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#define BUF_SIZE 256

const char sbox_matrix[0x10] = {
    0x0e, 0x04, 0x0d, 0x01,
    0x02, 0x0f, 0x0b, 0x08,
    0x03, 0x0a, 0x06, 0x0c,
    0x05, 0x09, 0x00, 0x07
};

char sbox(uint8_t input) {
    assert((input & 0xf0) == 0x00);

    return sbox_matrix[input];
}

int main(int argc, char* argv[]) {
    char buffer[BUF_SIZE];

    int n = 0;
    int isRunning = 1;

    while (isRunning) {
        n = read(0, buffer, 1);

        /*
         * We are reading one bit at times, so we can have only three
         * outcomes: error, eof or correct read.
         */
        switch (n) {
            case -1:
                perror("stdin");
            case 0:
                isRunning = 0;
                continue;
                break;
            case 1:
                break;
        }

        

        char
        result = sbox(buffer[0] & 0x0f);
        result |= sbox(buffer[0] >> 4) << 4;

        write(1, &result, 1);
    }

    return 0;
}
