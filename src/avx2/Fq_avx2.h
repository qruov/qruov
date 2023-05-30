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

inline static int _m256i_ne(__m256i a_, __m256i b_){ return ! _m256i_eq(a_, b_) ; }

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

inline static __m256i _m256i_Fq_add(__m256i a, __m256i b){
    __m256i c = _mm256_add_epi16(a, b) ;
    __m256i d = _mm256_set1_epi16(QRUOV_q-1) ;
    __m256i e = _mm256_cmpgt_epi16(c,d);
    __m256i f = _mm256_srli_epi16(e, 16-QRUOV_ceil_log_2_q) ;
    __m256i g = _mm256_sub_epi16(c, f) ;
    return g ;
}

inline static __m256i _m256i_Fq_sub(__m256i a, __m256i b){
    __m256i c = _mm256_sub_epi16(a, b) ;
    __m256i d = _mm256_setzero_si256() ;
    __m256i e = _mm256_cmpgt_epi16(d,c);
    __m256i f = _mm256_srli_epi16(e, 16-QRUOV_ceil_log_2_q) ;
    __m256i g = _mm256_add_epi16(c, f) ;
    return g ;
}

void print_m256i(char * header, __m256i a_){
//  alignas(alignof(__m256i)) 
  uint16_t * a = (uint16_t *) & a_ ;
  printf("%s%04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x %04x\n",
    header, 
    a[15], a[14], a[13], a[12], a[11], a[10], a[9], a[8],
    a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0]
  );
}

#define PRINT_m256i(a) print_m256i( #a " = ", a )

#if DEBUG
int main(){

  for(uint64_t i=0; i<10000000ULL ; i++){
    __m256i a = _m256i_Fq_random() ;
    __m256i b = _m256i_Fq_random() ;
    __m256i c = _m256i_Fq_sub(a, b) ;
    __m256i d = _m256i_Fq_sub_0(a, b) ;

//    __m256i c = _m256i_Fq_random() ;
//    __m256i d = _m256i_Fq_random() ;

    if(_m256i_ne(c, d)){
      PRINT_m256i(a) ;
      PRINT_m256i(b) ;
      PRINT_m256i(c) ;
      PRINT_m256i(d) ;
    }

  }
  return 0 ;
}
#endif
