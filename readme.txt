Clang (I use a Mac) is used for building. There are two java files included for testing:
Rectangle.java
Student.java


1)To build, nav to this folder, then type the following into the command line;

make build

2) To run, type the following ;

./javasort (insertnamehere).java output.txt (you could change output.txt to another name, but then the makefile clean won't work properly).

3) To clean up both of the files (exe and txt);

make clean




