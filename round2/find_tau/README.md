### find\_tau : 

For given $q,\lambda,n$, find\_tau finds $`\tau_{q,\lambda}(n):=\min{}\{t\in\mathbb{N}\mid{}P(n,t,q/2^{\lceil\log_2q\rceil})\le2^{-\lambda}\}`$ where
$P(n,t,p)$ is the cumulative binomial distribution.

$$ P(n,t,p) := \sum_{i=0}^{n-1} \binom{t}{i} p^i (1-p)^{t-i} = I_{1-p}(t-n+1,n), $$

which denotes the probability of less than $n'$ successes in $t$ independent Bernoulli trials
of success probability $p$. $I_{z}(a,b)$ is called the regularized incomplete beta function
which is well-known in statistics, and many numerical packages provide functions to compute it.

How to build:

0. Install a C compiler if necessary. For example, if you are using
   ubuntu, type the following into the command line shell.

   $ sudo apt -y install build-essential

1. Install "GNU Scientific Library (GSL) -- development package"
   if necessary. For example, if you are using ubuntu, type the
   following into the command line shell.

   $ sudo apt -y install libgsl-dev

2. Make it if necessary. For example, if you are using ubuntu,
   type the following into the command line shell.

   $ make
