# Memory porting and II







<!-- ABOUT THE PROJECT -->
## About The Project
* The project shows how memory porting affects latency and II taking matrix multiplication as example

**Directory structure**
* **syn_report/** - HLS synthesis reports for different solutions
* **README.md** - introduce the project, algorithm, reference ....
* **design_1.bit, design_1.hwh** - bitstream and hw_handoff for FPGA
* **matrixmul.cpp** - kernel function
* **matrixmul.h** - header file
* **matrixmul_test.cpp** - testbench
* **memory_port_ii.ipynb** - FPGA python host program 

<!-- USAGE EXAMPLES -->
## Usage
* **HLS Simulation:** use matrixmul.cpp, matrixmul.h as source and matrixmul_test.cpp as testbench on Vivado HLS
* **FPGA:** use memory_port_ii.ipynb to run on Zedboard
## Algorithm
* Matrix multiplication

## References
* ~Xilinx/Vivado/2019.2/examples/design

<!-- CONTRIBUTING -->
## Contributing
* resolve limited port issue and improve latency






