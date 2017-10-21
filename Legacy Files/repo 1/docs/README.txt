Author: Adam Walls
SID: 10525922
LinBlueID: amwa262

Program contents: Assignment1

Directory: 
	- Assignment1
contents: 
	- docs(directory)
	- include(directory)
	- src(directory)
	- run_make.sh(script): can be called using "sh run_make.sh" in the root directory in order to call the makefile
purpose: 
	- intended to be the root directory for all files an 

Directory: 
	- docs
contents: 
	- cleanup_out_file.txt: contains the database contents upon cleanup
	- README.txt
	- Sample_input_file.txt
purpose: 
	- docs - intended to hold all I/O files for these programs

Directory:
	-  include
contents: 
	- include.h
purpose:
	- contains the definitions for thwe student info structs
	- contains the definitions of the semaphore Wait, Signal and GetSemaphs functions
	- contains some descriptive macro definitions for semaphores and loading ini files

Directory: 
	- src
contents:
	- exe(directory): contains the excutables created by "make"
	- change.c: source code for modifying student info in shared memory
	- clean.c: source code for removing semaphores and shared memories
	- header.c: source code providing header.h function implementaions
	- load.c: source code allowing an ini file to load student info into shared memory
	- print.c: source code for printing the contents of shared memory
	- query.c: source code for searching for student id in shared memeory
	- makefile: file for compiling executables and placing them into the exe directory

purpose: 
	- contains source code for creating executables
	- contains folder containing the executables

HOW TO COMPILE ALL PROGRAMS:
	1) Sit in the root directory
	2) Call "sh run_make.sh", you should see that all the programs successfully compile

HOW TO RUN LOAD:
	- From the root directory type: 
		./src/exe/Load <Database initialization file>

HOW TO RUN CHANGE:
	- From the roo directory type: 
		./src/exe/Change <Password> <Student ID> <Student First Name> <Student Last Name> <Student Address> <Student Telephone Number>

HOW TO RUN QUERY:
	- From the root directory type: 
		./src/exe/Query <Student ID>

HOW TO RUN PRINT:
	- From the roo directory type:
		./src/exe/Print

HOW TO RUN CLEAN:
	- From the root directory type:
		./src/exe/Clean
