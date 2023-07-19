# MCD-coloring
This project contains tools to test reducibility of configurations for (9,4)-coloring
This code was used to present the results proposed in the paper "New reducible configurations for graph multicoloring with application to the experimental resolution of McDiarmid-Reed's Conjecture."
by J.-C. Godin and O. Togni
The Python program TestReducibility.py allows testing the reducibility of the configurations presented in the paper. It can be run by typing "python3 TestReducibility.py" in a shell command line.
The C++ program ReduceHexaGraph.cpp allows us to test the reducibility (i.e. (9,4)-coloring) of a random hexagonal graph using the reducible configurations of the paper.)
To be compiled with g++ ReduceHexaGraph.cpp -o ReduceHexaGraph -O3
