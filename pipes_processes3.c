#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid;
  int pid2;

  int i;
  for(i = 0; i < argc; i++){
    printf("argv[%d]: %s\n", i, argv[i]);
  }
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL, NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  //Integer will be in the Second Position of Argv
  grep_args[1] = argv[1];

  pipe(pipefd);

  pid = fork();

//Child 1
    if (pid == 0)
      {
        //Child 2
        pipe(pipefd2);
        pid2 = fork();

        if(pid2 == 0)
        {
          //printf("Me %d", 3);
          dup2(pipefd2[1],1);
          close(pipefd2[0]);
          close(pipefd[0]);
          execvp("cat", cat_args);
        }
        // child gets here and handles "grep Villanova"

        // replace standard input with input part of pipe
        else if(pid < 0)
        {
          printf("Error Thrown");
        }
        else{
          printf("Me %d", 2);
          dup2(pipefd[0], 0);
          dup2(pipefd2[1], 1);
          // close unused hald of pipe

          close(pipefd2[1]);
          close(pipefd[0]);

          // execute grep
          execvp("grep", grep_args);
        }

      }
    else if(pid < 0)
    {
      printf("Error Thrown");
    }
    else
      {
        //Parent 
        //printf("Me %d", 1);
        // parent gets here and handles "cat scores"
        // replace standard output with output part of pipe

        dup2(pipefd[0], 0);

        // close unused unput half of pipe
        close(pipefd[1]);
        close(pipefd[3]);
        // execute cat
        execvp("sort", sort_args);
      }
}