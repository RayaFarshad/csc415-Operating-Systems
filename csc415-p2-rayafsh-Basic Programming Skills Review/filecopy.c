#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//You may also add more include directives as well.
// THIS VALUE CANNOT BE CHANGED.
// You should use this value when creating your buffer.
// And also to limit the amount of bytes each read CAN do.
#define BUFF_MAX 13
//ssize_t read(int fd, void *buf, size_t count);
// DO NOT CHANGE ABOVE
//int open(const char *pathname, int flags, mode_t mode);
int
main(int argc, char const *argv[])
{
char name[] = "Raya";
int fd_input, fd_output;
ssize_t read_byte, write_byte;
char input_namefile[50];
char output_namefile[50];
char *buffer[BUFF_MAX];
ssize_t count = 0;
printf("Welcome to the filecopy program by %s\n", name);
printf("Enter the name of the file to copy from:\n");
scanf("%s",input_namefile);
fd_input = open(input_namefile, O_RDONLY, 0777);
if (fd_input<0){
  printf("Error opening the input file");
  exit(1);
}
printf("Enter the name of the file to copy to:\n");
scanf("%s",output_namefile);
fd_output = open(output_namefile, O_CREAT | O_EXCL | O_RDWR, 0777);
if (fd_output<0){
  printf("Error opening the output file");
  exit(1);
}
while ((read_byte = read(fd_input, buffer, BUFF_MAX))>0) {
  count = read_byte + count;
  write_byte = write(fd_output, buffer, read_byte);
  if(write_byte != read_byte){
    printf("Error");
    exit(1);
  }
  }
printf("File Copy Successful, %zd bytes copied\n", count);
close(fd_input);
close(fd_output);
   return 0;
}
