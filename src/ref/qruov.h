#pragma once

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qruov_config.h"

/* =====================================================================
   tools
   ===================================================================== */

#define TYPEDEF_STRUCT(TYPE_NAME, BODY)             \
typedef struct TYPE_NAME ## _t {                    \
  BODY                                              \
} TYPE_NAME ## _s, * TYPE_NAME ## _p, TYPE_NAME [1]

#define ERROR_ABORT(MESSAGE) { \
  fprintf(stderr, "runtime error: %s in file: %s, line: %d.\n",MESSAGE,__FILE__,__LINE__); \
  abort(); \
}

/* =====================================================================
   mgf
   ===================================================================== */

#ifdef QRUOV_HASH_LEGACY

#  if QRUOV_SEED_LEN == 32
#    define QRUOV_MGF_CORE EVP_sha256
/*
#    define HASH_CTX    SHA256_CTX
#    define HASH_Init   SHA256_Init
#    define HASH_Update SHA256_Update
#    define HASH_Final  SHA256_Final
*/
#  elif QRUOV_SEED_LEN == 48
#    define QRUOV_MGF_CORE EVP_sha384
/*
#    define HASH_CTX    SHA512_CTX
#    define HASH_Init   SHA384_Init
#    define HASH_Update SHA384_Update
#    define HASH_Final  SHA384_Final
*/
#  elif QRUOV_SEED_LEN == 64
#    define QRUOV_MGF_CORE EVP_sha512
/*
#    define HASH_CTX    SHA512_CTX
#    define HASH_Init   SHA512_Init
#    define HASH_Update SHA512_Update
#    define HASH_Final  SHA512_Final
*/
#  else
#    error "Unsupported: QRUOV_SEED_LEN == " # QRUOV_SEED_LEN
#  endif

/*
TYPEDEF_STRUCT (MGF_CTX,
  uint32_t counter ;
  HASH_CTX hash_ctx [1] ;
  uint32_t pool_bytes ;
  uint8_t  pool [QRUOV_SEED_LEN] ;
) ;
*/

#define QRUOV_MGF_POOLSIZE (QRUOV_SEED_LEN)

#else

/*
  SHAKE128: bsz = 168
  SHAKE256: bsz = 136
  unsigned char md [bsz+1]
*/

#  if   QRUOV_SEED_LEN == 32
#    define QRUOV_MGF_CORE  EVP_shake128
#    define QRUOV_SHAKE_BSZ 168
#  elif ( QRUOV_SEED_LEN == 48 ) || (QRUOV_SEED_LEN == 64)
#    define QRUOV_MGF_CORE  EVP_shake256
#    define QRUOV_SHAKE_BSZ 136
#  else
#    error "Unsupported: QRUOV_SEED_LEN == " # QRUOV_SEED_LEN
#  endif

#define QRUOV_MGF_POOLSIZE (QRUOV_SHAKE_BSZ + 1)

#endif

TYPEDEF_STRUCT (MGF_CTX,
  uint32_t counter    ;
  EVP_MD_CTX * mdctx  ;
  uint32_t pool_bytes ;
  uint8_t  pool [ QRUOV_MGF_POOLSIZE ] ;
) ;

extern MGF_CTX_s * MGF_init  (const uint8_t * seed, const size_t n0, MGF_CTX ctx) ;
extern MGF_CTX_s * MGF_update(const uint8_t * seed, const size_t n0, MGF_CTX ctx) ;
extern uint8_t   * MGF_yield (MGF_CTX ctx, uint8_t * dest, const size_t n1) ;
extern void MGF_final(MGF_CTX ctx) ;
extern void MGF_CTX_copy(MGF_CTX src, MGF_CTX dst) ;

/* =====================================================================
   F_q, F_q[X]/(f(X))
   ===================================================================== */

typedef int8_t Fq ;

typedef struct Fql_t {
  Fq c[QRUOV_L] ;
} Fql ;

extern Fql Fql_zero ;

inline static Fq Fq_add(Fq X, Fq Y){
  int Z = (int) X + (int) Y ;
  Z %= QRUOV_q ;
  return (Fq)Z ;
}

inline static Fq Fq_sub(Fq X, Fq Y){
  int Z = (int) X - (int) Y + QRUOV_q ;
  Z %= QRUOV_q ;
  return (Fq)Z ;
}

inline static Fq Fq_mul(Fq X, Fq Y){
  int Z = (int) X * (int) Y ;
  Z %= QRUOV_q ;
  return (Fq)Z ;
}

