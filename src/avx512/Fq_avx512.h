#define DEBUG 0

#if DEBUG
#pragma  once
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <x86intrin.h>

#if DEBUG
#  define QRUOV_q 127
#  if   (QRUOV_q == 7)
#    define QRUOV_ceil_log_2_q 3
#  elif (QRUOV_q == 31)
#    define QRUOV_ceil_log_2_q 5
#  elif (QRUOV_q == 127)
#    define QRUOV_ceil_log_2_q 7
#  else
#    error "Unsupported: QRUOV_q == " # QRUOV_q
#  endif
#else

#include "qruov.h"

#endif

inline static int _m256i_eq(__m256i a, __m256i b){
  __m256i  c = _mm256_cmpeq_epi64(a,b) ;
  __m128i  d = _mm256_castsi256_si128(c) ;
  __m128i  e = _mm256_extracti128_si256(c, 1) ;
  __m128i  f = _mm_or_si128(d,e) ;
  __m128i  g = _mm_shuffle_epi32(f, _MM_SHUFFLE(1,0,3,2)) ;
  __m128i  h = _mm_or_si128(f,g) ;
  __m128i  i = _mm_hadd_epi32(h, h) ;
  return _mm_extract_epi32(i, 0) ;
}

inline static int _m256i_ne(__m256i a, __m256i b){ return ! _m256i_eq(a, b) ; }
inline static int _m512i_eq(__m512i a, __m512i b){ return (_mm512_cmpeq_epi64_mask(a, b) == 0xFF) ; }
inline static int _m512i_ne(__m512i a, __m512i b){ return (_mm512_cmpneq_epi64_mask(a, b) == 0xFF) ; }

__m256i _m256i_Fq_add_0(__m256i a_, __m256i b_){
  uint16_t * a = (uint16_t *)&a_ ;
  uint16_t * b = (uint16_t *)&b_ ;
  uint16_t c[16] __attribute__ ((aligned (32))) ;
  for(int i=0;i<16;i++){
    c[i] = ((uint64_t)a[i] + (uint64_t)b[i]) % QRUOV_q ;
  }
  return _mm256_setr_epi16(
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],
    c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
  ) ;
}

__m512i _m512i_Fq_add_0(__m512i a_, __m512i b_){
  uint16_t * a = (uint16_t *)&a_ ;
  uint16_t * b = (uint16_t *)&b_ ;
  uint16_t c[32] __attribute__ ((aligned (sizeof(__m512i)))) ;
  for(int i=0;i<32;i++){
    c[i] = ((uint64_t)a[i] + (uint64_t)b[i]) % QRUOV_q ;
  }
  uint32_t d[16] __attribute__ ((aligned (sizeof(__m512i)))) ;
  for(int i=0;i<16;i++){
    uint32_t l = c[2*i] ;
    uint32_t h = c[2*i+1] ;
    uint32_t hl = ((h << 16) | l) ;
    d[i] = hl ;
  }
  return _mm512_setr_epi32(
    d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7],
    d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]
  ) ;
}

__m256i _m256i_Fq_sub_0(__m256i a_, __m256i b_){
  uint16_t * a = (uint16_t *)&a_ ;
  uint16_t * b = (uint16_t *)&b_ ;
  uint16_t c[16] __attribute__ ((aligned (32))) ;
  for(int i=0;i<16;i++){
    c[i] = ((uint64_t)a[i] - (uint64_t)b[i] + QRUOV_q) % QRUOV_q ;
  }
  return _mm256_setr_epi16(
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],
    c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
  ) ;
}

__m512i _m512i_Fq_sub_0(__m512i a_, __m512i b_){
  uint16_t * a = (uint16_t *)&a_ ;
  uint16_t * b = (uint16_t *)&b_ ;
  uint16_t c[32] __attribute__ ((aligned (sizeof(__m512i)))) ;
  for(int i=0;i<32;i++){
    c[i] = ((uint64_t)a[i] - (uint64_t)b[i] + QRUOV_q) % QRUOV_q ;
  }
  uint32_t d[16] __attribute__ ((aligned (sizeof(__m512i)))) ;
  for(int i=0;i<16;i++){
    uint32_t l = c[2*i] ;
    uint32_t h = c[2*i+1] ;
    uint32_t hl = ((h << 16) | l) ;
    d[i] = hl ;
  }
  return _mm512_setr_epi32(
    d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7],
    d[8], d[9], d[10], d[11], d[12], d[13], d[14], d[15]
  ) ;
}

__m256i _m256i_Fq_mul_0(__m256i a_, __m256i b_){
  uint16_t * a = (uint16_t *)&a_ ;
  uint16_t * b = (uint16_t *)&b_ ;
  uint16_t c[16] __attribute__ ((aligned (32))) ;
  for(int i=0;i<16;i++){
    c[i] = ((uint64_t)a[i] * (uint64_t)b[i]) % QRUOV_q ;
  }
  return _mm256_setr_epi16(
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],
    c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
  ) ;
}

inline static __m256i _m256i_Fq_random(){
    return _mm256_setr_epi16(
      random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q,
      random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q,
      random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q,
      random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q, random() % QRUOV_q
    ) ;
}

inline static __m512i _m512i_Fq_random(){
  uint32_t c[16] __attribute__ ((aligned (sizeof(__m512i)))) ;
  for(int i=0;i<16;i++){
    uint64_t h = random() % QRUOV_q ;
    uint64_t l = random() % QRUOV_q ;
    uint64_t a = ((h << 16) | l) ;
    c[i] = (uint32_t) a ;
  }
  return _mm512_setr_epi32(
    c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7],
    c[8], c[9], c[10], c[11], c[12], c[13], c[14], c[15]
  ) ;
}

