#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_defines.h"
//#include "img_hist1.h"
/* ######################################################################### */

// update the histogram and output the CDF
void update_histogram(uint19 inp_hist[GRAY_LEVELS], uint19 out_hist[GRAY_LEVELS],
		               uint25 inp_cdf[GRAY_LEVELS],  uint25 out_cdf[GRAY_LEVELS])
{
	L0: for (unsigned char i=0; i<(GRAY_LEVELS-1); i++)
	{
#pragma HLS PIPELINE
			  out_hist[i] = inp_hist[i];
			  //inp_hist[i] = 0; //reset histogram array for next computation
			  out_cdf[i]  = inp_cdf[i];
	}
	return;
}


/* ######################################################################### */

// computes the Cumulative Density Function cdf[] from the Histrogram hist[]
void compute_cdf(uint19 hist[GRAY_LEVELS], uint25 cdf[GRAY_LEVELS])
{
	uint8 i, k;


CDF_L1:	for (k = 0; k < GRAY_LEVELS-1; k++)
	{
	    uint25 tmp_cdf=0;
		CDF_L2: for (i = 0; i<=k; i++)
		{
			#pragma HLS LOOP_TRIPCOUNT  min=1 max=256 avg=256

			// cdf for current picture from past picture histogram
			tmp_cdf = tmp_cdf + hist[i];
		}
		cdf[k] = tmp_cdf;
	}

	return;

}



/* ######################################################################### */
/*
void top_img_hist_equaliz1( uint11 width, uint11 height, uint25 cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
							RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])*/
void top_img_hist_equaliz1( uint11 width, uint11 height, uint25 cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
							strm_t* in_strm, strm_t* out_strm)
{
#pragma HLS DEPENDENCE variable=hist array intra false
//#pragma HLS DATA_PACK variable=out_img
//#pragma HLS DATA_PACK variable=inp_img


//#pragma HLS array_partition variable=cdf complete




//	RGB_t yuv[MAX_WIDTH*MAX_HEIGHT];    /* NOTE: in HW this is a big chunk of memory */
//	RGB_t yeq[MAX_WIDTH*MAX_HEIGHT];

	// RGB to YUV conversion of current image I(n)
	//rgb2yuv(width, height, inp_img, yuv);


	// equalize the current image I(n) and compute its histogram for next image I(n+1)
	//img_hist_equaliz1(width, height, hist, cdf, yuv, yeq);

	// YUV to RGB conversion of current image I(n)
	//yuv2rgb(width, height, yeq, out_img);



	   int i;

	   uint11  x, y;
	   uint8   Y, U, V;

	   uint8   cdf_temp;
	   uint25  cdf_mult, gain;

	   uint32 acc = 0;
	   //uint32 acc_tmp;
	   uint8  old, val;


	   value_t valTemp;


	   //from rgb2yuv()
	   uint8   R, G, B;
	    const int9	Wrgb[3][3] = {
	      { 66, 129,  25},
	      {-38, -74, 112},
	      {122, -94, -18},
	      };

	    //from yuv2rgb()
	    int9    C, D, E;
	    const int11	Wyuv[3][3] = {
	     {298,    0,  409},
	     {298, -100, -208},
	     {298,  516,    0}
	     };

	   old = 0;
	   Ly: for (y=0; y<height; y++)
	   {
		#pragma HLS LOOP_TRIPCOUNT min=480 max=1080 avg=720

		   Lx: for (x=0; x<width; x++)
		   {
#pragma HLS PIPELINE
			   #pragma HLS LOOP_TRIPCOUNT min=640 max=1920 avg=1280

				valTemp = in_strm->read();
				R = valTemp.data.range(23,16);
				G = valTemp.data.range(15, 8);
				B = valTemp.data.range( 7, 0);
				// RGB to YUV conversion of current image I(n)
				// from rgb2yuv();
				//R = inp_img[y*MAX_WIDTH +x].R;
				//G = inp_img[y*MAX_WIDTH +x].G;
				//B = inp_img[y*MAX_WIDTH +x].B;

				Y = ( ( Wrgb[0][0] * R + Wrgb[0][1] * G + Wrgb[0][2] * B + 128) >> 8) +  16;
				U = ( ( Wrgb[1][0] * R + Wrgb[1][1] * G + Wrgb[1][2] * B + 128) >> 8) + 128;
				V = ( ( Wrgb[2][0] * R + Wrgb[2][1] * G + Wrgb[2][2] * B + 128) >> 8) + 128;

	//			out_img[y*MAX_WIDTH +x].R = Y;
	//			out_img[y*MAX_WIDTH +x].G = U;
	//			out_img[y*MAX_WIDTH +x].B = V;
	//			Y = inp_img[y*MAX_WIDTH +x].R;
	//			U = inp_img[y*MAX_WIDTH +x].G;
	//			V = inp_img[y*MAX_WIDTH +x].B;

				// compute the histogram for current image I(n)
				// hist[Y] = hist[Y] + 1;

				val = Y;

				if (old==val)
					acc++;
				else
				{
					hist[old]=acc;
					acc=hist[val]+1;
				}
				old = val;

				// histogram equalization
				int numer   = ( (cdf[Y]-1) *(GRAY_LEVELS-1));
				int denomin = (height * width -1);
				cdf_mult = numer / denomin;
				//cdf_mult =(cdf[Y]-1) * gain;
				//cdf_mult = cdf_mult >> 18;

				// clipping (due to the approssimation done above)
				if (cdf_mult >= (GRAY_LEVELS-1))
				   cdf_temp = (GRAY_LEVELS-1);
				else if (cdf_mult <= 0 )
				   cdf_temp = 0;
				else
				   cdf_temp = (uint8) cdf_mult;

				  Y = cdf_temp;

	//			  out_img[y*MAX_WIDTH +x].R = Y;
	//			  out_img[y*MAX_WIDTH +x].G = U;
	//			  out_img[y*MAX_WIDTH +x].B = V;

				  //from yuv2rgb()
	//			  Y = inp_img[y*MAX_WIDTH +x].R;
	//			  U = inp_img[y*MAX_WIDTH +x].G;
	//			  V = inp_img[y*MAX_WIDTH +x].B;

				  // YUV to RGB conversion of current image I(n)
				  C = Y - 16;
				  D = U - 128;
				  E = V - 128;

				  R = CLIP(( Wyuv[0][0] * C                  + Wyuv[0][2] * E + 128) >> 8);
				  G = CLIP(( Wyuv[1][0] * C + Wyuv[1][1] * D + Wyuv[1][2] * E + 128) >> 8);
				  B = CLIP(( Wyuv[2][0] * C + Wyuv[2][1] * D                  + 128) >> 8);

				  //out_img[y*MAX_WIDTH +x].R = R;
				  //out_img[y*MAX_WIDTH +x].G = G;
				  //out_img[y*MAX_WIDTH +x].B = B;

				  valTemp.data.range(23,16) = R;
				  valTemp.data.range(15, 8) = G;
				  valTemp.data.range( 7, 0) = B;
				  if (y==height-1 && x==width-1)
					  valTemp.last = 1;

				  out_strm->write(valTemp);



			} // end of Lx loop

		} // end of Ly loop

	   	hist[old]=acc;

	   	return;
}


