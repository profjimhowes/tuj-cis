typedef struct {
    struct {
        BigInt* (*from_int)(long value);
        BigInt* (*from_str)(const char* str);
        BigInt* (*copy)(const BigInt* src);
        void (*destroy)(BigInt* bi);
    } create;
    
    struct {
        BigInt* (*add)(const BigInt* a, const BigInt* b);
        BigInt* (*sub)(const BigInt* a, const BigInt* b);
        BigInt* (*mul)(const BigInt* a, const BigInt* b);
        BigInt* (*div)(const BigInt* a, const BigInt* b);
    } math;
    
    struct {
        int (*cmp)(const BigInt* a, const BigInt* b);
        bool (*eq)(const BigInt* a, const BigInt* b);
        bool (*lt)(const BigInt* a, const BigInt* b);
    } compare;
} BigIntLib;

extern const BigIntLib BigInt;