inline static Fq Fq_inv(Fq X){
  extern Fq Fq_inv_table[QRUOV_q] ;
  return Fq_inv_table[X] ;
}

inline static void Fq_vector_add(const size_t n, Fq * X, Fq * Y, Fq * Z){
  for(size_t i=0; i<n; i++) Z[i] = Fq_add(X[i], Y[i]);
}

inline static void Fq_vector_sub(const size_t n, Fq * X, Fq * Y, Fq * Z){
  for(size_t i=0; i<n; i++) Z[i] = Fq_sub(X[i], Y[i]);
}

inline static Fq Fq_vector_inner_product(const size_t n, Fq * X, Fq * Y){
  uint64_t t = 0 ;
  for(size_t i=0;i<n;i++) t += (uint64_t) X[i] * (uint64_t) Y[i] ;
  return (Fq)(t % QRUOV_q) ;
}

inline static Fql Fql_add(Fql X, Fql Y){
  Fql Z ;
  Fq_vector_add(QRUOV_L, X.c, Y.c, Z.c);
  return Z ;
}

inline static Fql Fql_sub(Fql X, Fql Y){
  Fql Z ;
  Fq_vector_sub(QRUOV_L, X.c, Y.c, Z.c);
  return Z ;
}

inline static Fql Fql_mul(Fql X, Fql Y){
  uint64_t T[2*QRUOV_L-1] ;
  memset(T, 0, sizeof(uint64_t)*(2*QRUOV_L-1)) ;

  for(size_t i=0; i<QRUOV_L; i++){
    for(size_t j=0; j<QRUOV_L; j++){
      T[i+j] += (uint64_t) X.c[i] * (uint64_t) Y.c[j] ;
    }
  }

  for(size_t i = 2*QRUOV_L-2; i >= QRUOV_L; i--){
      T[i-QRUOV_L]          += QRUOV_fc0 * T[i] ;
      T[i-QRUOV_L+QRUOV_fe] += QRUOV_fc  * T[i] ;
  }

  Fql Z ;
  for(size_t i=0; i<QRUOV_L; i++){
    Z.c[i] = (Fq)(T[i] % QRUOV_q) ;
  }
  return Z ;
}

/* =====================================================================
   pseudo random number generator
   ===================================================================== */

TYPEDEF_STRUCT ( Fql_RANDOM_CTX,
  MGF_CTX   mgf_ctx ;
  unsigned  pool_bits ;
  uint64_t  pool ;
) ;

typedef uint8_t QRUOV_SEED  [QRUOV_SEED_LEN] ;
typedef uint8_t QRUOV_SALT  [QRUOV_SALT_LEN] ;

inline static void Fql_srandom_init(const uint8_t * seed, const size_t n0, Fql_RANDOM_CTX ctx){
  MGF_init(seed, n0, ctx->mgf_ctx) ;
  ctx->pool      = 0 ;
  ctx->pool_bits = 0 ;
  return ;
}

inline static void Fql_srandom(const QRUOV_SEED seed, Fql_RANDOM_CTX ctx){
  Fql_srandom_init(seed, QRUOV_SEED_LEN, ctx) ;
  return ;
}

inline static void Fql_srandom_update(const uint8_t * seed, const size_t n0, Fql_RANDOM_CTX ctx){
  MGF_update(seed, n0, ctx->mgf_ctx) ;
  return ;
}

/* random bits -> {0,...,q-1} */
extern Fq Fq_random (Fql_RANDOM_CTX ctx) ;

/* random bits -> (1) */
extern Fql   Fql_random (Fql_RANDOM_CTX ctx) ;
extern Fql * Fql_random_vector (Fql_RANDOM_CTX ctx, const size_t n0, Fql vec[]) ;

inline static void Fq_random_final (Fql_RANDOM_CTX ctx) {
  MGF_final(ctx->mgf_ctx) ;
  ctx->pool = 0 ;
}

inline static void Fql_random_final (Fql_RANDOM_CTX ctx) {
  Fq_random_final (ctx) ;
}

inline static void Fql_RANDOM_CTX_copy (Fql_RANDOM_CTX src, Fql_RANDOM_CTX dst) {
  memcpy(dst, src, sizeof(Fql_RANDOM_CTX)) ;
  MGF_CTX_copy(src->mgf_ctx, dst->mgf_ctx) ;
}

/* =====================================================================
   QRUOV functions
   ===================================================================== */

TYPEDEF_STRUCT(QRUOV_SIGNATURE,
  QRUOV_SALT r           ;
  Fql        s [QRUOV_N] ;
) ;

