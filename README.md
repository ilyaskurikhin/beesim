

               @@@@@@   @@@  @@@  @@@@@@@   @@@@@@@    @@@@@@    @@@@@@@@  
             @@@@@@@   @@@  @@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@@  
            !@@       @@!  @@@  @@!  @@@  @@!  @@@  @@!  @@@  !@@        
           !@!       !@!  @!@  !@!  @!@  !@!  @!@  !@!  @!@  !@!        
          !!@@!!    @!@  !@!  @!@@!@!   @!@!!@!   @!@  !@!  !@! @!@!@  
          !!@!!!   !@!  !!!  !!@!!!    !!@!@!    !@!  !!!  !!! !!@!!  
             !:!  :!:  !!:  !!:       !!: :!!   !!:  !!!  :!!   !!:  
           !:!    ::!!:!   :!:       :!:  !:!  :!:  !:!  :!:   !::  
       ::::::     ::::     ::       ::   :::   ::::::   ::: ::::  


               Copyright (C) 2016 Julia Besson & Ilya Skurikhin

Introduction
================================================================================

This program aims to simulate an environment in bees, living in hives, collect
pollen from flowers to keep the other bees in their hive alive.

Installation
================================================================================

## Dependencies

The only direct dependency of the code is SFML.
In order to compile with SCons, you will need to have that too.

In order to use the documentation, you will need Doxygen.


## Generating documentation

Documentation can be generated with doxygen, from the /doc directory.


Usage
================================================================================

The default run command is `scons application-run`, which will build and run the
code in default configuration, using the app.json config file in the /res 
directory. You can change the configuration file used with the `--cfg=<file>`
argument.

## Environment

By default, the world specified in the configuration file will be loaded, and 
Hives, with Bees, as well as Flowers will be generated accoring to the 
configuration file. 

Press `h` to add a Hive at the cursor location.

Press `f` to add a Flower at the cursor location.

Press `r` to generate a new environment.

Press `i` to load world to file.

Press `s` to save world to file

Press `d` to enter debug mode.


Documentation
================================================================================

Here is documentation for the logic behind each class of the program.


Classes
--------------------------------------------------------------------------------

### World

Gives texture of each cell that the other elements will them be built on top of.
The data from this class is heavy, and will therefore be saved on demand.




Lisence
================================================================================


