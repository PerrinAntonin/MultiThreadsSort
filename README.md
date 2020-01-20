# MultiThreadsSort
Different sort in Multi Threads and in Mono Thread in C++.

```
Enter a positive integer (in millions): 44

Initialisation: done
Multi-thread initialisation: done

##### Start of Tests in monoThread #####

----------  QuickSort sort  ----------
    there was 0% error
    sort execution time: 5990 ms
    
----------  STL sort  ----------
    there was 0% error
    sort execution time: 6702 ms



##### Start of Tests in multitreads #####
----------  QuickSort  ----------
  For 2 threads: 
    There was 0% error
    Execution time: 3956 ms
    The ratio is 1.51416 theoretical 2

  For 4 threads: 
    There was 0% error
    Multithreaded execution time: 2937
    The ratio is 2.0395 theoretical 4

 ----------  STL sort  ----------
  For 2 threads:  
    There was 0% error
    Multithreaded execution time: 4446 ms
    The ratio is 1.50742 theoretical 2
  For 4 threads: 
    There was 0% error  
    Execution time: 3439 ms
    The ratio is 1.94882 theoretical 4
```
