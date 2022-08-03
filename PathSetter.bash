# Set the necessary paths for the COMP354 OS Projects.

# Set this variable to the path to the Comp354Projects directory.
COMP354_HOME=/Users/walnuthanhan/Desktop/Comp354Projects

# Add environment variables for Bochs and add it to the path.
export BOCHS_HOME=$COMP354_HOME/bochs-2.4.2
export BXSHARE=$BOCHS_HOME/bios
export LTDL_LIBRARY_PATH=$BOCHS_HOME/bochs.app/Contents/MacOS/lib
export PATH=$PATH:$BOCHS_HOME
export DISPLAY=:0.0

# Add environment variables for bcc, as86, ld86 and related tools and add them to the path.
export DEV86_HOME=$COMP354_HOME/dev86-0.16.17
export PATH=$PATH:$DEV86_HOME/bin:$DEV86_HOME/bcc:$DEV86_HOME/cpp:$DEV86_HOME/unproto
export PATH=$PATH:.
