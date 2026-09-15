#include "execute.h"


static int setup_redirect(const char *file, int target_fd, bool append){
    int flags = O_WRONLY | O_CREAT;

    flags |= append ? O_APPEND : O_TRUNC;

    int fd = open(file, flags, 0644);
    if (fd < 0) {
        perror(file);
        return -1;
    }

    if (dup2(fd, target_fd) < 0) {
        perror("dup2");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

static int setup_redirections(const Redirect *redirect){
    if (redirect == NULL)
        return 0;

    if (redirect->stdout_file != NULL) {
        if (setup_redirect(
                redirect->stdout_file,
                STDOUT_FILENO,
                redirect->append) < 0) {
            return -1;
        }
    }

    if (redirect->stderr_file != NULL) {
        if (setup_redirect(
                redirect->stderr_file,
                STDERR_FILENO,
                redirect->append) < 0) {
            return -1;
        }
    }

    return 0;
}

static void reset_child_signals(void){
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

static void execute_path(const char *path, i8 *args[], Redirect *redirect){
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        reset_child_signals();

        if (setup_redirections(redirect) < 0)
            _exit(1);

        execv(path, args);

        perror(path);
        _exit(127);
    }

    if (waitpid(pid, NULL, 0) < 0)
        perror("waitpid");
}

void execute_program(i8 *command, i8 *args[], Redirect *redirect){
    /*
     * If command contains '/', execute it directly.
     * Otherwise search for it in PATH.
     */
    if (strchr(command, '/') != NULL) {
        execute_path(command, args, redirect);
        return;
    }

    char *path_env = getenv("PATH");

    if (path_env == NULL) {
        fprintf(stderr, "%s: command not found\n", command);
        return;
    }

    /*
     * strtok() modifies its input, so make a copy of PATH.
     * PATH can theoretically be longer than 1024 bytes, so using
     * strdup() avoids an arbitrary fixed-size limit.
     */
    char *path_copy = strdup(path_env);

    if (path_copy == NULL) {
        perror("strdup");
        return;
    }

    char *dir = strtok(path_copy, ":");

    while (dir != NULL) {
        char full_path[PATH_MAX];

        int written = snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            *dir ? dir : ".",
            command
        );

        if (written < 0 || (size_t)written >= sizeof(full_path)) {
            dir = strtok(NULL, ":");
            continue;
        }

        if (access(full_path, X_OK) == 0) {
            execute_path(full_path, args, redirect);
            free(path_copy);
            return;
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);

    fprintf(stderr, "%s: command not found\n", command);
}
