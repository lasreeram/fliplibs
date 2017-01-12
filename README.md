# fliplibs
C++ libraries


debug_lib - An error handling library. Support for multi-threaded applications is available

sockets_lib - A socket library for TCP and UDP sockets with classes for select/poll, SSL sockets

ipc_lib - An IPC mechanism using system V shared memory

threads_lib - A library for POSIX threads


Demo Programs

ec_crypto - A set of demo programs on using elliptic curve cryptography with openssl
(some sample google tests for ec_crypto class)

mongo_db - Some demo programs for mongo db 


***How to Build***:
Step 1 - Under each directory there is a file named autogen.sh. Execute this file for each directory to install files required by autotools
Step 2 - run from build location "<src_path>/configure [--prefix <prefix_path>]" from a build directory 
Step 3 - run "make" from a build directory 
Step 4 - Install from build directory to prefix

***Macros for building***

#The different directories - source, build, prefix
export SDIR=~/src
export PDIR=~/pdir
export BDIR=~/bdir
alias sdir='cd ~/src'
alias pdir='cd ~/pdir'
alias bdir='cd ~/bdir'

#configure - Step 2
bcfg()
{
        mkdir -p $BDIR/$1;cd $BDIR/$1;$SDIR/$1/configure --prefix=$PDIR
}

#make - Step 3
bmake()
{
        mkdir -p $BDIR/$1;cd $BDIR/$1;make
}

#install - Step 4
binstall()
{
        cd $BDIR/$1;make install
}

#clean 
bclean()
{
        cd $BDIR/$1;make clean
}
