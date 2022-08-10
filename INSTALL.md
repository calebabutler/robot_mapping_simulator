
Compiling on Windows
====================

Windows users need two things: Git for Windows and Build Tools for Visual
Studio. Git for Windows is needed to download and modify the git repository.
Build Tools for Visual Studio builds the project.

Download Git for Windows from https://gitforwindows.org. Install using the
installer with the default components and settings (if you have preferences for
specific settings, feel free to change them, but the defaults are fine for this
project).

Download Build Tools for Visual Studio from
https://visualstudio.microsoft.com/downloads/. Build tools are under the Tools
for Visual Studio tab. You can also download all of Visual Studio, but it is not
required. Installing the build tools will install the Visual Studio Installer;
in this installer, you want to select the option to download build tools for
"Desktop development with C++".

Downloading the git repository
------------------------------

Open Developer Powershell for VS, installed by the Visual Studio Installer.
Use the command "pwd" to get your current directory, and "cd" to change your
directory. You want to set your directory to the directory where you want
the repository. Example:

    > cd 'C:\Users\Peter\Desktop\GitHub\'
    > pwd # Ensures the first command put us in the correct directory

Then you will use git to install the current repository, with the command:

    > git clone https://www.github.com/calebabutler/robot_mapping_simulator.git

Finally change directory into the git repository, and see if the files in
the directory are as expected.

    > cd '.\robot_mapping_simulator\'
    > dir # To see the files in the current directory

Building the project
--------------------

First you need to download the SFML source files. This can be done with the
command:

    > Invoke-WebRequest -UseBasicParsing -URI 'https://www.sfml-dev.org/files/SFML-2.5.1-sources.zip' -OutFile 'SFML.zip'

Next, extract the zip file downloaded:

    > Expand-Archive '.\SFML.zip'

Next, make a new directory called 'build', and change the current directory
to it:

    > mkdir build
    > cd build

Then run cmake, this project's build system:

    > cmake ..
    > cmake --build .

Finally, copy the built SFML dll files into the directory with the
executable:

    > cp .\SFML\SFML-2.5.1\lib\Debug\*.dll .\sources\Debug\

Then run the program:

    > .\sources\Debug\robot_mapping_simulator.exe

Doing incremental builds
------------------------

Rebuilding the project a second time is much easier, since all dependencies
are already downloaded and built. All you need to do is:

    > cd build
    > cmake --build .

Then run the program:

    > .\sources\Debug\robot_mapping_simulator.exe

Pushing changes to GitHub
-------------------------

First, ensure you have configured git to use the right user name and email:

    > git config --global user.name "NAME"
    > git config --global user.email "EMAIL"

Make sure git is using your preferred editor. As an example:

    > git config --global core.editor "notepad.exe"

Then, make sure the current directory is the root directory of the
repository (the directory with this file). Add all file changes to the git
index (a place to hold file changes for the next commit) with the command:

    > git add .

Make a commit. Git will ask you to write a commit message to describe what
this commit changes/adds. You must write a commit message for every commit.
The first line is a summary of the message; it should be under 50
characters. After the first line you should write a detailed (as detailed as
you want, it doesn't have to be very long) message about what this commit
does.

    > git commit

To ensure you made a commit you can look at the git log:

    > git log

Sync your git repository with GitHub. This will not remove your changes but
will instead try to merge your changes with the changes in GitHub:

    > git pull origin main --rebase

Make GitHub sync with your git repository:

    > git push origin main

Compiling on Linux
==================

Compiling on Linux is straightforward. First, open a terminal and install all
dependencies required by the project. On a Debian/Ubuntu system:

    > sudo apt install build-essential libsfml-dev git cmake

Then, change your directory using the "cd" command to where you want the github
repository. Run:

    > git clone https://www.github.com/calebabutler/robot_mapping_simulator.git
    > cd robot_mapping_simulator
    > mkdir build
    > cd build
    > cmake ..
    > cmake --build .
    > ./sources/robot_mapping_simulator

To do future incremental builds:

    > cd build
    > cmake --build .
    > ./sources/robot_mapping_simulator
