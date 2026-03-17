#include "myshell.h"
#include <errno.h>
#include <stdbool.h>

/*Maximum Limitation*/
#define MAX_ARGS (1 << 10)
#define MAX_LINE (1 << 13) // Max. limitation of command lines
#define MAX_PIPE 10		   // Max. limitation of pipe commands
#define MAX_SAVE 200	   // # of characters saved into struct
#define MAX_JOBS 100

/*Job Status*/
#define SUSPEND 0
#define RUNNING 1

/*Struct Job*/
typedef struct _job
{
	pid_t pid;
	int state; // 0: SUSPEND, 1: RUNNING
	bool bg;   // true: bg, false: fg
	char command[MAX_SAVE];
} JOB;

/*Global Variables*/
JOB jobs[MAX_JOBS];
int cmdcnt = 1;
char entire_line[MAX_SAVE];
pid_t last_pid, pgid_bg, pgid_fg;
bool bgflag;
volatile sig_atomic_t explicit_wait;
volatile sig_atomic_t fg_wait;
volatile int now;

/*Function Prototypes*/
void init_job(JOB *j);
void init_jobs();
void eval(char *cmdline, int cmdidx, int (*fd)[2]);
bool parse_line(char *buf, char *argv[MAX_ARGS], int cmdidx);
void parse_pipe(char *cmdline, char **cmdpipe);
pid_t parse_job(char *str);
bool builtin_command(char **argv);
void sigint_handler(int sig);
void sigint_handler_parent(int sig);
void sigtstp_handler(int sig);
void sigchld_handler(int sig);
void bg_wake(char *str);
void fg_run(char *str);
void update(pid_t pid, int num);
void delete(pid_t pid);
void addjob(pid_t pid);
void show_jobs();
void kill_job(char *str);
int find_jobnum(pid_t pid);

/*Main Function*/
int main(int argc, char *argv[])
{
	// Variables
	char cmdline[MAX_LINE];
	char *cmdpipe[MAX_PIPE];
	int fd[MAX_PIPE][2];

	bgflag = false;
	explicit_wait = 0;
	init_jobs();
	while (true)
	{
		printf("CSE4100-SP-P2> "); // Shell prompt

		// Reading command lines from stdin
		if (!fgets(cmdline, MAX_LINE, stdin))
		{
			unix_error("Error in reading command lines.");
		}
		if (feof(stdin))
		{
			return EXIT_SUCCESS;
		}

		// If pipe commands exist, parse them into seperated commands.
		parse_pipe(cmdline, cmdpipe);

		// Maximum limitation of Pipe
		if (cmdcnt > MAX_PIPE + 1)
		{
			unix_error("Pipe commands exceed the limitation.");
		}
		for (int i = 0; i < cmdcnt - 1; i++)
		{
			if (pipe(fd[i]) < 0)
			{
				unix_error("PIPE Error");
				exit(EXIT_FAILURE);
			}
		}

		// SIGCHLD Handler
		if (signal(SIGCHLD, sigchld_handler) == SIG_ERR)
		{
			unix_error("SIGCHLD Handler Error");
		}
		for (int i = 0; i < cmdcnt; i++)
		{
			eval(cmdpipe[i], i + 1, fd);
		}
	}
}

// Initialise one job
void init_job(JOB *j)
{
	j->pid = 0;
	j->state = 0;
	j->bg = false;
	memset(j->command, '\0', sizeof(j->command));
}

// Initialise entire jobs
void init_jobs()
{
	for (int i = 0; i < MAX_JOBS; i++)
	{
		init_job(&jobs[i]);
	}
}

