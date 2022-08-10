# robot\_mapping\_simulator
A simulator that simulates a robot mapping a gridded area. The robot uses an
algorithm to move around the gridded area and identify obstacles. Multiple
different algorithms can be chosen and tried by the user. The size of the area
and the number of obstacles can be chosen by the user.

The simulator currently uses SFML to plot its grid, so SFML is a dependency. For
details on building this project from source, see INSTALL.md.

Short-term goals:

* Support for multiple algorithms that can be swapped out by the user
* Grid area and amount of obstacles determined by the user
* At least one algorithm which can successfully identify all obstacles in
  arbitrarily sized grids
* Cross-platform support between Windows and Linux

Long-term goals:

* A Qt GUI for choosing the algorithm, changing the grid size, setting the
  amount of obstacles, and stepping through the algorithm
* Cross-platform support between Windows, macOS, and Linux
