# 📖 Blowfish Encryption Library — C Implementation
This library is completely built for training purposes. 

## Algorithm
Blowfish is a symmetric-key block cipher designed by Bruce Schneier in 1993 as a fast, free alternative to existing encryption algorithms at the time. It’s known for its simplicity, flexibility, and security within its operational range.


📌 Key Features:
- Block Size: 64 bits (8 bytes)
- Key Size: Variable, from 32 bits (4 bytes) up to 448 bits (56 bytes)
- Structure: 16-round Feistel network
- Components: A key-dependent P-array (subkeys) and four large S-boxes

Performance: Designed for efficient encryption and decryption on general-purpose CPUs


📖 How Blowfish Works:

The input plaintext is split into two 32-bit halves.

The halves undergo 16 rounds of operations involving:
- Subkey XORs
- S-box lookups
- Modular addition and XOR operations
- After the final round, the two halves are recombined to produce the 64-bit ciphertext.

Decryption reverses the process with the same key.

Blowfish is a lightweight, configurable implementation of the Blowfish encryption algorithm in C. This library supports encryption and decryption of 64-bit (8-byte) blocks using a user-defined key.

---

## 📦 Available Functions

### 📃 Block and Text Conversion

**`char* blocks_to_text(uint32_t bytes[], int block_length)`**

Converts an array of 32-bit blocks back into a character string.

- **Parameters:**
  - `bytes[]`: Array of `uint32_t` blocks.
  - `block_length`: Number of 32-bit blocks.
- **Returns:**  
  Pointer to a null-terminated character string.

---

**`uint32_t* text_to_blocks(char text[], int length)`**

Converts a character string into an array of 32-bit blocks, padding as needed.

- **Parameters:**
  - `text[]`: Input string.
  - `length`: Length of the input string.
- **Returns:**  
  Pointer to an array of 32-bit blocks.

---

### 🔐 Key Management

**`uint8_t fish_set_key(uint8_t key[], int length)`**

Initializes the encryption key and generates subkeys and S-boxes.

- **Parameters:**
  - `key[]`: User-defined encryption key.
  - `length`: Key length in bytes (1–56 bytes).
- **Returns:**  
  `0` on success, non-zero for errors (e.g. invalid length).

---

### 🔒 Encryption / Decryption

**`uint32_t* fish_encrypt_blocks(uint32_t* blocks, int length)`**

Encrypts an array of 32-bit blocks using the Blowfish cipher.

- **Parameters:**
  - `blocks`: Pointer to input blocks.
  - `length`: Number of 32-bit blocks.
- **Returns:**  
  Pointer to encrypted blocks.

---

**`uint32_t* fish_decrypt_blocks(uint32_t* blocks, int length)`**

Decrypts an array of 32-bit blocks.

- **Parameters:**
  - `blocks`: Pointer to encrypted blocks.
  - `length`: Number of 32-bit blocks.
- **Returns:**  
  Pointer to decrypted blocks.

---

### ⚙️ Utility

**`uint32_t* combine_blocks(uint32_t *xl, uint32_t *xr, int length)`**

Combines two arrays of 32-bit values (left and right halves) into a single array of blocks.

- **Parameters:**
  - `xl`: Pointer to left half blocks.
  - `xr`: Pointer to right half blocks.
  - `length`: Number of 32-bit blocks per half.
- **Returns:**  
  Pointer to combined blocks.

---

## 📝 Example Usage

```c
#include "blowfish.h"

int main() {
    // Define key
    uint8_t key[] = "mysecretkey";
    fish_set_key(key, strlen((char*)key));

    // Text to encrypt
    char text[] = "HelloWorld!";
    int text_len = strlen(text);

    // Convert text to blocks
    uint32_t* blocks = text_to_blocks(text, text_len);

    // Encrypt blocks
    uint32_t* encrypted = fish_encrypt_blocks(blocks, text_len / 4 + (text_len % 4 != 0));

    // Decrypt blocks
    uint32_t* decrypted = fish_decrypt_blocks(encrypted, text_len / 4 + (text_len % 4 != 0));

    // Convert decrypted blocks back to text
    char* result = blocks_to_text(decrypted, text_len / 4 + (text_len % 4 != 0));

    printf("Decrypted: %s\n", result);

    return 0;
}
```

---

## Notes

- Blowfish operates on **64-bit (8-byte) blocks**. Ensure text is properly padded if not a multiple of 8 bytes.
- The `fish_set_key()` function **must be called before encryption or decryption**.
- Memory management for dynamically allocated arrays returned by functions like `text_to_blocks()` and `blocks_to_text()` is the **caller’s responsibility**.
- Key length must be between **1 and 56 bytes (8–448 bits)**, as per the Blowfish specification.

---

## References

- Schneier, B. (1993). *Description of a New Variable-Length Key, 64-Bit Block Cipher (Blowfish)*. Fast Software Encryption.
- [Wikipedia: Blowfish (cipher)](https://en.wikipedia.org/wiki/Blowfish_(cipher))

---

## License