// Parse command lines and Execute one by one
void eval(char *cmdline, int cmdidx, int (*fd)[2])
{
	bool bg;			  // false: fg, true: bg
	char buf[MAX_LINE];	  // Saves commands here
	char *argv[MAX_ARGS]; // Split into arguments

	pid_t pid;
	sigset_t mask, mask_wait, mask_all, mask_prev;

	strcpy(buf, cmdline);
	bg = parse_line(buf, argv, cmdidx);
	if (argv[0] == NULL)
	{
		return;
	} // Empty line
	if (!builtin_command(argv))
	{
		if (signal(SIGINT, sigint_handler_parent) == SIG_ERR)
		{
			unix_error("Signal Install Error");
		}
		if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
		{
			unix_error("Signal Install Error");
		}
		Sigemptyset(&mask);
		Sigfillset(&mask_all);
		Sigaddset(&mask, SIGCHLD);
		Sigfillset(&mask_wait);
		Sigdelset(&mask_wait, SIGCHLD);
		if (cmdidx == cmdcnt)
		{
			Sigprocmask(SIG_BLOCK, &mask, &mask_prev);
		}
		// Child Process
		if ((pid = Fork()) == 0)
		{
			if (signal(SIGINT, sigint_handler) == SIG_ERR)
			{
				unix_error("Signal Install Error");
			}
			if (signal(SIGCHLD, SIG_DFL) == SIG_ERR)
			{
				unix_error("Child handler revert error");
			}
			if (cmdidx == cmdcnt)
			{
				Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
			}

			// If there are pipe codes, manipulate child fd with dup2
			if (cmdcnt > 1)
			{
				if (cmdidx == 1)
				{
					Dup2(fd[0][1], STDOUT_FILENO);
				}
				else if (cmdidx < cmdcnt)
				{
					Dup2(fd[cmdidx - 2][0], STDIN_FILENO);
					Dup2(fd[cmdidx - 1][1], STDOUT_FILENO);
				}
				else
				{
					Dup2(fd[cmdidx - 2][0], STDIN_FILENO);
				}
			}
			// Close all file descriptors
			for (int i = 0; i < cmdcnt - 1; i++)
			{
				close(fd[i][0]);
				close(fd[i][1]);
			}
			// execvp to run command
			if (execvp(argv[0], argv) < 0)
			{
				delete (getpid());
				unix_error("Command error");
			}
		}
		// Parent Process
		// Single command
		if (cmdcnt == 1)
		{
			Sigprocmask(SIG_BLOCK, &mask_all, NULL);
			addjob(pid);
			if (!bg)
			{
				last_pid = pid;
				explicit_wait = 1;
				sigsuspend(&mask_wait); // Waiting last_pid
				Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
			}
			else
			{
				Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
				printf("%d %s", pid, cmdline);
			}

			if (signal(SIGINT, sigint_handler) == SIG_ERR)
			{
				unix_error("Signal Install Error");
			}
		}
		// Pipe commands
		else
		{
			/* on the function call of the last command */
			if (cmdidx == cmdcnt)
			{
				// Parent file descriptors should be closed.
				for (int i = 0; i < cmdcnt - 1; i++)
				{
					close(fd[i][0]);
					close(fd[i][1]);
				}

				Sigprocmask(SIG_BLOCK, &mask_all, NULL);
				addjob(pid);
				// Foreground
				if (!bg)
				{
					last_pid = pid;
					explicit_wait = 1;
					sigsuspend(&mask_wait); // Waiting last_pid
					Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
				}
				else
				{
					Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
					printf("%d %s", pid, cmdline);
				}
				if (signal(SIGINT, sigint_handler) == SIG_ERR)
				{
					unix_error("Signal Install Error");
				}
			}
		}
	}
}

