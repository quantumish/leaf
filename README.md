<p align="center"><img style="float: right; width: 120px;" src="https://i.imgur.com/xz7unmY.png"></p>

# firefly
Energy profiler for large distributed systems. Built for tartanhacks24!


To run the Firefly profiler and generate the associated JSON:
```
mkdir build
cmake ..
make
./leaf
```

To run the GUI on a generated profile (JSON): 
```
cd ui-beta
mkdir build
cmake ..
make
./ftxui-starter
```
