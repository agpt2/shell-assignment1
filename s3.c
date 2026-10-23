#include "s3.h"

///Simple for now, but will be expanded in a following section
void construct_shell_prompt(char shell_prompt[])
{
    strcpy(shell_prompt, "[s3]$ ");
}

///Prints a shell prompt and reads input from the user
void read_command_line(char line[])
{
    char shell_prompt[MAX_PROMPT_LEN];
    construct_shell_prompt(shell_prompt);
    printf("%s", shell_prompt);

    ///See man page of fgets(...)
    if (fgets(line, MAX_LINE, stdin) == NULL)
    {
        perror("fgets failed");
        exit(1);
    }
    ///Remove newline (enter)
    line[strlen(line) - 1] = '\0';
}

void parse_command(char line[], char *args[], int *argsc)
{
    ///Implements simple tokenization (space delimited)
    ///Note: strtok puts '\0' (null) characters within the existing storage, 
    ///to split it into logical cstrings.
    ///There is no dynamic allocation.

    ///See the man page of strtok(...)
    char *token = strtok(line, " ");
    *argsc = 0;
    while (token != NULL && *argsc < MAX_ARGS - 1)
    {
        args[(*argsc)++] = token;
        token = strtok(NULL, " ");
    }
    
    args[*argsc] = NULL; ///args must be null terminated
}

///Launch related functions
void child(char *args[], int argsc)
{
    ///Implement this function:
    execvp(args[ARG_PROGNAME], args);
    ///Use execvp to load the binary 
    ///of the command specified in args[ARG_PROGNAME].
    ///For reference, see the code in lecture 3.
    perror("execvp failed");
    exit(1);

}

void launch_program(char *args[], int argsc)
{
    ///Implement this function
    if(argsc == 0){
        return;
    }
    if( strcmp(args[ARG_PROGNAME], "exit") == 0){
        exit(0);
    }
    int rc = fork();
    ///fork() a child process.
    ///In the child part of the code,
    ///call child(args, argv)
    ///For reference, see the code in lecture 2.
    if(rc < 0){
        exit(1);
    }
    else if(rc == 0){//child
        child(args, argsc);
    }

    ///Handle the 'exit' command;
    ///so that the shell, not the child process,
    ///exits.
    return;
}

int command_with_redirection(char *args[], int argsc){
    for(int i = 0; i < argsc; i++){
        if(strcmp(args[i], "<") == 0 ||
           strcmp(args[i], ">") == 0 ||
           strcmp(args[i], ">>") == 0){
            return 1;
        }
    }
    return 0;
}

void launch_program_with_redirection(char *args[], int argsc){
    int rc = fork();

    if(rc < 0){
        perror("fork failed");
        return;
    }
    else if(rc == 0){ // child
        for(int i = 0; i < argsc; i++){
            if(strcmp(args[i], "<") == 0){
                child_with_input_redirected(args, i);
            }
            else if(strcmp(args[i], ">") == 0 ||
                    strcmp(args[i], ">>") == 0){
                child_with_output_redirected(args, i);
            }
        }
        exit(EXIT_FAILURE);
    }

    // The parent returns to main, where reap() waits for the child.
}

void child_with_output_redirected(char *args[], int i)
{
    if (i == 0 || args[i + 1] == NULL) {
        fprintf(stderr, "Missing command or output filename\n");
        exit(EXIT_FAILURE);
    }

    char *filename = args[i + 1];

    int flags = O_WRONLY | O_CREAT;

    if (strcmp(args[i], ">>") == 0) {
        flags |= O_APPEND;
    }
    else {
        flags |= O_TRUNC;
    }

    int fd = open(filename, flags, 0666);
    //0666?

    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (fd != STDOUT_FILENO) {
        close(fd);
    }

    /* Stop the command's arguments before the operator. */
    args[i] = NULL;

    /* Reuse your existing function to run the command. */
    child(args, i);
}


void child_with_input_redirected(char *args[], int i)
{
    if (i == 0 || args[i + 1] == NULL) {
        fprintf(stderr, "Missing command or input filename\n");
        exit(EXIT_FAILURE);
    }

    char *filename = args[i + 1];

    /* Open the existing file for reading, as in Lecture 3. */
    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    /* Make standard input refer to the file. */
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2 failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (fd != STDIN_FILENO) {
        close(fd);
    }

    /* Exclude "<" and the filename from the command arguments. */
    args[i] = NULL;

    child(args, i);
}