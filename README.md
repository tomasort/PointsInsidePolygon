# Points inside the polygon 

This program returns all the points inside of a polygon. 

The Idea behind this algorithm is simple. Simply count how many times a segment formed by each point and a randomly chosen point intercepts the edges of the polygon. If this count is odd, then the point selected must be inside the polygon. On the other hand, if the count is even, then the point must be in the outside of the polygon.

## Input

The input of the program is a collection of points and a polygon. 

## Output

The output of the program is the collection of points that are inside the polygon. 

# Build the project

To run use cmake. Make a directory called build, go into it and call cmake from there. 
```
mkdir build
cd build
cmake ../
```
Now you can call ``make`` to create the binary files and run the programs. 