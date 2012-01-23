from BuildConfig import *

import platform

env = Environment(CC = compilerType)
    
setupEnv(env, ".")
# Set up environment to be receptive to shared objects in the ../lib directory
#env.Append( LINKFLAGS = Split('-z origin') )
#env.Append( RPATH = env.Literal(os.path.join('\\$$ORIGIN', os.pardir, 'lib')))

# Do everything else

objects = []

sources = [
           Glob(join(srcDir, "cpp/*.cpp"))
           ]

# Build all modules within the source directory
for src in sources:

    objects += env.Object(src)


# Compile the remaining source files and create a program using the above libs
objects = objects + env.Object(Glob(join(srcDir, '*.cpp')))

env.Program(join(programPath, programName), objects)