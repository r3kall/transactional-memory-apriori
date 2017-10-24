# transactional-memory-apriori
Parallel Apriori algorithm with Software Transactional Memory (STM).

Original work at http://cucis.ece.northwestern.edu/projects/DMS/MineBench.html .

The original benchmark suite uses the OpenMP library to perform the Apriori algorithm in parallel over multiple processors and shared memory.

The aim of this project is to verify properties of the Software Transactional Memory invariants and to evaulate their performance. The STM library is provided by the GCC compiler: https://gcc.gnu.org/wiki/TransactionalMemory .
