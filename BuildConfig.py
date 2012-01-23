import os
import sys

from SCons.Script import *
from os.path import join
from os import path

#------------------------------------------------------------------------------#

arch32="32"
arch64="64"

# Program to output
programName = 'strata'

# The compiler to use throughout the compilation process
compilerType = 'g++'

# Where to place all build-related objects and files
buildPath = "build"

# The location to compile the program
programPath = buildPath

# No source besides include files and test files should exist outside of here
srcDir = "Engine"

# Test directory
testPath = "test"

externPath = "extern"


sysDefines = {"GL_GLEXT_PROTOTYPES":"1"} #{"FIELD3D_EXTRA_NAMESPACE":"SPI"}
# These lie under the source directory

# Paths to include from
includePaths = [
                   join(srcDir, 'extern'),
                   join(srcDir, 'extern/include/'),
                   join(srcDir, 'cpp'),
                   join(srcDir, 'objc')
                  ]

# Libraries to reference
sysLibs = [
           'GL',        # OpenGL Libraries
           'glut',      # Glut libraries
           'png'        # Used to load textures
          ]


#------------------------------------------------------------------------------#

## pulled from Field3D build script.  Used to determine arch string
def architectureStr():
    if ARGUMENTS.get('do64', 0):
        return arch64
    else:
        return arch32


## isDebugBuild returns whether or not the "debug=1" flag was included while
#  running scons
def isDebugBuild():
    return ARGUMENTS.get('debug', 0)


## isDebugBuild returns whether or not the "test=1" flag was included while
#  running scons
def isTestBuild():
    return ARGUMENTS.get('test', 0)
    

## getBuildPath finds the path to the root directory and tacks on the build path
#  @param pathToRoot: Necessary to normalize the path to the root directory
def getBuildPath(pathToRoot = "."):
    return join(pathToRoot, buildPath)
    
    
## buildPaths takes the list of paths and sanitizes them for whatever folder we
#  are currently in (according to pathToRoot.)  This fixes any relative path
#  craziness
#  @param paths: List of paths to sanitize
#  @param pathToRoot: The path that gets us to the root directory
def buildPaths(paths, pathToRoot="."):
    mapPath = lambda x:  x if path.isabs(x) else join(pathToRoot, x); 
    return map(mapPath, paths)


## getSource will take the module requested and normalize it based on the 
#  pathToRoot variable, returning all source files within
#  @param module: The directory under the source dir to grab cpp files from
#  @param pathToRoot: Used to normalize the path to the source dir
def getSource(module, pathToRoot = "."):
    #print join(pathToRoot, buildPath, module, "*.cpp")
    
    return Glob(join(pathToRoot, srcDir, module, "*.cpp"))



## getSource will take the module requested and normalize it based on the 
#  pathToRoot variable, returning all source files within
#  @param module: The directory under the source dir to grab cpp files from
#  @param pathToRoot: Used to normalize the path to the source dir
def buildSource(module, env, pathToRoot = "."):
    #print join(pathToRoot, buildPath, module, "*.cpp")
    objects = []
    for file in Glob(join(pathToRoot, srcDir, module, '*.cpp')):
        fileName = os.path.basename(str(file))
        
        fileName = fileName.split(".")[0]

        outDir = join(getBuildPath(pathToRoot), module, fileName)
        objects = objects + env.Object(outDir, file)
    return objects


#------------------------------------------------------------------------------#
## setupEnv will take the current environment and path to root directory and 
#  apply the correct paths and library locations to the environment
#  @param env: SCons environment
#  @param pathToRoot: Used to normalize us to the root directory
def setupEnv(env, pathToRoot = "."):
    
    applyDir = lambda x, y: "%s/%s" % (x, y);
    
    # Include paths
    env.Replace(CPPPATH = buildPaths(includePaths, pathToRoot))
    
    # Library paths
    # env.Replace(LIBPATH = buildPaths(sysLibPaths, pathToRoot))
    
    # Libraries
    env.Replace(LIBS = sysLibs)
    
    # Defines
    env.AppendUnique(CPPDEFINES=sysDefines)

    # Debug symbols    
    if isDebugBuild():
        env.Append(CCFLAGS = ["-g"])
        
    #env.Append(LDFLAGS="-rpath=%s" % path.abspath(join(externPath, "lib")))

        
"""
def updateEnv(env, pathToRoot = "."):
            
    applyDir = lambda x, y: "%s/%s" % (x, y);
    
    # Include paths
    env.Replace(CPPPATH = buildPaths(sysIncludePaths, pathToRoot))
    
    # Library paths
    env.Replace(LIBPATH = buildPaths(sysLibPaths, pathToRoot))
    
    if isDebugBuild():
        env.Append(CCFLAGS = ["-g"])
    
    # Libraries
    env.Replace(LIBS = sysLibs)
"""