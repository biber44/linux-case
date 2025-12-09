# NOTE: There's an issue with the pretty printers when using the toolchain gdb.
# Check these articles:
# - https://youtrack.jetbrains.com/issue/CPP-14177#focus=streamItem-27-3371983.0-0
# - https://intellij-support.jetbrains.com/hc/en-us/community/posts/115000098784-Python-error-when-debugging

# These make gdb never pause in its output.
set height 0
set width  0

# Change the color of the prompt. Useful when searching the history.
set extended-prompt \[\e[0;31m\]gdb$ \[\e[0m\]

# Don't download any debug info from remote servers.
set debuginfod enabled off

# Tell gdb where to look for source files.
# 1) Yes, it's a build directory, just like it is done when using QtCreator.
# 2) If you set something like "dir .", gdb won't find anything and therefore not hit any breakpoints.
# 3) Debugging shared libs and executables on the host still works, if even this directory seems wrong.
dir build/gcc-debug

# Required for debugging shared libraries (especially on the target), irrelevant for debugging the main application and tests.
set solib-search-path build/gcc-debug/build/bin
