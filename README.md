# Shell

A **Unix shell written from scratch in C** implementing job control,
pipelines, I/O redirection, tab completion, and command history —
built directly on Linux system calls with no shell libraries.

---

## Features

* **Command execution** — PATH resolution, `fork`/`exec` based process spawning
* **Pipelines** — arbitrary length `cmd1 | cmd2 | cmd3` with correct file descriptor management
* **I/O redirection** — `>` `>>` `<` output, append, and input redirection
* **Job control** — `Ctrl+Z`, `bg`, `fg`, background execution with `&`, `jobs` command
* **Tab completion** — longest common prefix completion across commands and files
* **Command history** — persistent history across sessions loaded from and saved to disk
* **Variable assignment** — `NAME=value` syntax with `$NAME` and `${NAME}` expansion
* **Environment variables** — full access to `$HOME`, `$PATH`, `$USER` and all env vars
* **Built-in commands** — `cd`, `pwd`, `jobs`, `fg`, `bg`, `history`, `declare`, `exit`
* **Signal handling** — `SIGINT`, `SIGTSTP`, `SIGQUIT` correctly ignored by shell, restored in child processes
* **Working directory prompt** — prompt shows current directory, collapses home to `~`

---

## Architecture Overview

* **Parser** — reads raw input in raw mode, tokenises into command and argument arrays, detects pipes, redirects, and background operators

* **Executor** — resolves commands against PATH using `access()`, forks child processes, applies redirections via `dup2` before `execvp`

* **Pipeline** — chains N commands with N-1 pipes, each stage reading from the previous stage's write end

* **Job Control** — tracks foreground and background jobs with PIDs and status, reaps completed jobs before each prompt via `waitpid` with `WNOHANG`

* **Tab Completion** — reads raw terminal input character by character using raw mode, scans PATH and current directory for matches, inserts longest common prefix

* **History** — circular buffer of recent commands, persisted to disk on exit and loaded on startup

* **Variable Store** — hash table backed variable store for shell variables, falls back to `getenv` for environment variables

---

## Design Decisions

**Raw terminal mode for tab completion**

Standard `fgets` or `readline` would buffer input until Enter. To intercept Tab keystrokes the terminal is switched to raw mode using `tcsetattr` — input is read one character at a time, allowing Tab to trigger completion without submitting the line.

**`WNOHANG` for job reaping**

Zombie processes accumulate if children are not waited on. Rather than blocking on `waitpid` after every command, the shell calls `waitpid(-1, &status, WNOHANG)` before printing each prompt — non-blocking, cleans up any finished background jobs without stalling the foreground.

**Signal disposition across fork**

Child processes inherit the parent's signal handlers. The shell ignores `SIGINT`, `SIGTSTP`, and `SIGQUIT` at the top level. Each child restores default signal disposition immediately after `fork` and before `execvp` — otherwise foreground programs would incorrectly ignore `Ctrl+C`.

**`PATH_MAX` and dynamic allocation for PATH search**

The executor uses `PATH_MAX` for path buffers and `strdup` for copying the PATH environment variable — avoiding arbitrary fixed-size limits that could silently truncate long paths.

---

## Build & Run

```bash
make
./main
```

---

## Usage

```bash
# basic command
ls -la

# pipeline
ls -la | grep ".c" | wc -l

# background job
sleep 10 &

# I/O redirection
cat file.txt > output.txt
cat file.txt >> output.txt

# job control
sleep 20
^Z          # suspend
bg          # resume in background
fg          # bring back to foreground

# variable assignment and expansion
NAME=Lucas
echo $NAME
echo ${HOME}

# history
history 10  # show last 10 commands

# tab completion
ls De<Tab>  # completes to Desktop/
```

---

## Requirements

* Linux
* GCC
* Make

---

## Limitations

* No support for subshell execution `$(command)`
* No shell scripting — loops, conditionals, functions
* History limited to 256 entries
* Tab completion limited to 255 matches
* No multiline command editing

---