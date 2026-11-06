#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlwapi.h> 

int main(int argc, char *argv[])
{
    // Increased size slightly to avoid edge-case overflow warnings
    char command[8192];
    char linux_path[4096] = {0};
    char full_path[4096] = {0};
    char parent_dir[4096] = {0};

    // If no file is provided, just open WT with a blank Vim
    if (argc < 2) {
        snprintf(command, sizeof(command),
                 "wt.exe -p \"Ubuntu\" wsl vim");
        return system(command);
    }

    // Resolve the absolute Windows path of the target file
    if (!GetFullPathNameA(argv[1], sizeof(full_path),
                          full_path, NULL)) {
        fprintf(stderr, "Could not resolve file path.\n");
        return 1;
    }

    // Use snprintf instead of strncpy to guarantee null-termination
    // and completely satisfy GCC's stringop-truncation check.
    snprintf(parent_dir, sizeof(parent_dir), "%s", full_path);
    
    // PathRemoveFileSpecA strips the file name, leaving just the directory
    PathRemoveFileSpecA(parent_dir);

    // Convert the full Windows path to a Linux path for Vim
    snprintf(command, sizeof(command),
             "wsl.exe wslpath -a -u \"%s\"",
             full_path);

    FILE *pipe = _popen(command, "r");

    if (!pipe) {
        fprintf(stderr, "Could not run wslpath.\n");
        return 1;
    }

    if (!fgets(linux_path, sizeof(linux_path), pipe)) {
        _pclose(pipe);
        fprintf(stderr, "Could not convert file path.\n");
        return 1;
    }

    _pclose(pipe);

    // Strip trailing newlines from the wslpath output
    linux_path[strcspn(linux_path, "\r\n")] = '\0';

    /*
     * Pass the parent directory to WT (-d) and the converted path to Vim
     */
    snprintf(command, sizeof(command),
             "wt.exe -p \"Ubuntu\" -d \"%s\" wsl vim \"%s\"",
             parent_dir, linux_path);

    return system(command);
}

