# CSC 415 - Project 2 - File copy

## Student Name : Raya Farshad

## Student ID   : 917940106

## Build Instructions
In order to build the program I use gcc compiler using the following command:
gcc -o filecopy filecopy.c
## Run Instructions
To run the program I use the following command to run the executable file that I built in the last step:
./filecopy
## strace/dtruss output
strace count total time it takes to run the program and each sys calls. Also it give the error for each sys calls. and the total error.

raya@raya-VirtualBox:~/csc415-p2-rayafsh$ strace -c ls
?	     CMakeLists.txt  filecopy.c  Makefile
415_HW2.pdf  filecopy	     LICENSE	 README.MD
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 18.89    0.000157          16        10           open
 18.77    0.000156           8        20           mmap
 13.96    0.000116          10        12           mprotect
  8.90    0.000074          11         7           read
  5.42    0.000045           4        11           fstat
  4.81    0.000040           3        12           close
  4.45    0.000037           5         7         7 access
  4.21    0.000035          18         2           getdents
  3.85    0.000032          16         2         2 statfs
  3.49    0.000029          29         1           munmap
  3.37    0.000028          14         2           rt_sigaction
  2.41    0.000020          10         2           write
  2.17    0.000018           6         3           brk
  1.08    0.000009           9         1           set_tid_address
  0.84    0.000007           7         1           rt_sigprocmask
  0.84    0.000007           7         1           getrlimit
  0.72    0.000006           3         2           ioctl
  0.72    0.000006           6         1           set_robust_list
  0.48    0.000004           4         1           execve
  0.36    0.000003           3         1           futex
  0.24    0.000002           2         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000831                   100         9 total

What your code does:
1-My code basically first prompt the user for the name of the file we want to read from Then check if the file is already exist. if it exist then will open the file.
2-After that ask user for the name of the file we want to write in to if it exist open if not create the Read&Write file with the same name.
3-Next we have a while loop which read the input file and put it into the buffer with the max size of 13. It copy it to the buffer 13 by 13 until the end of the srting.
4-I assigned a variable to count the read bytes to the buffer
5-Then the write function will write from the buffer to the output file.