typedef Fql        VECTOR_V        [QRUOV_V]        ;
typedef Fql        VECTOR_M        [QRUOV_M]        ;
typedef VECTOR_V   MATRIX_VxV      [QRUOV_V]        ;
typedef VECTOR_V   MATRIX_MxV      [QRUOV_M]        ;
typedef VECTOR_M   MATRIX_VxM      [QRUOV_V]        ;
typedef VECTOR_M   MATRIX_MxM      [QRUOV_M]        ;

extern void MATRIX_MUL_MxV_VxV(MATRIX_MxV A, MATRIX_VxV B, MATRIX_MxV C) ;
extern void MATRIX_MUL_MxV_VxM(MATRIX_MxV A, MATRIX_VxM B, MATRIX_MxM C) ;
extern void MATRIX_MUL_ADD_MxV_VxM(MATRIX_MxV A, MATRIX_VxM B, MATRIX_MxM C) ; // C += A*B
extern void MATRIX_SUB_MxV(MATRIX_MxV A, MATRIX_MxV B, MATRIX_MxV C) ;
extern void MATRIX_ADD_MxM(MATRIX_MxM A, MATRIX_MxM B, MATRIX_MxM C) ;
extern void MATRIX_TRANSPOSE_VxM(MATRIX_VxM A, MATRIX_MxV C) ;

typedef MATRIX_VxM QRUOV_Sd                         ;
typedef MATRIX_MxV QRUOV_SdT                        ;
typedef MATRIX_VxV QRUOV_P1        [QRUOV_m]        ;
typedef MATRIX_VxM QRUOV_P2        [QRUOV_m]        ;
typedef MATRIX_MxV QRUOV_P2T       [QRUOV_m]        ;
typedef MATRIX_MxM QRUOV_P3        [QRUOV_m]        ;

//
// SECRET       KEY : (sk_seed,        ,   )
// SIGNING      KEY : (sk_seed, pk_seed,   )
// VERIFICATION KEY : (       , pk_seed, P3)
//

extern void QRUOV_KeyGen(
  const QRUOV_SEED sk_seed,      // input
  const QRUOV_SEED pk_seed,      // input
  QRUOV_P3         P3            // output
) ;

extern void QRUOV_Sign(
  const QRUOV_SEED sk_seed,      // input
  const QRUOV_SEED pk_seed,      // input
                                 //
  const QRUOV_SEED vineger_seed, // input
  const QRUOV_SEED r_seed,       // input
                                 //
  const uint8_t    Msg[],        // input
  const size_t     Msg_len,      // input
                                 //
  QRUOV_SIGNATURE  sig           // output
) ;

extern int QRUOV_Verify(         // NG:0, OK:1,
  const QRUOV_SEED      pk_seed, // input
  const QRUOV_P3        P3,      // input
                                 //
  const uint8_t         Msg[],   // input
  const size_t          Msg_len, // input
                                 //
  const QRUOV_SIGNATURE sig      // input
) ;

/* =====================================================================
   memory I/O
   ===================================================================== */

inline static void store_bits(
  int             x,               // \in {0,...,255}
  const int       num_bits,        // \in {0,...,8}
  uint8_t       * pool,            //
  size_t        * pool_bits
){
  int    shift = (int)(*pool_bits &  7) ;
  size_t index = (*pool_bits >> 3) ;
  int    mask  = (1<<num_bits) - 1 ;

  x &= mask ;
  x <<= shift ;
  uint8_t x0 = (x & 0xFF) ;
  if(shift == 0){
    pool[index] = x0 ;
  }else{
    pool[index] |= x0 ;
  }
  if(shift + num_bits > 8){
    uint8_t x1 = (x >> 8) ;
    pool[index+1] = x1 ;
  }

  *pool_bits += (size_t) num_bits ;
}

inline static int restore_bits(     // \in {0,...,255}
  const uint8_t * pool,
  size_t        * pool_bits,
  const int       num_bits          // \in {0,...,8}
){
  int    shift = (int)(*pool_bits &  7) ;
  size_t index = (*pool_bits >> 3) ;
  int    mask  = (1<<num_bits) - 1 ;

  int x        = ((int) pool[index]) 
               | (((shift + num_bits > 8) ? (int)pool[index+1] : 0) << 8) ;
  x >>= shift ;
  x &= mask ; 
  *pool_bits += (size_t) num_bits ;
  return x ;
}


inline static void store_Fq(
  int             x,               // \in Fq
  uint8_t       * pool,
  size_t        * pool_bits
){
  store_bits(x, QRUOV_ceil_log_2_q, pool, pool_bits) ;
}

