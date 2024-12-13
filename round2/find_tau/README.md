### find\_tau : 

Find $\tau=\min[t\in\mathbb{N}\mid{}P(n,t,p)\le2^{-\lambda}]$
where $P$ is the probability of less than $n$ successes in $t$
independent Bernoulli trials of success probability $p$.

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
