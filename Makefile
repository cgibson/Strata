all:
	scons -j 5 .

debug:
	scons -j 5 debug=1 .

clean:
	scons -c .
	rm -r build