bool parse_line(char *buf, char *argv[MAX_ARGS], int cmdidx)
{
	bool bg;
	int argc;
	char *delim;
	char *cut;
	int len;

	/* 1. Strip whitespaces and Re-designate the string as pointers, buf and cut */

	len = strlen(buf);
	buf[len - 1] = ' '; // Substitute last newline into whitespace
	len = strlen(buf);
	while (*buf && (*buf == ' '))
	{
		buf++;
	}					 // Ignore leading spaces
	cut = buf + len - 1; // Final character pointer
	while (*cut && (*cut == ' ' || *cut == '\n'))
	{
		cut--;
	} // Ignore following spices

	/* 2. Check if this command has ampersand or not */
	if (*cut == '&')
	{
		bg = true;	// Background flag
		*cut = ' '; // Substitute ampersand into whitespace
		if (buf == cut)
		{
			argc = 0;
			argv[0] = NULL;
			return bg;
		}
		bgflag = bg;
	}
	else
	{
		bg = false;
		bgflag = false;
	}

	/* 3. Split commands from buf into argv */
	argc = 0;
	while ((delim = strchr(buf, ' ')))
	{
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))
		{
			buf++;
		}

		// Ignore quotation marks
		while ((*buf == '\"') || (*buf == '\''))
		{
			char tmp = *buf;
			argv[argc++] = ++buf;
			delim = strchr(buf, tmp);
			*delim = '\0';
			buf = delim + 2;
			while (*buf && (*buf == ' '))
			{
				buf++;
			} // Ignore whitespaces
		}
	}
	argv[argc] = NULL; // Last fragment

	// Concatenate all argv into entire_line
	for (int i = 0; i < argc; i++)
	{
		strcat(entire_line, argv[i]);
		strcat(entire_line, " ");
	}
	if (cmdcnt > 1 && cmdcnt > cmdidx)
	{
		strcat(entire_line, "| ");
	}
	if (argc == 0)
	{
		return EXIT_FAILURE;
	}
	return bg;
}

bool builtin_command(char **argv)
{
	if (!strcmp(argv[0], "exit"))
	{
		exit(EXIT_SUCCESS);
	}
	if (!strcmp(argv[0], "&"))
	{
		return true;
	}
	if (!strcmp(argv[0], "cd"))
	{
		if (chdir(argv[1]) < 0)
		{
			fprintf(stderr, "cd: %s: %s\n", argv[1], strerror(errno));
		}
		return true;
	}
	if (!strcmp(argv[0], "jobs"))
	{
		show_jobs();
		return true;
	}
	if (!strcmp(argv[0], "kill"))
	{
		kill_job(argv[1]);
		return true;
	}
	if (!strcmp(argv[0], "bg"))
	{
		bg_wake(argv[1]);
		return true;
	}
	if (!strcmp(argv[0], "fg"))
	{
		if (signal(SIGINT, sigint_handler_parent) == SIG_ERR)
		{
			unix_error("Signal Install Error");
		}
		fg_run(argv[1]);
		if (signal(SIGINT, sigint_handler) == SIG_ERR)
		{
			unix_error("Signal Install Error");
		}
		return true;
	}
	return false;
}

void parse_pipe(char *cmdline, char **cmdpipe)
{
	cmdcnt = 1;
	cmdpipe[0] = cmdline;

	if (!strchr(cmdline, '|'))
	{
		return;
	}

	char *buf = cmdline;
	while (buf = strchr(buf, '|'))
	{
		*buf = '\0';
		buf++;
		while (*buf && (*buf == ' '))
		{
			buf++;
		}
		cmdpipe[cmdcnt++] = buf;
	}
}

