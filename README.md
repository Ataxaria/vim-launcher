# vim-wsl-launcher
## Disclaimer: This code is AI slop.

### Compilation command
```bash
windres resource.rc -o resource.o
gcc main.c resource.o -o VimLauncher.exe -O2 -lshlwapi
```

### How to use
1. Open the properties of the file you want to open and change the default behaviour for that file extension.
2. Select "Choose an app on your PC" in the dialog box and select the compiled executable.
3. _Voila!_


todo pass in the name of the terminal profile
