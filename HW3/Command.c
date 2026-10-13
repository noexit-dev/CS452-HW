#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "Command.h"
#include "error.h"

typedef struct {
  char *file;
  char **argv;
  char *in;
  char *out;
} *CommandRep;

#define BIARGS CommandRep r, int *eof, Jobs jobs
#define BINAME(name) bi_##name
#define BIDEFN(name) static void BINAME(name) (BIARGS)
#define BIENTRY(name) {#name,BINAME(name)}

static char *owd=0;
static char *cwd=0;

static void builtin_args(CommandRep r, int n) {
  char **argv=r->argv;
  for (n++; *argv++; n--);
  if (n)
    ERROR("wrong number of arguments to builtin command"); // warn
}

BIDEFN(exit) {
  builtin_args(r,0);
  *eof=1;
}

BIDEFN(pwd) {
  builtin_args(r,0);
  if (!cwd)
    cwd=getcwd(0,0);
  printf("%s\n",cwd);
}

BIDEFN(cd) {
  builtin_args(r,1);
  if (strcmp(r->argv[1],"-")==0) {
    char *twd=cwd;
    cwd=owd;
    owd=twd;
  } else {
    if (owd) free(owd);
    owd=cwd;
    cwd=strdup(r->argv[1]);
  }
  if (cwd && chdir(cwd))
    ERROR("chdir() failed"); // warn
}

  //where remaining builtin commands go

static int builtin(BIARGS) {
  typedef struct {
    char *s;
    void (*f)(BIARGS);
  } Builtin;
  static const Builtin builtins[]={ //add list of builtin cmds when implementing
    BIENTRY(exit),
    BIENTRY(pwd),
    BIENTRY(cd),
    {0,0}
  };
  int i;
  for (i=0; builtins[i].s; i++)
    if (!strcmp(r->file,builtins[i].s)) {
      builtins[i].f(r,eof,jobs);
      return 1;
    }
  return 0;
}

static char **getargs(T_words words) {
  int n=0;
  T_words p=words;
  while (p) {
    p=p->words;
    n++;
  }
  char **argv=(char **)malloc(sizeof(char *)*(n+1));
  if (!argv)
    ERROR("malloc() failed");
  p=words;
  int i=0;
  while (p) {
    argv[i++]=strdup(p->word->s);
    p=p->words;
  }
  argv[i]=0;
  return argv;
}

static int open_input(char *path) {
  int fd=open(path,O_RDONLY);
  if (fd==-1)
    ERROR("open() failed");
  return fd;
}

static int open_output(char *path) {
  int fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,0666);
  if (fd==-1)
    ERROR("open() failed");
  return fd;
}

static void apply_redirection(CommandRep r, int *saved_stdin, int *saved_stdout) {
  *saved_stdin=-1;
  *saved_stdout=-1;
  if (r->in) {
    *saved_stdin=dup(STDIN_FILENO);
    if (*saved_stdin==-1)
      ERROR("dup() failed");
    int fd=open_input(r->in);
    if (dup2(fd,STDIN_FILENO)==-1)
      ERROR("dup2() failed");
    close(fd);
  }
  if (r->out) {
    *saved_stdout=dup(STDOUT_FILENO);
    if (*saved_stdout==-1)
      ERROR("dup() failed");
    int fd=open_output(r->out);
    if (dup2(fd,STDOUT_FILENO)==-1)
      ERROR("dup2() failed");
    close(fd);
  }
}

static void restore_redirection(int saved_stdin, int saved_stdout) {
  if (saved_stdin!=-1) {
    if (dup2(saved_stdin,STDIN_FILENO)==-1)
      ERROR("dup2() failed");
    close(saved_stdin);
  }
  if (saved_stdout!=-1) {
    if (dup2(saved_stdout,STDOUT_FILENO)==-1)
      ERROR("dup2() failed");
    close(saved_stdout);
  }
}

extern Command newCommand(T_command command) {
  CommandRep r=(CommandRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->argv=getargs(command->words);
  r->file=r->argv[0];
  r->in=command->in ? strdup(command->in->s) : 0;
  r->out=command->out ? strdup(command->out->s) : 0;
  return r;
}

static void child(CommandRep r, int fg) {
  int eof=0;
  Jobs jobs=newJobs();
  int saved_stdin;
  int saved_stdout;
  apply_redirection(r,&saved_stdin,&saved_stdout);
  if (builtin(r,&eof,jobs))
    _exit(0);
  execvp(r->argv[0],r->argv);
  ERROR("execvp() failed");
  exit(0);
}

extern void execCommand(Command command, Pipeline pipeline, Jobs jobs, int *jobbed, int *eof, int fg) {
  CommandRep r=command;
  int saved_stdin=-1;
  int saved_stdout=-1;
  if (fg) {
    apply_redirection(r,&saved_stdin,&saved_stdout);
    if (builtin(r,eof,jobs)) {
      restore_redirection(saved_stdin,saved_stdout);
      return;
    }
    restore_redirection(saved_stdin,saved_stdout);
  }
  if (!fg && !*jobbed) {
    *jobbed=1;
    addJobs(jobs,pipeline);
  }
  int pid=fork();
  if (pid==-1)
    ERROR("fork() failed");
  if (pid==0)
    child(r,fg);
  if (fg && waitpid(pid,0,0)==-1)
    ERROR("waitpid() failed");
}

extern void freeCommand(Command command) {
  CommandRep r=command;
  char **argv=r->argv;
  while (*argv)
    free(*argv++);
  free(r->argv);
  if (r->in)
    free(r->in);
  if (r->out)
    free(r->out);
  free(r);
}

extern void freestateCommand() {
  if (cwd) free(cwd);
  if (owd) free(owd);
}