// SIGCHLD handler
void sigchld_handler(int sig)
{
	int olderrno = errno;
	int status;
	pid_t pid;
	sigset_t mask_all, mask_prev;

	Sigfillset(&mask_all);

	// Foreground Explicit wait
	if (explicit_wait > 0)
	{
		if ((pid = waitpid(last_pid, &status, WUNTRACED)) < 0)
		{
			Sio_error("waitpid error in foreground wait\n");
		}
		Sigprocmask(SIG_BLOCK, &mask_all, &mask_prev);
		// Normal termination
		if (WIFEXITED(status))
		{
			delete (pid);
		}
		// If not terminated,
		else
		{
			if (WIFSTOPPED(status))
			{
				Sio_puts("\n");
				update(pid, SUSPEND);
			}
			else if (WIFSIGNALED(status))
			{
				delete (pid);
			}
		}
		explicit_wait = 0;
		Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
	}

	while ((pid = waitpid(-1, &status, WNOHANG | WCONTINUED)) > 0)
	{
		Sigprocmask(SIG_BLOCK, &mask_all, &mask_prev);
		if (WIFEXITED(status))
		{
			delete (pid);
		}
		else if (WIFSIGNALED(status))
		{
			delete (pid);
		}
		else if (WIFCONTINUED(status))
		{
			// Waken from Stopped
			int jobnum;
			update(pid, RUNNING);
			jobnum = find_jobnum(pid);
			Sio_puts("[");
			Sio_putl(jobnum);
			Sio_puts("] running ");
			Sio_puts(jobs[jobnum].command);
			Sio_puts("\n");
		}
		Sigprocmask(SIG_SETMASK, &mask_prev, NULL);
	}
	errno = olderrno;
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
	if (sigprocmask(how, set, oldset) < 0)
	{
		unix_error("Sigprocmask error");
	}
}
void Sigemptyset(sigset_t *set)
{
	if (sigemptyset(set) < 0)
	{
		unix_error("Sigemptyset error");
	}
}
void Sigaddset(sigset_t *set, int signum)
{
	if (sigaddset(set, signum) < 0)
	{
		unix_error("Sigaddset error");
	}
}
void Sigfillset(sigset_t *set)
{
	if (sigfillset(set) < 0)
	{
		unix_error("Sigfillset error");
	}
}
void Sigdelset(sigset_t *set, int signum)
{
	if (sigdelset(set, signum) < 0)
	{
		unix_error("Sigdelset error");
	}
}

// Nothing to do
void sigtstp_handler(int sig) {}

// Update Job Status
void update(pid_t pid, int num)
{
	for (int i = 0; i < MAX_JOBS; i++)
	{
		if (jobs[i].pid == pid)
		{
			if (num == RUNNING)
				jobs[i].state = RUNNING;
			if (num == SUSPEND)
				jobs[i].state = SUSPEND;
		}
	}
}

// Delete Jobs with PID
void delete(pid_t pid)
{
	for (int i = 0; i < MAX_JOBS; i++)
	{
		if (jobs[i].pid == pid)
		{
			jobs[i].pid = 0;
			jobs[i].command[0] = '\0';
		}
	}
}

// Add jobs with PID
void addjob(pid_t pid)
{
	if (now >= MAX_JOBS)
	{
		now = 1;
		while (jobs[now].pid != 0)
		{
			now++;
		}
		if (now >= MAX_JOBS)
		{
			unix_error("JOB list are fulled!\n");
		}
	}

	if (jobs[now].pid == 0)
	{
		jobs[now].pid = pid;
		jobs[now].state = RUNNING;
		if (bgflag)
			jobs[now].bg = 1;
		else
			jobs[now].bg = 0;
		strcpy(jobs[now++].command, entire_line);
	}
}

// list all jobs that are running or suspended
void show_jobs()
{
	for (int i = 1; i < MAX_JOBS; i++)
	{
		if (jobs[i].pid != 0)
		{
			if (jobs[i].state == RUNNING)
			{
				printf("[%d] running %s\n", i, jobs[i].command);
			}
			else if (jobs[i].state == SUSPEND)
			{
				printf("[%d] suspended %s\n", i, jobs[i].command);
			}
		}
	}
}
// Kill the designated job
void kill_job(char *str)
{
	pid_t pid;
	if ((pid = parse_job(str)) < 0)
	{
		return;
	}
	kill(pid, SIGKILL);
}

// Wake the background running job
void bg_wake(char *str)
{
	pid_t pid;
	if ((pid = parse_job(str)) < 0)
	{
		return;
	}
	kill(pid, SIGCONT);
	sleep(1);
}

