#include "systemcalls.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "fcntl.h"
#include <string.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
	
    int res = system(cmd);
    if(!res)
        return true;
    else
        return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool is_absolute_path(const char *path) {
    return path[0] == '/';  // Absolute path starts with '/'
}

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);

    // Create an array of arguments with one extra slot for the NULL terminator
    char *command[count + 1];
    int i;

    // Collect the command arguments from the va_list
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL; // Null terminate the argument list
    
        // Check if the command is an absolute path
    if (!is_absolute_path(command[0])) {
        fprintf(stderr, "The command must be specified with an absolute path: %s\n", command[0]);
        va_end(args);
        return false;
    }

    // TODO: Fork, execv(), and wait as described
    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed
        perror("fork failed");
        va_end(args);
        return false;
    }

    if (pid == 0) {
        // Inside child process
        if (execv(command[0], command) == -1) {
            // execv failed
            perror("execv failed");
            va_end(args);
            return false;
        }
    } else {
        // Inside parent process
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // Wait failed
            perror("waitpid failed");
            va_end(args);
            return false;
        }

        // Check if child process exited normally
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // Child executed successfully
            va_end(args);
            return true;
        } else {
            // Child process did not exit successfully
            va_end(args);
            return false;
        }
    }

    va_end(args);
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);

    // Create an array of arguments with one extra slot for the NULL terminator
    char *command[count + 1];
    int i;

    // Collect the command arguments from the va_list
    for (i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL; // Null terminate the argument list

    // Fork the process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed
        perror("fork failed");
        va_end(args);
        return false;
    }

    if (pid == 0) {
        // Inside child process

        // Open the output file for writing, create it if it doesn't exist, and truncate it if it does
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            // Failed to open the output file
            perror("open failed");
            va_end(args);
            return false;
        }

        // Redirect stdout to the file descriptor using dup2
        if (dup2(fd, STDOUT_FILENO) == -1) {
            // Failed to redirect stdout
            perror("dup2 failed");
            va_end(args);
            return false;
        }

        // Close the file descriptor after redirecting
        close(fd);

        // Now call execv to execute the command
        if (execv(command[0], command) == -1) {
            // execv failed
            perror("execv failed");
            va_end(args);
            return false;
        }
    } else {
        // Inside parent process

        // Wait for the child process to finish
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            // Wait failed
            perror("waitpid failed");
            va_end(args);
            return false;
        }

        // Check if the child process exited normally
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            // Child executed successfully
            va_end(args);
            return true;
        } else {
            // Child process did not exit successfully
            va_end(args);
            return false;
        }
    }

    va_end(args);
    return false;
}
