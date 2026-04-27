#include <cstdint>
#include <cstddef>
#include <cstring>

static inline uint64_t rotl(uint64_t x, int r)
{
    return (x << r) | (x >> (64 - r));
}

static inline uint64_t mix64(uint64_t x)
{
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

template<typename T>
uint64_t mem_hash
(
    const T* data,
    size_t len
)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data);
    size_t byte_len = len * sizeof(T);
    uint64_t h = 0x1234567890abcdefULL;
    size_t i = 0;

    // Основной цикл: по 8 байт
    for (; i + 8 <= byte_len; i += 8)
    {
        uint64_t v;
        memcpy(&v, bytes + i, 8);
        uint64_t x = v;
        x ^= (i / 8 + 1) * 0x9e3779b97f4a7c15ULL;
        x = mix64(x);
        h ^= x;
        h = mix64(h);
        h = rotl(h, 27);
    }

    // Хвост: оставшиеся байты (0-7)
    if (i < byte_len)
    {
        uint64_t tail = 0;
        memcpy(&tail, bytes + i, byte_len - i);
        tail ^= (i / 8 + 1) * 0x9e3779b97f4a7c15ULL;
        tail = mix64(tail);
        h ^= tail;
        h = mix64(h);
        h = rotl(h, 27);
    }

    return mix64(h);
}
