from BuildConfig import *


Import('env')

srcEnv = env.Clone()

setupEnv(srcEnv)

objects = []

# Build all modules within the source directory
for dir in srcModules:

	for file in Glob(join(srcDir, dir, '*.cpp')):
		fileName = os.path.basename(str(file))

		fileName = fileName.split(".")[0]

		outDir = join(buildPath, dir, fileName)
		objects = objects + srcEnv.Object(outDir, file)


# Compile the remaining source files and create a program using the above libs
objects = objects + srcEnv.Object(Glob(join(srcDir, '*.cpp')))

srcEnv.Program(join(programPath, programName), objects)