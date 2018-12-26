#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



enum
{
  MAX_PROGRAM_SIZE = 8192
};

extern void
exec_python (const char* expression);

int 
main()
{
  char program_source_buff[MAX_PROGRAM_SIZE];
  char valid_program[MAX_PROGRAM_SIZE];
  fgets(
    program_source_buff,
    MAX_PROGRAM_SIZE,
    stdin    
  );
  char* slash_ptr = strchr(program_source_buff, '\0');
  if (slash_ptr != NULL)
  {
    *slash_ptr = '\0';
  }
  size_t expression_length = strnlen(
    program_source_buff,
    MAX_PROGRAM_SIZE    
  );
  if (expression_length > 0)
  {
    snprintf(
      valid_program,
      MAX_PROGRAM_SIZE,
      "print(%s)",
      program_source_buff      
    );
    exec_python(valid_program);
  }
  return 0;
}

extern void
exec_python (const char* expression)
{
  execlp("python3", "python3", "-c", expression, NULL);
  perror("exec");
  exit(1);
}
