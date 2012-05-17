-- premake script for gorgone (creates the Makefile for the project)
--
-- Author: Brandon Harvey <brandon@oblong.com>
--
-- To build:
--     # Generate a Makefile
--     premake4 gmake
--
--     # Have a look at what the Makefile can do for you
--     make help
--
--     # Choose the configuration: 'debug' or 'release'.  Default: debug
--     make config=debug verbose=1
--
--     # To use clang, add CXX=clang CC=clang to the make line
--     make config=debug verbose=1 CXX=clang CC=clang
--
solution "gorgone"
   configurations { "Debug", "Release" }

   project "gorgone"
      kind "ConsoleApp"
      language "C++"
      files        { "**.h", "**.cpp" }
      includedirs  { "." }

      links { "boost_thread-mt" }
      linkoptions { "-pthread" }

      configuration "Debug"
           defines "DEBUG"
           flags   { "Symbols", "ExtraWarnings" }

      configuration "Release"
           defines "NDEBUG"
           flags   { "OptimizeSpeed", "ExtraWarnings", "FatalWarnings" }

      configuration "linux"
          -- todo: these are wrong, just placeholders
          includedirs { "/usr/local/include/boost/" } 
          libdirs { "/usr/local/lib/" }

      configuration "macosx"
          -- This is geared only to clang 3.1
          --buildoptions { "-std=c++0x", "-stdlib=libc++", "-Wno-c++11-extensions" }
          includedirs { "/usr/local/include/boost/" }
          libdirs { "/usr/local/lib/" }
          links   { "stdc++" }
