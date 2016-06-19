# Algorithm Exercises
#### Currently implemented algorithms
- C++
  - Heapsort
    - Heapify-down implemented as non-recursive
  - Quicksort
    - Uses the Hoare partition scheme
    - Uses median-of-three pivot selection (considering implementing the ninther for larger lists)
  - Insertion sort
  - Introsort
    - Using C++ STL implementation
    - Quicksort is performed to a maximum depth given by 2×log2(n), then heapsort is done on any list longer than 16, finally an insertion sort pass
  - Bogosort
    - Random & permutation methods implemented
    - Removed from timeTest.cpp as it is horribly slow
      - Using randomization, given input of length `n` made of random numbers generated in the range `[a,b]`, the expected number of iterations is `(b-a+1)^n / ((b-a+1)+n choose (b-a+1))`
      - Ex. `length=100 & range=[1,10]`, expected number of iterations is `10^100 / (110 choose 10)` which equals `2.13 × 10^96`
  - Bubble sort