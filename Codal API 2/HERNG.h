/* HERNG.h - Header-only seeded PRNG (xoshiro256** with splitmix64 seeding)
 *
 * Features:
 *  - Seed from 64-bit scalar or arbitrary byte buffer.
 *  - Fast u64/u32 generation, unbiased bounded integers, and byte filling.
 *  - Lightweight reseed/mix for long uptimes.
 *
 * Usage:
 *  HERNG_State rng;
 *  HERNG_seed_u64(&rng, 0x12345678ULL);
 *  uint32_t r = HERNG_u32(&rng);
 *  uint32_t die = HERNG_bounded_u32(&rng, 6) + 1;
 *
 * Notes: Not a CSPRNG. For adversarial resistance, use a cryptographic DRBG
 * (e.g., ChaCha20/AES-CTR) seeded from a TRNG.
 */

#ifndef HERNG_H
#define HERNG_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- State --- */

typedef struct {
    uint64_t s[4];
} HERNG_State;

/* --- Internals: splitmix64 seeding & rotations --- */

static inline uint64_t HERNG_rotl64(uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

static inline uint64_t HERNG_splitmix64_next(uint64_t *x) {
    uint64_t z = (*x += 0x9E3779B97F4A7C15ULL);
    z ^= (z >> 30); z *= 0xBF58476D1CE4E5B9ULL;
    z ^= (z >> 27); z *= 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/* xoshiro256** step */
static inline uint64_t HERNG_xoshiro256ss_next(HERNG_State *st) {
    const uint64_t result = HERNG_rotl64(st->s[1] * 5ULL, 7) * 9ULL;

    const uint64_t t = st->s[1] << 17;
    st->s[2] ^= st->s[0];
    st->s[3] ^= st->s[1];
    st->s[1] ^= st->s[2];
    st->s[0] ^= st->s[3];
    st->s[2] ^= t;
    st->s[3] = HERNG_rotl64(st->s[3], 45);

    return result;
}

/* --- Seeding --- */

/* Seed from a single 64-bit value (expanded via splitmix64). */
static inline void HERNG_seed_u64(HERNG_State *st, uint64_t seed) {
    uint64_t sm = seed + 0x9E3779B97F4A7C15ULL; /* stir starting point */
    st->s[0] = HERNG_splitmix64_next(&sm);
    st->s[1] = HERNG_splitmix64_next(&sm);
    st->s[2] = HERNG_splitmix64_next(&sm);
    st->s[3] = HERNG_splitmix64_next(&sm);

    /* Avoid the all-zero state (shouldn't occur, but cheap insurance). */
    if ((st->s[0] | st->s[1] | st->s[2] | st->s[3]) == 0) {
        st->s[0] = 0x1ULL;
    }
}

/* Seed from an arbitrary byte buffer. Robustly mixes into a 64-bit
 * accumulator and then expands to 256 bits via splitmix64. */
static inline void HERNG_seed_bytes(HERNG_State *st, const void *seed, size_t len) {
    const uint8_t *p = (const uint8_t *)seed;
    uint64_t acc = 0x6A09E667F3BCC909ULL; /* non-zero IV */

    /* Absorb 8-byte chunks, little-endian, with simple permutation between */
    while (len >= 8) {
        uint64_t v =
            ((uint64_t)p[0])       |
            ((uint64_t)p[1] << 8)  |
            ((uint64_t)p[2] << 16) |
            ((uint64_t)p[3] << 24) |
            ((uint64_t)p[4] << 32) |
            ((uint64_t)p[5] << 40) |
            ((uint64_t)p[6] << 48) |
            ((uint64_t)p[7] << 56);
        acc ^= v;
        /* one round of splitmix-like scrambling */
        acc ^= (acc >> 30); acc *= 0xBF58476D1CE4E5B9ULL;
        acc ^= (acc >> 27); acc *= 0x94D049BB133111EBULL;
        acc ^= (acc >> 31);

        p += 8;
        len -= 8;
    }

    /* Tail (0..7 bytes), pad with length to avoid collisions */
    if (len > 0) {
        uint64_t v = 0;
        for (size_t i = 0; i < len; ++i) v |= ((uint64_t)p[i]) << (8 * i);
        v ^= ((uint64_t)len) << 61;
        acc ^= v;
        acc ^= (acc >> 30); acc *= 0xBF58476D1CE4E5B9ULL;
        acc ^= (acc >> 27); acc *= 0x94D049BB133111EBULL;
        acc ^= (acc >> 31);
    }

    HERNG_seed_u64(st, acc);
}

/* Mix in additional seed material without full reseed (e.g., periodic TRNG words). */
static inline void HERNG_mix_seed_bytes(HERNG_State *st, const void *seed, size_t len) {
    /* Fold incoming bytes into a small splitmix accumulator, then XOR into state. */
    uint64_t acc = 0xC2B2AE3D27D4EB4FULL;
    const uint8_t *p = (const uint8_t *)seed;

    while (len >= 8) {
        uint64_t v =
            ((uint64_t)p[0])       |
            ((uint64_t)p[1] << 8)  |
            ((uint64_t)p[2] << 16) |
            ((uint64_t)p[3] << 24) |
            ((uint64_t)p[4] << 32) |
            ((uint64_t)p[5] << 40) |
            ((uint64_t)p[6] << 48) |
            ((uint64_t)p[7] << 56);
        acc ^= v;
        acc = HERNG_splitmix64_next(&acc);
        p += 8; len -= 8;
    }
    if (len > 0) {
        uint64_t v = 0;
        for (size_t i = 0; i < len; ++i) v |= ((uint64_t)p[i]) << (8 * i);
        acc ^= v;
        acc = HERNG_splitmix64_next(&acc);
    }

    /* Diffuse into state */
    st->s[0] ^= acc;
    st->s[1] ^= HERNG_rotl64(acc, 17);
    st->s[2] ^= HERNG_rotl64(acc, 33);
    st->s[3] ^= HERNG_rotl64(acc, 49);

    /* One step to re-diffuse */
    (void)HERNG_xoshiro256ss_next(st);
}

/* --- Random output --- */

static inline uint64_t HERNG_u64(HERNG_State *st) {
    return HERNG_xoshiro256ss_next(st);
}

static inline uint32_t HERNG_u32(HERNG_State *st) {
    /* Use high 32 bits to reduce correlation in low bits */
    return (uint32_t)(HERNG_xoshiro256ss_next(st) >> 32);
}

/* Unbiased integer in [0, bound). If bound == 0, returns 0. */
static inline uint32_t HERNG_bounded_u32(HERNG_State *st, uint32_t bound) {
    if (bound == 0) return 0;
    /* Rejection method using 32x32->64 multiply-high */
    const uint32_t threshold = (uint32_t)(-(uint32_t)bound) % bound;
    for (;;) {
        uint32_t r = HERNG_u32(st);
        uint64_t m = (uint64_t)r * (uint64_t)bound;
        uint32_t l = (uint32_t)m;
        if (l >= threshold) {
            return (uint32_t)(m >> 32);
        }
    }
}

/* Fill a byte buffer with random bytes. */
static inline void HERNG_bytes(HERNG_State *st, void *out, size_t len) {
    uint8_t *o = (uint8_t *)out;
    while (len >= 8) {
        uint64_t v = HERNG_u64(st);
        o[0] = (uint8_t)(v);
        o[1] = (uint8_t)(v >> 8);
        o[2] = (uint8_t)(v >> 16);
        o[3] = (uint8_t)(v >> 24);
        o[4] = (uint8_t)(v >> 32);
        o[5] = (uint8_t)(v >> 40);
        o[6] = (uint8_t)(v >> 48);
        o[7] = (uint8_t)(v >> 56);
        o += 8; len -= 8;
    }
    if (len > 0) {
        uint64_t v = HERNG_u64(st);
        for (size_t i = 0; i < len; ++i) o[i] = (uint8_t)(v >> (8 * i));
    }
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* HERNG_H */