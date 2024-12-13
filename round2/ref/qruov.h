#pragma once

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qruov_misc.h"
#include "qruov_tau.h"
#include "Fql.h"
#include "matrix.h"
#include "mgf.h"

/* =====================================================================
   pseudo random generator
   ===================================================================== */

// #define QRUOV_PRG_SHAKE

#ifndef QRUOV_PRG_SHAKE
#  define QRUOV_PRG_CTX  EVP_CIPHER_CTX
#  define PRG_init       PRG_init_aes_ctr
#  define PRG_yield      PRG_yield_aes_ctr
#  define PRG_final      PRG_final_aes_ctr
#  define PRG_copy       PRG_copy_aes_ctr
#  define RejSampPRG     RejSampPRG_aes_ctr
#  define QRUOV_PRG2_CTX EVP_CIPHER_CTX
#  define PRG2_init      PRG_init_aes_ctr
#  define PRG2_yield     PRG_yield_aes_ctr
#  define PRG2_final     PRG_final_aes_ctr
#  define PRG2_copy      PRG_copy_aes_ctr
#else
#  define QRUOV_PRG_CTX  EVP_MD_CTX
#  define PRG_init       PRG_init_shake
#  define PRG_yield      PRG_yield_shake
#  define PRG_final      PRG_final_shake
#  define PRG_copy       PRG_copy_shake
#  define RejSampPRG     RejSampPRG_shake
#  define QRUOV_PRG2_CTX MGF_CTX_s
#  define PRG2_init      PRG_init_mgf
#  define PRG2_yield     PRG_yield_mgf
#  define PRG2_final     PRG_final_mgf
#  define PRG2_copy      PRG_copy_mgf
#endif

#if (QRUOV_security_strength_category == 1)
#  define EVP_AES_CTR  EVP_aes_128_ctr
#  define EVP_SHAKE    EVP_shake128
#elif (QRUOV_security_strength_category == 3)
#  define EVP_AES_CTR  EVP_aes_192_ctr
#  define EVP_SHAKE    EVP_shake256
#elif (QRUOV_security_strength_category == 5)
#  define EVP_AES_CTR  EVP_aes_256_ctr
#  define EVP_SHAKE    EVP_shake256
#else
#  error "illegal QRUOV_security_strength_category"
#endif

/* =====================================================================
   QRUOV functions
   ===================================================================== */

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
  // ---------------------------------------------------
  const QRUOV_SEED seed_sk,        // input (signing key)
  const QRUOV_SEED seed_pk,        // input (signing key)
  // ---------------------------------------------------
  const QRUOV_SEED seed_y,         // input (random (F_q)^v)
  const QRUOV_SEED seed_r,         // input (random byte)
  const QRUOV_SEED seed_sol,       // input (random (F_q)^*)
  // ---------------------------------------------------
  const uint8_t    message[],      // input (message)
  const size_t     message_length, // input (message)
  // ---------------------------------------------------
  QRUOV_SIGNATURE  sig             // output
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

#if QRUOV_L == 3

inline static void store_Fql(
  Fql       X,
  uint8_t       * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  Fq x0 = (Fq)X & QRUOV_q ; X >>= Fql_ws ;
  Fq x1 = (Fq)X & QRUOV_q ; X >>= Fql_ws ;
  Fq x2 = (Fq)X & QRUOV_q ;
  store_Fq(x0, pool, pool_bits) ;
  store_Fq(x1, pool, pool_bits) ;
  store_Fq(x2, pool, pool_bits) ;
}

inline static Fql restore_Fql(
  const uint8_t * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  Fql a0 = restore_Fq(pool, pool_bits) ;
  Fql a1 = restore_Fq(pool, pool_bits) ; a1 <<= (Fql_ws) ;
  Fql a2 = restore_Fq(pool, pool_bits) ; a2 <<= (Fql_ws*2) ;
  return a0|a1|a2 ;
}

#elif QRUOV_L == 10

inline static void store_Fql(
  Fql       X,
  uint8_t       * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  for(int i=0; i<QRUOV_L; i++){
    Fq x = Fql2Fq(X,i) ;
    store_Fq(x, pool, pool_bits) ;
  }
}

inline static Fql restore_Fql(
  const uint8_t * pool,            // bit pool
  size_t        * pool_bits        // current bit index
){
  uint16_t a[QRUOV_L] ;
  for(int i=0; i<QRUOV_L; i++){
    a[i] = restore_Fq(pool, pool_bits) ;
  }
  return Fq2Fql(a) ;
}
#else
#  error "unsupported QRUOV_L in qruov.h: store_Fql(), restore_Fql()"
#endif

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
