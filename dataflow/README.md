# Memory porting and II







<!-- ABOUT THE PROJECT -->
## About The Project
* The project includes two parts. First saving ten numbers into an array, and then accumulate the numbers according to the input index except 0 or out of boundary.

**Directory structure**
* **syn_report** - HLS synthesis reports for different solutions
* **README.md** - introduce the project, algorithm, reference ....
* **design_1.bit, design_1.hwh** - bitstream and hw_handoff for FPGA
* **dataflow.ipynb** - FPGA python host program 
* **example.cpp** - kernel function
* **example.h** - header file
* **example_test.cpp** - testbench

<!-- USAGE EXAMPLES -->
## Usage
* **HLS Simulation:** use example.cpp example.h as source and example_test.cpp as testbench on Vivado HLS
* **FPGA:** use dataflow.ipynb to run on Zedboard
## Algorithm
* Integer addition of pre-saved array 

## References
* ~Xilinx/Vivado/2019.2/examples/design

<!-- CONTRIBUTING -->
## Contributing
* resolve II violation and improve latency