// Make it Foreground Job
void fg_run(char *str)
{
	pid_t pid;
	int jobnum;

	sigset_t mask_wait;
	Sigfillset(&mask_wait);
	Sigdelset(&mask_wait, SIGCHLD);

	if ((pid = parse_job(str)) < 0)
		return;
	jobnum = find_jobnum(pid);
	if (jobs[jobnum].state == SUSPEND)
	{
		kill(pid, SIGCONT);
		sleep(1);
		last_pid = pid;
		explicit_wait = 1;		// flag will be turned off after handle (handler do deletejob and reaping)
		sigsuspend(&mask_wait); // wait for child(last_pid)
	}
	else
	{
		Sio_puts("[");
		Sio_putl(jobnum);
		Sio_puts("] running ");
		Sio_puts(jobs[jobnum].command);
		Sio_puts("\n");
		last_pid = pid;
		explicit_wait = 1;
		sigsuspend(&mask_wait);
	}
}

// find jobnum of the pid and return jobnum
int find_jobnum(pid_t pid)
{
	int jobnum;
	for (int i = 1; i < MAX_JOBS; i++)
	{
		if (jobs[i].pid == pid)
			return i;
	}
}

pid_t parse_job(char *str)
{
	int jobnum;
	// parsing num and error catching
	if (str[0] != '%')
	{
		fprintf(stderr, "jobstring arg should be %% with a digit (such as %%1)\n");
		return -1;
	}
	else
	{
		if ((jobnum = atoi(str + 1)) == 0)
		{
			// if it starts with alphanumeric character ...
			fprintf(stderr, "jobstring arg should be %% with a digit (such as %%1)\n");
			return -1;
		}
	}

	// check if jobnum is valid on the job list
	if (jobnum >= MAX_JOBS)
	{
		fprintf(stderr, "Max job num is %d\n", (MAX_JOBS - 1));
		return -1;
	}
	if (jobs[jobnum].pid == 0)
	{
		fprintf(stderr, "No such job\n");
		return -1;
	}

	return jobs[jobnum].pid;
}

/* Wrappers */
void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

int Dup2(int fd1, int fd2)
{
	int rc;
	if ((rc = dup2(fd1, fd2)) < 0)
		unix_error("dup2 error");
	return rc;
}

pid_t Fork()
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		unix_error("Fork error");
	}
	return pid;
}

int Execve(char *cmd, char **argv)
{
	if (execve(cmd, argv, environ) < 0)
	{
		printf("%s: Command not found.\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
}

/* Basic Handlers */
void sigint_handler(int sig)
{
	Sio_puts("\n");
	_exit(0);
}
void sigint_handler_parent(int sig) { Sio_puts("\n"); }

/* SIO */
static size_t sio_strlen(char s[])
{
	int i = 0;
	while (s[i] != '\0')
	{
		++i;
	}
	return i;
}
ssize_t sio_puts(char s[]) { return write(STDOUT_FILENO, s, sio_strlen(s)); }
static void sio_reverse(char s[])
{
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
static void sio_ltoa(long v, char s[], int b)
{
	int c, i = 0;
	do
	{
		s[i++] = ((c = (v % b)) < 10) ? c + '0' : c - 10 + 'a';
	} while ((v /= b) > 0);
	s[i] = '\0';
	sio_reverse(s);
}
ssize_t sio_putl(long v)
{
	char s[128];
	sio_ltoa(v, s, 10);
	return sio_puts(s);
}
void sio_error(char s[])
{
	sio_puts(s);
	_exit(1);
}
ssize_t Sio_puts(char s[])
{
	ssize_t n;
	if ((n = sio_puts(s)) < 0)
	{
		sio_error("Sio_puts error");
	}
	return n;
}
ssize_t Sio_putl(long v)
{
	ssize_t n;
	if ((n = sio_putl(v)) < 0)
	{
		sio_error("Sio_putl error");
	}
	return n;
}
void Sio_error(char s[])
{
	Sio_puts(s);
	_exit(1);
}