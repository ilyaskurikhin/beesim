
       .d8888b. 888     888      8888888b. 8888888b.  .d88888b.  .d8888b.  
      d88P  Y88b888     888      888   Y88b888   Y88bd88P" "Y88bd88P  Y88b 
      Y88b.     888     888      888    888888    888888     888888    888 
       "Y888b.  Y88b   d88P      888   d88P888   d88P888     888888        
          "Y88b. Y88b d88P       8888888P" 8888888P" 888     888888  88888 
            "888  Y88o88P        888       888 T88b  888     888888    888 
      Y88b  d88P   Y888P         888       888  T88b Y88b. .d88PY88b  d88P 
       "Y8888P"     Y8P          888       888   T88b "Y88888P"  "Y8888P88 
                                                                     

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

The representation of the world is toric, and therefore for most elements, there
is no boundard to the world, this includes:
  - seeds during generation
  - humidity calculations
  - bees flying and detecting flowers
  - hiveable detection
  - bear activity

#### Regeneration

During regeneration, seeds are created, they move randomly on the map and pass 
their texture to the cell that they land on. Once the seeds have been moved a
number of times defined in the config file, the resulting world goes through 
smoothing, during which islands of a certain texture are eliminated.

##### Humidity calculation

Humidity levels are generated by calculating a matrix of humidity as a function
of distance for one quadrant of the area that would need to be humidified (while
the humidity is higher than the threshold specified). This matrix is then added 
to the surrounding cells of a water cell.

###### Random Hive & Flower generation

To test a set of configuration settings, you can set the number of hives and 
flowers to be generated automatically at each startup, or load from file. These
settings are in the 'initial' section of the config file.

### Env

The environtent contains all the elements of the simulation, including the 
world, the hives, and the flowers. 

### Movable

This class contains methods that move objects through the world while
interacting with the environment.

### Interfaces

There are a certain number of interfaces available in order to guarantee that 
certain actions can be performed on a set of classes that inherit from these
interfaces.

#### Drawable

Guarantees the implementation of a `void drawOn(sf::RenderTexture)` method.

#### Updatable

Guarantees the implementation of a `void update(sf::Time)` method.

#### Configurable

Guarantees the implementation of a `void reloadConfig()` method. These methods
are called when the user wants the configuration file to be reloaded, which 
would be most useful when regenerating the world afterwards. Since this 
interface is inherited by many classes, its inheritance is virtual in most
cases, such that intermediate methods can be defined to reload the attributes of
a higher class, but it is up to the lowest class to call all the higher methods
should the lowest one be called.



License
================================================================================


