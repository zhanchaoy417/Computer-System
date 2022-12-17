//
// tsh - A tiny shell program with job control
//
// <Put your name and login ID here>
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

using namespace std;

#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
//

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

//
// main - The shell's main routine
//
int main(int argc, char *argv[])
{
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF)
  {
    switch (c)
    {
    case 'h': // print help message
      usage();
      break;
    case 'v': // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':          // don't print a prompt
      emit_prompt = 0; // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT, sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler); // ctrl-z
  Signal(SIGCHLD, sigchld_handler); // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler);

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
  for (;;)
  {
    //
    // Read command line
    //
    if (emit_prompt)
    {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
    {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin))
    {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  }

  exit(0); //control never reaches here
}

/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
//
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//

int running = 0;

void eval(char *cmdline)
{
  /* Parse command line */
  //
  // The 'argv' vector is filled in by the parseline
  // routine below. It provides the arguments needed
  // for the execve() routine, which you'll need to
  // use below to launch a process.
  //
  char *argv[MAXARGS];

  //
  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
  //
  int bg = parseline(cmdline, argv);
  if (argv[0] == NULL)
    return; /* ignore empty lines */

  pid_t pid;
  sigset_t mask_all, prev_all;


 sigemptyset(&mask_all);
  sigaddset(&mask_all, SIGCHLD);
 sigaddset(&mask_all, SIGINT);//c
 sigaddset(&mask_all,  SIGSTOP);//c
  //sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    if(!builtin_cmd(argv)){
          sigprocmask(SIG_BLOCK, &mask_all, NULL);
  if ((pid = fork()) == 0)
  {
    // I am the child process
        sigprocmask(SIG_UNBLOCK, &mask_all,NULL);
      setpgid(0,0);
    //   sigprocmask(SIG_SETMASK, &prev_all, NULL);
  //  fprintf(stderr, "I am child with pid %d\n", getpgid(0));
    if (execve(argv[0], argv, environ) < 0)
    {
        printf("%s: Command not found\n",argv[0]);
      //fprintf(stderr, "execve failed\n");
    }
  exit(0);
  }
    //parent process
   else {
        if(bg) 
        { 
            addjob(jobs, pid, BG, cmdline);
            printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);
        }
        else
        {
            addjob(jobs, pid, FG, cmdline);
            sigprocmask(SIG_UNBLOCK, &mask_all, NULL);
         waitfg(pid);
        }
      
       
    }
}//built_cmd
  /*addjob(jobs, pid, FG, cmdline);
  fprintf(stderr, "Parent forks child %d\n", pid);
  running = 1;
  sigprocmask(SIG_SETMASK, &prev_all, NULL);

#if 0
  while ( running ) {
    sleep(1);
    fprintf(stderr,"Waiting for child to die...\n");
  }
#endif
*/

  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv)
{
  string cmd(argv[0]);
 if(strcmp(argv[0],"quit")==0)//!strcmp(argv[0],"quit")
 {
     exit(0);
 }
  else  if( strcmp(argv[0],"bg")==0 ||strcmp(argv[0],"fg")==0 )
 {
    do_bgfg(argv);
      return 1;
 }
 
  else  if(strcmp(argv[0],"jobs")==0)
 {
      listjobs(jobs);
      return 1;
 }
 
  return 0; /* not a builtin command */
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv)
{
  struct job_t *jobp = NULL;

  /* Ignore command if no argument */
  if (argv[1] == NULL)
  {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }

  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0]))
  {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid)))
    {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%')
  {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid)))
    {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }
  else
  {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
  string cmd(argv[0]);
    kill(-(jobp->pid),SIGCONT);
    //if call bg
    if(strcmp(argv[0],"bg")==0)
    {
        jobp->state=BG;
        printf("[%d] (%d) %s",jobp->jid,jobp->pid,jobp->cmdline);
    }
    //else is fg case
    else
    {
        jobp->state=FG;
        waitfg(jobp->pid);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{   
  while(pid==fgpid(jobs))
    {
        sleep(1);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//

/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.
//
void sigchld_handler(int sig)
{
  /*fprintf(stderr, "In SIGCHLD handler\n");
  for (;;)
  {
    // Figure out who died / stopped using WAIT()
    int pid = wait(NULL);
    if (pid < 0)
    {
      fprintf(stderr, "No more children to reap\n");
      break;
    }
    else
    {
      fprintf(stderr, "Process %d died\n", pid);
      deletejob(jobs, pid);
    }
  }
  // Go through jobs data structure and indicate the PID has died....
  running = 0;
  fprintf(stderr, "Running set to %d\n", running);
  return;*/
   
   //sigset_t mask_all,prev_mask;
    int status;
    pid_t pid;
    while((pid=waitpid(-1,&status, WNOHANG|WUNTRACED))>0)
    {
         //sigprocmask(SIG_SETMASK,&mask_all,&prev_mask);
        
        if(WIFEXITED(status))
        {
            deletejob(jobs,pid);
        }
       if(WIFSIGNALED(status))
        {
          printf("Job [%d] (%d) terminated by signal %d\n",pid2jid(pid),pid,WTERMSIG(status));
          deletejob(jobs,pid);
        }
         if(WIFSTOPPED(status))
        {
             job_t *j=getjobpid(jobs,pid);
            printf("Job [%d] (%d) stopped by signal %d\n",pid2jid(pid),pid,WSTOPSIG(status));
            j->state=ST;
        }
         // sigprocmask(SIG_SETMASK,&prev_mask,NULL);
    }
 
 return;
  
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.
//
//int how_many = 0;
void sigint_handler(int sig)
{
  /*fprintf(stderr, "You got interrupted!\n");
  how_many++;
  if (how_many > 5)
  {
    fprintf(stderr, "Well, if you insist....\n");
    exit(100);
  }*/
    pid_t pid=fgpid(jobs);
    if(pid!=0)
    {
        kill(-pid,sig);
    }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.
//
void sigtstp_handler(int sig)
{
 // fprintf(stderr, "You got put to sleep!\n");
   
      pid_t pid=fgpid(jobs);
    if(pid!=0)
    {
        kill(-pid,sig);
    }
  return;
}

/*********************
 * End signal handlers
 *********************/
