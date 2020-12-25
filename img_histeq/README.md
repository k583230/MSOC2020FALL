# Image histogram equalization







<!-- ABOUT THE PROJECT -->
## About The Project
* The project can perform histogram equalization for a RGB image. However, the testbench and the previous histogram(used for equalizing histogram) set in this project is only for one channel, the work on FPGA is only the equalization result from the input G channel.

**Directory structure**
* **HLS/** - files for HLS 
	* syn_report/ - HLS synthesis reports for different solutions
	* test_data/ - image and input array for source and testbench
	* img_hist1.cpp - kernel function
	* img_hist1.h - header file
	* main_test.cpp - testbench
	* other files - function or defines included by testbench and source
* **data/** - image input for python host program 
* **README.md** - introduce the project, algorithm, reference ....
* **design_2_hist.bit, design_2_hist.hwh** - bitstream and hw_handoff for FPGA
* **img_hist.ipynb** - FPGA python host program 
* **.npy** - output arrays from host program
* **visualize.py** - python program using output arrays to show the result image


<!-- USAGE EXAMPLES -->
## Usage
* **HLS Simulation:** use img_hist1.cpp as source and main_test.cpp as testbench on Vivado HLS
* **FPGA:** use img_hist.ipynb to run on Zedboard
## Algorithm
* Transfer the input RGB image to YUV domain
* Use previous histogram and Y channel value to update new Y
* Transfer back to RGB domain
* Update new histogram for future usage

## References
* [HLx examples](https://github.com/Xilinx/HLx_Examples/tree/master/Vision/img_histEq)

<!-- CONTRIBUTING -->
## Contributing
* modify interface for Zedboard usage and visualize result
* change memory storage to improve latency
* resolve II violation






