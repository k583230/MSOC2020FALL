// 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
#include "example.h"

// Structure for "dut":
// 1. "writemem" reads from a stream and fills up the tb buffer
//
// 2. "readmem"
//     Function implements DATAFLOW between "readmemA" and "readmemB".
//     1. "readmemA" reads a stream of indexes to tb and writes out the lookup
//        as a stream "req_strm"
//     2. "readmemB" reads the previous stream ("req_strm") as indexes to
//        another tb lookup and writes the sum of the looped up numbers
//     Problem: tb is read from both sub-functions and prevent DATAFLOW
//     Solution: Apply the new stable pragma

//-------------------------------------------------------
void readmemAB(strm_t &in_strm, strm_t &out_strm, int *tb)
{
AB:
    int sum = 0;
    value_t valTemp;
    for (int i = 0; i < 10; i++)
    {
#pragma HLS PIPELINE
    	value_t in = in_strm.read();
        int b = tb[in.data];
        if (b < 10 && in.data != 0)
        {
            sum += b;
        }

    }

    valTemp.data=sum;
    valTemp.keep = 0xF;
    valTemp.strb = 0;
    valTemp.user = 0;
    valTemp.id = 0;
    valTemp.dest = 0;
    valTemp.last=1;
    out_strm.write(valTemp);
    return;

}

void readmemA(strm_t &in_strm, strm_t &req_strm, int *tb)
{
//#pragma HLS PIPELINE
A:
    for (int i = 0; i < 10; i++)
    {
#pragma HLS PIPELINE
    	value_t in = in_strm.read();
        int b = tb[in.data];
        if (b > 10)
        {
        	in.data=0;
        }
        req_strm.write(in);
    }
    return;

}

//--------------------------------------------------------
void readmemB(strm_t &req_strm, strm_t &out_strm, int *tb)
{
//#pragma HLS PIPELINE
    value_t sum;
    sum.data = 0;
B:
    for (int i = 0; i < 10; i++)
    {
#pragma HLS PIPELINE
    	value_t r = req_strm.read();
        if (r.data != 0)
        {
            int b = tb[r.data];
            sum.data += b;
        }
    }
    out_strm.write(sum);
    return;
}

//------------------------------------------------------
void readmem(strm_t &in_strm, strm_t &out_strm, int *tb)
{
    strm_t req_strm;

#pragma HLS STREAM variable = req_strm depth = 4
//#pragma HLS DATAFLOW
#pragma HLS stable variable = tb
    //readmemA(in_strm, req_strm, tb);
    //readmemB(req_strm, out_strm, tb);

    readmemAB(in_strm, out_strm, tb);
    return;
}

//-------------------------------------
void writemem(strm_t &in_strm, int *tb)
{
load:
    for (int i = 0; i < 10; i++)
    {
#pragma HLS PIPELINE
        value_t in = in_strm.read();
        tb[i] = in.data;
    }
    return;
}

//-------------------------------
void example(strm_t &in1_strm, strm_t &in2_strm, strm_t &out_strm)
{
    // tb is read twice in "readmem"
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS INTERFACE axis register both port=in1_strm
	#pragma HLS INTERFACE axis register both port=in2_strm
	#pragma HLS INTERFACE axis register both port=out_strm
    int tb[10];
    #pragma HLS RESOURCE core = RAM_2P_BRAM variable = tb
    writemem(in1_strm, tb);
    readmem(in2_strm, out_strm, tb);
    return;
}
