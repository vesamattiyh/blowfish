#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fish.h"
#include "tests/test_fish.c"

int main(void) {
    
    printf("Running encrypt variable key tests!\n");
    uint32_t *plain_bytes = combine_blocks(plaintext_l, plaintext_r, NUM_VARIABLE_KEY_TESTS);
    uint32_t *cipher_bytes = combine_blocks(ciphertext_l, ciphertext_r, NUM_VARIABLE_KEY_TESTS);
    
    for(int i=0; i<NUM_VARIABLE_KEY_TESTS*2; i+=2){
        printf("Test: %d\nClear bytes: 0x%08x%08x ", i/2, plain_bytes[i], plain_bytes[i+1]);
        
        printf("\nKey bytes: ");
        for(int k=0; k<8; k++){
            printf("0x%02x ", variable_key[i/2][k]);
        }
        printf("\nCipher bytes: 0x%08x%08x ", cipher_bytes[i], cipher_bytes[i+1]);

        uint8_t key[8] = {0};
        memcpy(&key, variable_key[i/2], sizeof(key));

        for(int k=0; k<8; k++){
            printf("\n0x%02x dec:%d key[%d]", key[k], key[k], k);
        }
        printf("\n");

        fish_set_key(key, 8);
        uint32_t *crypt = fish_encrypt_blocks(plain_bytes, NUM_VARIABLE_KEY_TESTS*2);

        printf("Crypted bytes: 0x%08x%08x\n", crypt[i], crypt[i+1]);

        if(crypt[i] == cipher_bytes[i] && crypt[i+1] == cipher_bytes[i+1]){
            printf("Result: [" COLOR_GREEN "PASS" COLOR_RESET "]\n" );
        } else {
            printf("Result: [" COLOR_RED "FAIL" COLOR_RESET "]\n" );
        }
        printf("\n\n");
    }

    printf("Running decrypt variable key tests!\n");
    
    for(int i=0; i<NUM_VARIABLE_KEY_TESTS*2; i+=2){
        printf("Test: %d\nClear bytes: 0x%08x%08x ", i/2, plain_bytes[i], plain_bytes[i+1]);
        
        printf("\nKey bytes: ");
        for(int k=0; k<8; k++){
            printf("0x%02x ", variable_key[i/2][k]);
        }
        printf("\nCipher bytes: 0x%08x%08x ", cipher_bytes[i], cipher_bytes[i+1]);

        uint8_t key[8] = {0};
        memcpy(&key, variable_key[i/2], sizeof(key));

        for(int k=0; k<8; k++){
            printf("\n0x%02x dec:%d key[%d]", key[k], key[k], k);
        }
        printf("\n");

        fish_set_key(key, 8);
        uint32_t *decrypt = fish_decrypt_blocks(cipher_bytes, NUM_VARIABLE_KEY_TESTS*2);

        printf("Decrypted bytes: 0x%08x%08x\n", decrypt[i], decrypt[i+1]);

        if(decrypt[i] == plain_bytes[i] && decrypt[i+1] == plain_bytes[i+1]){
            printf("Result: [" COLOR_GREEN "PASS" COLOR_RESET "]\n" );
        } else {
            printf("Result: [" COLOR_RED "FAIL" COLOR_RESET "]\n" );
        }
        printf("\n\n");
    }
 
    return 0;
}
