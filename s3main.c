#include "s3.h"

int main(int argc, char *argv[]){

    ///Stores the command line input
    char line[MAX_LINE];

    ///Stores pointers to command arguments.
    ///The first element of the array is the command name.
    char *args[MAX_ARGS];
    char lwd[MAX_DIR_LEN]; 
    init_lwd(lwd);///Implement this function: initializes lwd with the cwd (using getcwd)


    ///Stores the number of arguments
    int argsc;

    while (1) {

        read_command_line(line);
        parse_command(line, args, &argsc);
        if(is_cd(args, argsc)){
            run_cd(args, argsc, lwd); ///Implement this function
        }
        else if(command_with_redirection(args, argsc)){///Command with redirection
           launch_program_with_redirection(args, argsc);
           reap(); 
       }
       else ///Basic command
       {
           launch_program(args, argsc);
           reap();
       }
    }

    return 0;
    
}
