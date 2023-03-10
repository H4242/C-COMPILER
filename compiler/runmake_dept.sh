# to get the required packages (ubuntu 21.10):
#   sudo apt install     antlr4 libantlr4-runtime-dev default-jdk
# the ANTLRJAR part below is copied from /usr/bin/antlr4
make ANTLRJAR=../antlr/jar/antlr-4.9.2-complete.jar ANTLRINC=../antlr/include ANTLRLIB=../antlr/lib/libantlr4-runtime.a "$@"

# You can override these from the command-line, as illustrated by the various `runmake_*.sh` scripts
