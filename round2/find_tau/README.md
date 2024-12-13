### find\_tau : 

Find $\tau=\min(t)$ s.t. $\lambda\le-\log_2P(n,t,p)$ where $P(n,t,p)$ is
the probability $P$ of observing $n$ or grater than $n$ independent
events each of probability $p$ after $t$ Bernoulli trials.

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
