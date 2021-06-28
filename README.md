# algo-project
.
Project Name: Algorithmic Step towards a Clean City
Development of an algorithm for optimal dustbin placement in a smart city, considering hotspots and other factors

Compiler Req: GCC

Input Format:

1st Line: (int value) The number of points of intersection of all roads 
2nd Line: (int value) The number of roads in our network | Suppose 'E'
3rd Line: (int value) Max allowable distance b/w consecutive dustbins

	  Vertices are represented by 0-based indexing

Next 'E' lines: Input the edges representing the roads (Each road connects two vertices)
(int value), (int value), (int value):	(index of either vertex), (index of the other vertex), (length of the road)



Output Format:

First 'E' lines contain the total dustbins to be installed on each road(edge).
Then, last line contains the total number of dustbins installed.
