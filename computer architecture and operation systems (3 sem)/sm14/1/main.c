#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


enum
{
  MAX_PROGRAM_SIZE = 16384
};

extern void
build_c_source (char* valid_program, const char* program_source_buff);


int
main ()
{
  char program_source_buff[MAX_PROGRAM_SIZE];
  char valid_program[MAX_PROGRAM_SIZE];
  fgets(
      program_source_buff,
      MAX_PROGRAM_SIZE,
      stdin
  );
  const char *source_fname = "exec_gcc_source.c";
  const char *source_bin = "exec_gcc_source.out";
  int source_fd = open(
      source_fname,
      O_RDWR | O_CREAT,
      (mode_t) 0600
  );
  char* slash_ptr = strchr(program_source_buff, '\n');
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
    build_c_source(valid_program, program_source_buff);
    dprintf(source_fd, "%s" ,valid_program);
    close(source_fd);
    pid_t pid = fork();
    if (pid == 0)
    {
      execlp(
          "gcc",
          "gcc",
          source_fname,
          "-o",
          source_bin,
          NULL
      );
      perror("exec");
      exit(1);
    }
    else
    {
      int status;
      waitpid(pid, &status, 0);
      remove(source_fname);
      char to_run[20] = "";
      snprintf(
          to_run,
          strlen(source_bin) + 3,
          "./%s",
          source_bin
      );
      execlp(to_run, to_run, NULL);
      perror("exec");
      exit(1);
    }
  }
  return 0;
}

extern void
build_c_source (char* valid_program, const char* program_source_buff)
{
  const char *
      SOURCE_CODE_FMT = "#include <math.h>\n"
                        "#include <stdio.h>\n"
                        "#include <stdlib.h>\n"
                        "#include <string.h>\n"
                        "int main(){\n"
                        "\tint expr=(int)(%s);\n"
                        "\tprintf(\"%%d\", expr);\n"
                        "\treturn 0;\n"
                        "}";
  snprintf(
      valid_program,
      MAX_PROGRAM_SIZE,
      SOURCE_CODE_FMT,
      program_source_buff
  );
}
