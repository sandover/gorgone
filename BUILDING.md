
Mac instructions (with premake)
===============================

1.  Ensure boost 1.48 or later.  Can do this through Homebrew as
      
       brew install boost

   This puts boost at /usr/local/include and /usr/local/lib.

   ** Note ** The premake build script included here (premake4.lua) assumes 
   these locations.  Edit for your local conditions.

2.  Ensure premake 4 or later is installed.  Homebrew way:

       brew install premake

3.  Go see premake4.lua


Linux instructions
==================

Same as above, but with different paths and apt-get.  Recent-enough premake will have to be obtained directly from premake site (add a new source to /etc/apt/sources.list), or by using a newer Ubuntu than 10.

