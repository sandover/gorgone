-- premake script for gorgone (creates the Makefile for the project)
--
-- Author: Brandon Harvey <brandon@oblong.com>
--
-- To build:
--     premake4 gmake  # Generates a GNU Makefile
--
--     # Know your options
--     make help
--
--     # Builds the default configuration, which is debug
--     make
--
--     # Clang?  cores?  etc.
--     make CXX=clang CC=clang -j4 verbose=1
--
solution "gorgone"
   configurations { "Debug", "Release" }

   project "gorgone"
      kind "ConsoleApp"
      language "C++"
      files        { "**.h", "**.cpp" }
      includedirs  { "." }

      links { "boost_thread-mt" }
      --linkoptions { "-pthread" }

      configuration "Debug"
           defines   "DEBUG"
           flags   { "Symbols", "ExtraWarnings" }

      configuration "Release"
           defines   "NDEBUG"
           flags   { "OptimizeSpeed", "ExtraWarnings", "FatalWarnings" }

      configuration "linux"
          -- todo: these paths are wrong, just placeholders
          includedirs { "/usr/local/include/boost/" }
          libdirs     { "/usr/local/lib/" }

      configuration "macosx"
          -- This is geared only to clang 3.1
          --buildoptions { "-std=c++0x", "-stdlib=libc++", "-Wno-c++11-extensions" }
          includedirs { "/usr/local/include/boost/" }
          libdirs     { "/usr/local/lib/" }
          links       { "stdc++" }
