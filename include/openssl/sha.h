#pragma once
#define SHA256_DIGEST_LENGTH 32
typedef struct SHA256state_st { unsigned int h[8]; unsigned int Nl, Nh; unsigned int data[16]; unsigned int num, md_len; } SHA256_CTX;
inline int SHA256_Init(SHA256_CTX*) { return 1; }
inline int SHA256_Update(SHA256_CTX*, const void*, size_t) { return 1; }
inline int SHA256_Final(unsigned char*, SHA256_CTX*) { return 1; }
inline unsigned char *SHA256(const unsigned char*, size_t, unsigned char*) { return nullptr; }
