#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fish.h"

#define KEYS_DATA "keys.dat"                    // Initial key values
#define HALF_BLOCK_SIZE  (sizeof(uint32_t))

static uint32_t P[NUM_SUBKEYS] =  {0};
static uint32_t S[NUM_SBOXES][256] = {0};

static void algorithm(uint32_t *x, uint32_t *y, bool reverse);
static uint32_t f(uint32_t xL);
static void swap(uint32_t *a, uint32_t *b);

uint32_t* combine_blocks(uint32_t *xl, uint32_t *xr, int length){
    uint32_t *blocks = (uint32_t*)calloc(2*length, sizeof(uint32_t));

    if(blocks == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    int j=0;
    for(int i=0; i<length; i++){
        blocks[j] = xl[i];
        blocks[j+1] = xr[i];
        j+=2;
    }

    return blocks;
}

uint32_t* text_to_blocks(char text[], int length){
    int num_blocks = length / sizeof(uint32_t);

    if(length % sizeof(uint32_t) != 0){
        num_blocks++;
    }

    uint32_t *blocks = (uint32_t*)calloc(num_blocks, sizeof(uint32_t));

    if(blocks == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    int j=0;
    for(int i=0; i<num_blocks; i++){
        uint32_t block = 0x00000000;
        
        for(int k=0, t_size=sizeof(uint32_t); k<t_size; k++){
            if(j < length){
                block = block << 8 | text[j];
            }else{
                block = block << 8 | 0x00;
            }
            j++;
        }

        blocks[i] = block;
    }

    return blocks;
}

char* blocks_to_text(uint32_t bytes[], int block_length){
    int length = block_length * sizeof(uint32_t);
    char *text = (char*)calloc(length+1, sizeof(char));

    if(text == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }

    int j=0;
    for(int i=0; i<block_length; i++){
        uint32_t block = bytes[i];
        
        for(int k=HALF_BLOCK_SIZE-1; k>-1; k--){
            char c = (char)(block >> (k*8));
            text[j] = c;
            j++;
        }
    }

    text[j] = '\0';
    return text;
}

uint8_t fish_set_key(uint8_t key[], int length){

    if(length > MAX_KEY_SIZE){
        // key too long, invalid key
        return 255;
    }

    FILE *fp = fopen(KEYS_DATA, "rb");

    if(fp == NULL) {
        printf("Error on initializing the keys!\n");
        return 255;
    }

    fread(P, sizeof(P), 1, fp);
    fread(S, sizeof(S), 1, fp);
    fclose(fp);

    int j=0;

    for(int i=0; i<NUM_SUBKEYS; i++) {
        uint32_t data = 0x00000000;

        for(int k=0; k<4; k++){
            data = (data << 8) | key[j];
            j = (j+1) % length;
        }
        
        P[i] = P[i] ^ data;
    }

    uint32_t xl = 0x00000000;
    uint32_t xr = 0x00000000;
    
    for(int i=0; i<NUM_SUBKEYS; i+=2){      // Mix in with the P-array
        algorithm(&xl, &xr, false);
        P[i] = xl;
        P[i+1] = xr;
    }

    for(int i=0; i<NUM_SBOXES; i++){       // Mix in with the S-boxes
        for(int j=0; j<256; j+=2){
            algorithm(&xl, &xr, false);
            S[i][j] = xl;
            S[i][j+1] = xr;
        }
    }

    return 0;
}

uint32_t* fish_encrypt_blocks(uint32_t* blocks, int length){

    uint32_t *crypt_blocks = (uint32_t*)calloc(length, sizeof(uint32_t));

    for(int i=0; i<length; i+=2){
        uint32_t xl = 0x00000000;
        uint32_t xr = 0x00000000;
        
        xl = blocks[i];

        if(i+1 < length){
            xr = blocks[i+1];
        }
        
        algorithm(&xl, &xr, false);
        crypt_blocks[i] = xl;
        crypt_blocks[i+1] = xr;
    }

    return crypt_blocks;
}

uint32_t* fish_decrypt_blocks(uint32_t* blocks, int length){
    uint32_t *clear_blocks = (uint32_t*)calloc(length, sizeof(uint32_t));

    for(int i=0; i<length; i+=2){
        uint32_t xl = 0x00000000;
        uint32_t xr = 0x00000000;
        
        xl = blocks[i];

        if(i+1 < length){
            xr = blocks[i+1];
        }
        
        algorithm(&xl, &xr, true);
        clear_blocks[i] = xl;
        clear_blocks[i+1] = xr;
    }

    return clear_blocks;
}

static uint32_t f(uint32_t xL){
    // Divide xL into four eight-bit quarters: a, b, c, and d
    uint8_t a = xL >> 24;
    uint8_t b = (xL >> 16) & 0xFF;
    uint8_t c = (xL >> 8) & 0xFF;
    uint8_t d = xL & 0xFF;

    // Perform the S-box substitutions
    return (( (S[0][a] + S[1][b])          // SUM OF S-BOXES 0 AND 1
                ^ S[2][c] )                // XOR with S-BOX 2
                + S[3][d]);                // ADD with S-BOX 3
}

static void swap(uint32_t *a, uint32_t *b){
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

static void algorithm(uint32_t *x, uint32_t *y, bool reverse){
    uint32_t xL = *x;     // Left half
    uint32_t xR = *y;    // Right half

    // IF reverse is true, then we need to reverse the order of the P-array
    // and the S-boxes
    if(reverse){
        for(int i=NUM_ROUNDS+1; i>1; i--){
            xL ^= P[i];
            xR ^= f(xL);
            swap(&xL, &xR);
        }

        swap(&xL, &xR);
        xR ^= P[1];
        xL ^= P[0];

    } else {
        for(int i=0; i<NUM_ROUNDS; i++){
            xL ^= P[i];
            xR ^= f(xL);
            swap(&xL, &xR);
        }

        swap(&xL, &xR);
        xR ^= P[NUM_ROUNDS];
        xL ^= P[NUM_ROUNDS + 1];
    }

    *x = xL;
    *y = xR;
}