inline static Fq restore_Fq(
  const uint8_t * pool,
  size_t        * pool_bits
){
  return (Fq) restore_bits(pool, pool_bits, QRUOV_ceil_log_2_q) ;
}

inline static void store_Fq_vector(
  const Fq  * X,
  const int   n,
  uint8_t   * pool,
  size_t    * pool_bits
){
  for(size_t i=0; i<n; i++) store_Fq(X[i], pool, pool_bits) ;
}

inline static void restore_Fq_vector(
  const uint8_t * pool,
  size_t        * pool_bits,
  Fq            * Z,
  const size_t    n
){
  for(size_t i=0; i<n; i++) Z[i] = restore_Fq(pool, pool_bits) ;
}

inline static void store_Fql(
  const Fql       X,
  uint8_t       * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  store_Fq_vector(X.c, QRUOV_L, pool, pool_bits) ;
}

inline static Fql restore_Fql(
  const uint8_t * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  Fql Z ;
  restore_Fq_vector(pool, pool_bits, Z.c, QRUOV_L) ;
  return Z ;
}

inline static void store_Fql_vector(
  const Fql    * X,
  const size_t   n,
  uint8_t      * pool,
  size_t       * pool_bits
){
  for(size_t i=0; i<n; i++) store_Fql(X[i], pool, pool_bits) ;
}

inline static void restore_Fql_vector(
  const uint8_t * pool,
  size_t        * pool_bits,
  Fql           * Z,
  const size_t    n
){
  for(size_t i=0; i<n; i++) Z[i] = restore_Fql(pool, pool_bits) ;
}

inline static void store_QRUOV_SEED (
  const QRUOV_SEED   seed,      // input
  uint8_t          * pool,      // output
  size_t           * pool_bits  // must be 8*n
){
  size_t index = (*pool_bits >> 3) ;
  memcpy(pool+index, seed, QRUOV_SEED_LEN) ;
  *pool_bits += (QRUOV_SEED_LEN<<3) ;
}

inline static void restore_QRUOV_SEED (
  const uint8_t * pool,      // input
  size_t        * pool_bits, // must be 8*n
  QRUOV_SEED      seed       // output
){
  size_t index = (*pool_bits >> 3) ;
  memcpy(seed, pool+index, QRUOV_SEED_LEN) ;
  *pool_bits += (QRUOV_SEED_LEN<<3) ;
}

inline static void store_QRUOV_SALT (
  const QRUOV_SALT   salt,      // input
  uint8_t          * pool,      // output
  size_t           * pool_bits  // must be 8*n
){
  size_t index = (*pool_bits >> 3) ;
  memcpy(pool+index, salt, QRUOV_SALT_LEN) ;
  *pool_bits += (QRUOV_SALT_LEN<<3) ;
}

inline static void restore_QRUOV_SALT (
  const uint8_t * pool,      // input
  size_t        * pool_bits, // must be 8*n
  QRUOV_SALT      salt       // output
){
  size_t index = (*pool_bits >> 3) ;
  memcpy(salt, pool+index, QRUOV_SALT_LEN) ;
  *pool_bits += (QRUOV_SALT_LEN<<3) ;
}

extern void store_QRUOV_P3(
  const QRUOV_P3   P3,        // input
  uint8_t        * pool,      // output
  size_t         * pool_bits  //
);

extern void restore_QRUOV_P3(
  const uint8_t * pool,      // input
  size_t        * pool_bits, // input (current bit index)
  QRUOV_P3        P3         // output
);

inline static void store_QRUOV_SIGNATURE(
  const QRUOV_SIGNATURE   sig,      // input
  uint8_t               * pool,     // output
  size_t                * pool_bits // output (current bit index) // must be 8*n
){
  store_QRUOV_SALT (sig->r, pool, pool_bits) ;
  for(size_t i=0; i<QRUOV_N; i++) store_Fql(sig->s[i], pool, pool_bits) ;
  return ;
}

inline static void restore_QRUOV_SIGNATURE(
  const uint8_t   * pool,      // input
  size_t          * pool_bits, // input (current bit index) // must be 8*n
  QRUOV_SIGNATURE   sig        // output
){
  restore_QRUOV_SALT (pool, pool_bits, sig->r) ;
  for(size_t i=0; i<QRUOV_N; i++) sig->s[i] = restore_Fql(pool, pool_bits) ;
  return ;
}

#define BITS2BYTES(BITS)      (((BITS)>>3)+(((BITS)&7)?1:0))
