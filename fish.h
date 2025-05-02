#include <stdint.h>

#define MAX_KEY_SIZE 56     // Size of key is 448 bits, 56 bytes
#define NUM_ROUNDS 16       // Manipulation rounds
#define NUM_SUBKEYS 18      // Number of subkeys
#define NUM_SBOXES 4        // Number of S-boxes


char* blocks_to_text(uint32_t bytes[], int block_length);
uint32_t* text_to_blocks(char text[], int length);

uint8_t fish_set_key(uint8_t key[], int length);
uint32_t* fish_encrypt_blocks(uint32_t* blocks, int length);
uint32_t* fish_decrypt_blocks(uint32_t* blocks, int length);
uint32_t* combine_blocks(uint32_t *xl, uint32_t *xr, int length);