inline static __m256i _m256i_Fq_mul(__m256i a, __m256i b){
    __m256i c = _mm256_slli_epi16(a, 16-QRUOV_ceil_log_2_q) ;
    __m256i d = _mm256_mullo_epi16(c, b) ;
    __m256i e = _mm256_mulhi_epu16(c, b) ;
    __m256i f = _mm256_srli_epi16(d, 16-QRUOV_ceil_log_2_q) ;
    __m256i g = _mm256_add_epi16(e,f) ;
    __m256i h = _mm256_set1_epi16(QRUOV_q-1) ;
    __m256i j = _mm256_cmpgt_epi16(g,h);
    __m256i k = _mm256_srli_epi16(j, 16-QRUOV_ceil_log_2_q) ;
    __m256i l = _mm256_sub_epi16(g, k) ;
    return l ;
}

inline static __m512i _m512i_Fq_mul(__m512i a, __m512i b){
    __m512i c = _mm512_slli_epi16(a, 16-QRUOV_ceil_log_2_q) ;
    __m512i d = _mm512_mullo_epi16(c, b) ;
    __m512i e = _mm512_mulhi_epu16(c, b) ;
    __m512i f = _mm512_srli_epi16(d, 16-QRUOV_ceil_log_2_q) ;
    __m512i g = _mm512_add_epi16(e,f) ;
    __m512i h = _mm512_set1_epi16(QRUOV_q-1) ;
    __mmask32 j = _mm512_cmpgt_epi16_mask(g,h);
    __m512i k = _mm512_movm_epi16(j) ;
    __m512i l = _mm512_srli_epi16(k, 16-QRUOV_ceil_log_2_q) ;
    __m512i m = _mm512_sub_epi16(g, l) ;
    return m ;
}

inline static __m256i _m256i_Fq_add(__m256i a, __m256i b){
    __m256i c = _mm256_add_epi16(a, b) ;
    __m256i d = _mm256_set1_epi16(QRUOV_q-1) ;
    __m256i e = _mm256_cmpgt_epi16(c,d);
    __m256i f = _mm256_srli_epi16(e, 16-QRUOV_ceil_log_2_q) ;
    __m256i g = _mm256_sub_epi16(c, f) ;
    return g ;
}

inline static __m512i _m512i_Fq_add(__m512i a, __m512i b){
    __m512i   c = _mm512_add_epi16(a, b) ;
    __m512i   d = _mm512_set1_epi16(QRUOV_q-1) ;
    __mmask32 e = _mm512_cmpgt_epi16_mask(c,d);
    __m512i   f = _mm512_movm_epi16(e) ;
    __m512i   g = _mm512_srli_epi16(f, 16-QRUOV_ceil_log_2_q) ;
    __m512i   h = _mm512_sub_epi16(c, g) ;
    return h ;
}

inline static __m256i _m256i_Fq_sub(__m256i a, __m256i b){
    __m256i c = _mm256_sub_epi16(a, b) ;
    __m256i d = _mm256_setzero_si256() ;
    __m256i e = _mm256_cmpgt_epi16(d,c);
    __m256i f = _mm256_srli_epi16(e, 16-QRUOV_ceil_log_2_q) ;
    __m256i g = _mm256_add_epi16(c, f) ;
    return g ;
}

inline static __m512i _m512i_Fq_sub(__m512i a, __m512i b){
    __m512i   c = _mm512_sub_epi16(a, b) ;
    __m512i   d = _mm512_setzero_si512() ;
    __mmask32 e = _mm512_cmpgt_epi16_mask(d,c) ;
    __m512i   f = _mm512_movm_epi16(e) ;
    __m512i   g = _mm512_srli_epi16(f, 16-QRUOV_ceil_log_2_q) ;
    __m512i   h = _mm512_add_epi16(c, g) ;
    return h ;
}

void print_m256i(char * header, __m256i a_){
  uint16_t * a = (uint16_t *) & a_ ;
  printf("%s%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
    header, 
    a[15], a[14], a[13], a[12], a[11], a[10], a[9], a[8],
    a[7],  a[6],  a[5],  a[4],  a[3],  a[2],  a[1], a[0]
  );
}

void print_m512i(char * header, __m512i a_){
//  alignas(alignof(__m256i)) 
  uint16_t * a = (uint16_t *) & a_ ;
  printf("%s%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x"
          " %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
    header, 
    a[31], a[30], a[29], a[28], a[27], a[26], a[25], a[24],
    a[23], a[22], a[21], a[20], a[19], a[18], a[17], a[16],
    a[15], a[14], a[13], a[12], a[11], a[10], a[9],  a[8],
    a[7],  a[6],  a[5],  a[4],  a[3],  a[2],  a[1],  a[0]
  );
}

#define PRINT_m256i(a) print_m256i( #a " = ", a )
#define PRINT_m512i(a) print_m512i( #a " = ", a )

#if DEBUG
int main(){

  for(uint64_t i=0; i<1000000ULL ; i++){
    __m512i a = _m512i_Fq_random() ;
    __m512i b = _m512i_Fq_random() ;
    __m512i c = _m512i_Fq_mul(a, b) ;
    __m512i d = _m512i_Fq_mul(a, b) ;

//    __m512i c = _m512i_Fq_random() ;
//    __m512i d = _m512i_Fq_random() ;

    if(_m512i_ne(c, d)){
      PRINT_m512i(a) ;
      PRINT_m512i(b) ;
      PRINT_m512i(c) ;
      PRINT_m512i(d) ;
    }

  }
  return 0 ;
}
#endif
