dockable dashboard Plugin for OpenCPN 
-------------------------------------

this Plugin is the original taken code from OpenCPN main code dashboard_pi and works in same way.

- It makes it possible to dock a dashboard window on OpenCPN canvas, by deleting the caption of the window. so the used size will get smaler on screen.
- the "Engine dashboard" is full included.

  This code is only tested with Windows:
  compile the code, create the installation package and install it to OpenCPN.
  This will replace the dashbord_pi.dll file.
  Build as normally:

    cd ..
    cd build
    cmake ..
    make
    make install  
 
  