#pragma once
#if (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 156) && (QRUOV_m == 54)
#  define QRUOV_tau1    4323 // n1 = L*V*(V+1)/2 :    4134
#  define QRUOV_tau2    2967 // n2 = L*V*M       :    2808
#  define QRUOV_tau3     218 // n3 = L*V = v     :     156
#  define QRUOV_tau4     104 // n4 = L*M = m     :      54
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 165) && (QRUOV_m == 60)
#  define QRUOV_tau1    5057 // n1 = L*V*(V+1)/2 :    4620
#  define QRUOV_tau2    3658 // n2 = L*V*M       :    3300
#  define QRUOV_tau3     264 // n3 = L*V = v     :     165
#  define QRUOV_tau4     136 // n4 = L*M = m     :      60
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 600) && (QRUOV_m == 70)
#  define QRUOV_tau1   19409 // n1 = L*V*(V+1)/2 :   18300
#  define QRUOV_tau2    4612 // n2 = L*V*M       :    4200
#  define QRUOV_tau3     755 // n3 = L*V = v     :     600
#  define QRUOV_tau4     149 // n4 = L*M = m     :      70
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 740) && (QRUOV_m == 100)
#  define QRUOV_tau1   33042 // n1 = L*V*(V+1)/2 :   27750
#  define QRUOV_tau2    9178 // n2 = L*V*M       :    7400
#  define QRUOV_tau3    1122 // n3 = L*V = v     :     740
#  define QRUOV_tau4     260 // n4 = L*M = m     :     100
#elif (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 228) && (QRUOV_m == 78)
#  define QRUOV_tau1    9115 // n1 = L*V*(V+1)/2 :    8778
#  define QRUOV_tau2    6207 // n2 = L*V*M       :    5928
#  define QRUOV_tau3     321 // n3 = L*V = v     :     228
#  define QRUOV_tau4     154 // n4 = L*M = m     :      78
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 246) && (QRUOV_m == 87)
#  define QRUOV_tau1   11047 // n1 = L*V*(V+1)/2 :   10209
#  define QRUOV_tau2    7803 // n2 = L*V*M       :    7134
#  define QRUOV_tau3     395 // n3 = L*V = v     :     246
#  define QRUOV_tau4     201 // n4 = L*M = m     :      87
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 890) && (QRUOV_m == 100)
#  define QRUOV_tau1   42267 // n1 = L*V*(V+1)/2 :   40050
#  define QRUOV_tau2    9667 // n2 = L*V*M       :    8900
#  define QRUOV_tau3    1123 // n3 = L*V = v     :     890
#  define QRUOV_tau4     218 // n4 = L*M = m     :     100
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 1100) && (QRUOV_m == 140)
#  define QRUOV_tau1   72168 // n1 = L*V*(V+1)/2 :   61050
#  define QRUOV_tau2   18858 // n2 = L*V*M       :   15400
#  define QRUOV_tau3    1672 // n3 = L*V = v     :    1100
#  define QRUOV_tau4     376 // n4 = L*M = m     :     140
#elif (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 306) && (QRUOV_m == 105)
#  define QRUOV_tau1   16283 // n1 = L*V*(V+1)/2 :   15759
#  define QRUOV_tau2   11141 // n2 = L*V*M       :   10710
#  define QRUOV_tau3     431 // n3 = L*V = v     :     306
#  define QRUOV_tau4     206 // n4 = L*M = m     :     105
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 324) && (QRUOV_m == 114)
#  define QRUOV_tau1   18986 // n1 = L*V*(V+1)/2 :   17658
#  define QRUOV_tau2   13361 // n2 = L*V*M       :   12312
#  define QRUOV_tau3     522 // n3 = L*V = v     :     324
#  define QRUOV_tau4     266 // n4 = L*M = m     :     114
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 1120) && (QRUOV_m == 120)
#  define QRUOV_tau1   66657 // n1 = L*V*(V+1)/2 :   63280
#  define QRUOV_tau2   14550 // n2 = L*V*M       :   13440
#  define QRUOV_tau3    1424 // n3 = L*V = v     :    1120
#  define QRUOV_tau4     274 // n4 = L*M = m     :     120
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 1490) && (QRUOV_m == 190)
#  define QRUOV_tau1  131440 // n1 = L*V*(V+1)/2 :  111750
#  define QRUOV_tau2   34303 // n2 = L*V*M       :   28310
#  define QRUOV_tau3    2260 // n3 = L*V = v     :    1490
#  define QRUOV_tau4     508 // n4 = L*M = m     :     190
#else
#  error "unknown (q,L,v,m)"
#endif
