/*
 * MEDUSA CORE ENCRYPTION LIBRARY - C/C++ Header
 * Gold Standard, Secure, Infinite Reusability
 * 
 * LIBRARY HEADER: Include this in any C/C++ project
 * SECURITY LEVEL: Gold Standard - Absolutely Secured
 * ACCESS CONTROL: Only Roylepython can access
 * 
 * USAGE:
 * #include <medusa_encryption.h>
 * // Link with: -lmedusa_encryption
 * 
 * FEATURES:
 * - AES-256-GCM encryption (Gold Standard)
 * - ChaCha20-Poly1305 stream encryption
 * - RSA-4096 asymmetric encryption
 * - ECDH key exchange with P-521 curves
 * - PBKDF2 key derivation with Yorkshire Champion Salt
 * - Secure memory management
 * - Thread-safe operations
 * - Cross-platform compatibility
 */

#ifndef LIBMEDUSA_ENCRYPTION_H
#define LIBMEDUSA_ENCRYPTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

// Library constants
#define MEDUSA_AES_KEY_SIZE 32
#define MEDUSA_AES_IV_SIZE 12
#define MEDUSA_AES_TAG_SIZE 16
#define MEDUSA_CHACHA_KEY_SIZE 32
#define MEDUSA_CHACHA_NONCE_SIZE 12
#define MEDUSA_RSA_KEY_SIZE 4096
#define MEDUSA_PBKDF2_ITERATIONS 100000

// Library initialization and information
int medusa_encryption_init(void);
const char* medusa_encryption_version(void);
double medusa_encryption_enigma_constant(void);

// AES-256-GCM Encryption/Decryption
int medusa_encrypt_aes_gcm(const unsigned char* plaintext, size_t plaintext_len,
                          const unsigned char* key, size_t key_len,
                          const char* context,
                          unsigned char* encrypted_data, size_t* encrypted_len,
                          unsigned char* iv, size_t* iv_len,
                          unsigned char* tag, size_t* tag_len);

int medusa_decrypt_aes_gcm(const unsigned char* encrypted_data, size_t encrypted_len,
                          const unsigned char* key, size_t key_len,
                          const unsigned char* iv, size_t iv_len,
                          const unsigned char* tag, size_t tag_len,
                          const char* context,
                          unsigned char* decrypted_data, size_t* decrypted_len);

// ChaCha20-Poly1305 Encryption/Decryption
int medusa_encrypt_chacha20_poly1305(const unsigned char* plaintext, size_t plaintext_len,
                                     const unsigned char* key, size_t key_len,
                                     const char* context,
                                     unsigned char* encrypted_data, size_t* encrypted_len,
                                     unsigned char* nonce, size_t* nonce_len,
                                     unsigned char* tag, size_t* tag_len);

int medusa_decrypt_chacha20_poly1305(const unsigned char* encrypted_data, size_t encrypted_len,
                                     const unsigned char* key, size_t key_len,
                                     const unsigned char* nonce, size_t nonce_len,
                                     const unsigned char* tag, size_t tag_len,
                                     const char* context,
                                     unsigned char* decrypted_data, size_t* decrypted_len);

// PBKDF2 Key Derivation
int medusa_derive_key_pbkdf2(const char* password, const char* context,
                            size_t key_length,
                            unsigned char* derived_key, size_t* derived_len);

// RSA-4096 Key Pair Generation
int medusa_generate_rsa_keypair(char* public_key_pem, size_t* public_len,
                               char* private_key_pem, size_t* private_len);

// RSA Encryption/Decryption
int medusa_encrypt_rsa(const unsigned char* plaintext, size_t plaintext_len,
                      const char* public_key_pem,
                      unsigned char* encrypted_data, size_t* encrypted_len);

int medusa_decrypt_rsa(const unsigned char* encrypted_data, size_t encrypted_len,
                      const char* private_key_pem,
                      unsigned char* decrypted_data, size_t* decrypted_len);