/* ######################################################################### */

void wrapper_img_hist_equaliz1( uint11 width, uint11 height, uint25 out_cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
					strm_t* in_strm, strm_t* out_strm)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=width
#pragma HLS INTERFACE s_axilite port=height
#pragma HLS INTERFACE s_axilite port=out_cdf
#pragma HLS INTERFACE s_axilite port=hist

//#pragma HLS INTERFACE ap_fifo port=out_img
//#pragma HLS INTERFACE ap_fifo port=inp_img
#pragma HLS INTERFACE axis register both port=in_strm
#pragma HLS INTERFACE axis register both port=out_strm

//#pragma HLS INTERFACE bram port=hist
//#pragma HLS RESOURCE variable=hist core=RAM_T2P_BRAM


	  uint19 prev_hist[GRAY_LEVELS] = {
			#include "histogram.h"
	};


	uint25 cdf[GRAY_LEVELS];

	//uint19 hist[GRAY_LEVELS];
	//#pragma HLS RESOURCE variable=hist core=RAM_T2P_BRAM // to force a True Dual Port BRAM core

	// compute the  CDF of current image I(n) from histogram of previous image I(n-1)
	compute_cdf(prev_hist, cdf);


	uint19 hist_bram[GRAY_LEVELS];
	#pragma HLS RESOURCE variable=hist_bram core=RAM_T2P_BRAM

	for (int i = 0; i < GRAY_LEVELS; i ++ ) {
		hist_bram[i] = hist[i];
	}





/*
	RGB_t inp_img[NUM_PIX];
	RGB_t out_img[NUM_PIX];

	value_t valTemp;


	int i;
	for (i = 0; i < NUM_PIX; i ++ ) {
		#pragma HLS pipeline
		valTemp = in_strm->read();
		inp_img[i].R = valTemp.data.range(23,16);
		inp_img[i].G = valTemp.data.range(15, 8);
		inp_img[i].B = valTemp.data.range( 7, 0);
	}
*/
	top_img_hist_equaliz1(width, height, cdf, hist_bram, in_strm, out_strm);

/*
	for (i = 0; i < NUM_PIX; i ++ ) {
	  #pragma HLS pipeline
	  valTemp.data.range(23,16) = out_img[i].R;
	  valTemp.data.range(15, 8) = out_img[i].G;
	  valTemp.data.range( 7, 0) = out_img[i].B;
	  out_strm->write(valTemp);
	}
*/
	// THIS function  HAS TO STAY OUT OF THIS TOP TO APPLY DATAFLOW CONCURRENCY
	// update the histogram to be used for I(n+1) 


	for (int i = 0; i < GRAY_LEVELS; i ++ ) {
		hist[i] = hist_bram[i];
	}

	update_histogram(hist, prev_hist, cdf, out_cdf);

	return;

}
