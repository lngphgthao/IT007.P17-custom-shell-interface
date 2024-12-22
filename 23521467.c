#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h> 

#define TRUE 1
#define MAX_LINE 80 // do dai toi da cua command

pid_t pid;
int pipe_flag = 0;
int backquotes_flag = 0;

void sigint_handler();
void sigabrt_handler();
void check_input(char user_input[]);
void parse_input(char user_input[], char *args[], char *sec_args[], char **filein, char **fileout);
void backquotes_handling(char *arg, char *args[], int *num_args);
void cmd_handling(char *args[], char *sec_args[], char *filein, char *fileout);

//xu ly signal Ctrl+C
void sigint_handler() {
    if (pid > 0) 
        kill(pid, SIGINT);
    else {
        printf("\nit007sh> ");
        fflush(stdout);
    }
}

//xu ly signal Ctrl+'\'
void sigabrt_handler() {
    exit(0);
}

// kiem tra cac lenh dac biet
void check_input(char user_input[]) {
    if (strcmp(user_input, "") == 0) 
        return;
    else if (strcmp(user_input, "exit") == 0) 
        exit(0);
    else if (strcmp(user_input, "help") == 0) {
        execlp("bash", "bash", "-c", "help", NULL);
        return;
    }
}

//tach cac phan tu trong command
void parse_input(char user_input[], char *args[], char *sec_args[], char **filein, char **fileout) {
    pipe_flag = 0; backquotes_flag = 0;
    int num_args = 0;
    char *temp = strtok(user_input, " ");

    while (temp != NULL) {
        if (temp[0] == '`') {
            backquotes_flag = 1;
            backquotes_handling(temp, args, &num_args);
        } else {            
            args[num_args] = temp;
            num_args++;
        }
            temp = strtok(NULL, " ");
    }
    args[num_args] = NULL;

    num_args = 0;

    while (args[num_args] != NULL) {
        if (strcmp(args[num_args], "<") == 0) {
            args[num_args] = NULL;
            *filein = args[num_args + 1];
        } else if (strcmp(args[num_args], ">") == 0) {
            args[num_args] = NULL;
            *fileout = args[num_args + 1];
        } else if (strcmp(args[num_args], "|") == 0) {
            args[num_args++] = NULL; pipe_flag = 1;

            int i = 0;

            while (args[num_args] != NULL) {
                sec_args[i] = args[num_args];
                args[num_args] = NULL;
                num_args++; i++;
            }
            sec_args[i] = NULL;
            break;
        }
        num_args++;
    }
}

// xu ly backquotes
void backquotes_handling(char *arg, char *args[], int *num_args) {
    char backquotes[MAX_LINE];
    strncpy(backquotes, arg + 1, strlen(arg) - 2);
    backquotes[strlen(arg) - 2] = '\0';

    int fd[2];
    pipe(fd);
    pid = fork();

    if (pid < 0) {
        perror("Error");
        exit(1);
    } 

    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        char *bq_args[MAX_LINE];
        char *filein = NULL, *fileout = NULL;
        char *sec_args[MAX_LINE];
        parse_input(backquotes, bq_args, sec_args, &filein, &fileout);
        execvp(bq_args[0], bq_args);
    } else {
        close(fd[1]);
        wait(NULL);

        char output[MAX_LINE];
        read(fd[0], output, MAX_LINE);
        close(fd[0]);
        char *output_token = strtok(output, " \n");
        while (output_token != NULL) {
            args[*num_args] = strdup(output_token);
            (*num_args)++;
            output_token = strtok(NULL, " \n");
        }
    }
}

// xu ly cac lenh
void cmd_handling(char *args[], char *sec_arg[], char *filein, char *fileout) {

    if (filein != NULL) {
        int fd = open(filein, O_RDONLY);
        if (fd == -1) {
            perror("Error");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (fileout != NULL) {
        int fd = open(fileout, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (pipe_flag) {
        int fd[2];

        pipe(fd);

        pid_t sec_pid = fork();

        if (sec_pid == 0) {
            close(fd[0]); dup2(fd[1], STDOUT_FILENO);
            close(fd[1]); execvp(args[0], args);
        } else {
            close(fd[1]); dup2(fd[0], STDIN_FILENO);
            close(fd[0]); execvp(sec_arg[0], sec_arg);
        }
        exit(0);
    } 
    
    execvp(args[0], args);
    exit(0);
}

int main() {
    char user_input[MAX_LINE];
    char *args[MAX_LINE];

    signal(SIGINT, sigint_handler);
    signal(SIGABRT, sigabrt_handler);

    while (TRUE) {  
        
        printf("it007sh> ");
        fflush(stdout);

        // nguoi dung nhap lenh
        fgets(user_input, MAX_LINE, stdin);
        user_input[strcspn(user_input, "\n")] = '\0';
        
        check_input(user_input);

        char *filein = NULL, *fileout = NULL;
        char *sec_args[MAX_LINE];
        parse_input(user_input, args, sec_args, &filein, &fileout);

        pid = fork();

        if (pid < 0) {
            perror("Error");
            exit(1);
        } else if (pid == 0) {
            cmd_handling(args, sec_args, filein, fileout);
        } else wait(NULL);
    }
    return 0;
}