// ECDH Key Exchange
int medusa_generate_ecdh_keypair(char* public_key_pem, size_t* public_len,
                                char* private_key_pem, size_t* private_len);

int medusa_ecdh_derive_shared_secret(const char* private_key_pem, const char* public_key_pem,
                                    unsigned char* shared_secret, size_t* secret_len);

// Digital Signatures
int medusa_sign_data(const unsigned char* data, size_t data_len,
                    const char* private_key_pem,
                    unsigned char* signature, size_t* signature_len);

int medusa_verify_signature(const unsigned char* data, size_t data_len,
                           const unsigned char* signature, size_t signature_len,
                           const char* public_key_pem);

// Secure Random Generation
int medusa_generate_secure_random(unsigned char* buffer, size_t size);
uint64_t medusa_generate_secure_random_uint64(void);

// Secure Memory Management
void* medusa_allocate_secure(size_t size);
void medusa_deallocate_secure(void* ptr);
int medusa_secure_memzero(void* ptr, size_t size);

// Hashing Functions
int medusa_hash_sha256(const unsigned char* data, size_t data_len,
                      unsigned char* hash, size_t* hash_len);

int medusa_hash_sha512(const unsigned char* data, size_t data_len,
                      unsigned char* hash, size_t* hash_len);

int medusa_hmac_sha256(const unsigned char* data, size_t data_len,
                      const unsigned char* key, size_t key_len,
                      unsigned char* hmac, size_t* hmac_len);

// Password Hashing (for authentication integration)
int medusa_hash_password(const char* password, const char* context,
                        char* hash_output, size_t hash_size);

int medusa_verify_password(const char* password, const char* hash, const char* context);

// Security Utilities
int medusa_constant_time_compare(const void* a, const void* b, size_t size);
int medusa_generate_salt(char* salt, size_t salt_size);

// Library Status and Diagnostics
int medusa_encryption_security_audit(char* audit_result, size_t result_size);
int medusa_encryption_performance_test(char* test_result, size_t result_size);

// Error Handling
const char* medusa_encryption_get_last_error(void);
void medusa_encryption_clear_error(void);

#ifdef __cplusplus
}
#endif

#endif // LIBMEDUSA_ENCRYPTION_H

/*
USAGE EXAMPLES:

1. AES-256-GCM Encryption:
```c
#include <medusa_encryption.h>

unsigned char key[MEDUSA_AES_KEY_SIZE];
unsigned char plaintext[] = "Hello, Yorkshire Champion!";
unsigned char encrypted[1024];
unsigned char iv[MEDUSA_AES_IV_SIZE];
unsigned char tag[MEDUSA_AES_TAG_SIZE];
size_t encrypted_len = sizeof(encrypted);
size_t iv_len = sizeof(iv);
size_t tag_len = sizeof(tag);

// Derive key from password
medusa_derive_key_pbkdf2("my_password", "my_context", MEDUSA_AES_KEY_SIZE, key, &key_len);

// Encrypt data
if (medusa_encrypt_aes_gcm(plaintext, strlen(plaintext), key, sizeof(key), "context",
                          encrypted, &encrypted_len, iv, &iv_len, tag, &tag_len)) {
    printf("Encryption successful!\\n");
}
```

2. RSA Key Generation:
```c
char public_key[8192];
char private_key[8192];
size_t pub_len = sizeof(public_key);
size_t priv_len = sizeof(private_key);

if (medusa_generate_rsa_keypair(public_key, &pub_len, private_key, &priv_len)) {
    printf("RSA keypair generated!\\n");
}
```

3. Secure Memory:
```c
void* secure_ptr = medusa_allocate_secure(1024);
// Use secure memory...
medusa_deallocate_secure(secure_ptr);
```

LIBRARY READY FOR INFINITE REUSE ACROSS ALL PROJECTS!
*/