#include <assert.h>
#include <stdio.h>

#include "my_defines.h"
//#include "ref_img_hist.h"


/* ######################################################################### */

// update the histogram and output the CDF
void ref_update_histogram(uint19 inp_hist[GRAY_LEVELS], uint19 out_hist[GRAY_LEVELS],
		               uint25 inp_cdf[GRAY_LEVELS],  uint25 out_cdf[GRAY_LEVELS])
{
	L0: for (unsigned char i=0; i<(GRAY_LEVELS-1); i++)
	{
			  out_hist[i] = inp_hist[i];
			  //inp_hist[i] = 0; //reset histogram array for next computation
			  out_cdf[i]  = inp_cdf[i];
	}
}


/* ######################################################################### */

// computes the Cumulative Density Function cdf[] from the Histrogram hist[]
void ref_compute_cdf(uint19 hist[GRAY_LEVELS], uint25 cdf[GRAY_LEVELS])
{
	uint8 i, k;


CDF_L1:	for (k = 0; k < GRAY_LEVELS-1; k++)
	{
	    uint25 tmp_cdf=0;
		CDF_L2: for (i = 0; i<=k; i++)
		{

			// cdf for current picture from past picture histogram
			tmp_cdf = tmp_cdf + hist[i];
		}
		cdf[k] = tmp_cdf;
	}

	return;

}

/* ######################################################################### */

void ref_yuv2rgb( uint11 width, uint11 height, RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
  {

   int i;

   uint11  x, y;
   uint8   R, G, B, Y, U, V;
   int9    C, D, E;

   const int11	Wyuv[3][3] = {
    {298,    0,  409},
    {298, -100, -208},
    {298,  516,    0}
    };


	Ly: for (y=0; y<height; y++)
	{


	  Lx: for (x=0; x<width; x++)
	  {


		  Y = inp_img[y*MAX_WIDTH +x].R; // in the input image Y is stored into the R channel
		  U = inp_img[y*MAX_WIDTH +x].G;
		  V = inp_img[y*MAX_WIDTH +x].B;

		  // YUV to RGB conversion of current image I(n)
		  C = Y - 16;
		  D = U - 128;
		  E = V - 128;

		  R = CLIP(( Wyuv[0][0] * C                  + Wyuv[0][2] * E + 128) >> 8);
		  G = CLIP(( Wyuv[1][0] * C + Wyuv[1][1] * D + Wyuv[1][2] * E + 128) >> 8);
		  B = CLIP(( Wyuv[2][0] * C + Wyuv[2][1] * D                  + 128) >> 8);

		  out_img[y*MAX_WIDTH +x].R = R;
		  out_img[y*MAX_WIDTH +x].G = G;
		  out_img[y*MAX_WIDTH +x].B = B;

	  } // end of Lx loop

	} // end of Ly loop


}


/* ######################################################################### */

void ref_rgb2yuv( uint11 width, uint11 height, RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{

   int i;

   uint11  x, y;
   uint8   R, G, B, Y, U, V;

   const int9	Wrgb[3][3] = {
     { 66, 129,  25},
     {-38, -74, 112},
     {122, -94, -18},
     };


	Ly: for (y=0; y<height; y++)
	{


		Lx: for (x=0; x<width; x++)
		{

			// RGB to YUV conversion of current image I(n)
			// rgb2yuv(in, &yuv);

			R = inp_img[y*MAX_WIDTH +x].R;
			G = inp_img[y*MAX_WIDTH +x].G;
			B = inp_img[y*MAX_WIDTH +x].B;

			Y = ( ( Wrgb[0][0] * R + Wrgb[0][1] * G + Wrgb[0][2] * B + 128) >> 8) +  16;
			U = ( ( Wrgb[1][0] * R + Wrgb[1][1] * G + Wrgb[1][2] * B + 128) >> 8) + 128;
			V = ( ( Wrgb[2][0] * R + Wrgb[2][1] * G + Wrgb[2][2] * B + 128) >> 8) + 128;

			out_img[y*MAX_WIDTH +x].R = Y;
			out_img[y*MAX_WIDTH +x].G = U;
			out_img[y*MAX_WIDTH +x].B = V;

		  } // end of Lx loop

	} // end of Ly loop


}

/* ######################################################################### */

void ref_img_hist_equaliz1( uint11 width, uint11 height, uint19 hist[GRAY_LEVELS], uint25 cdf[GRAY_LEVELS],
		RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{


   int i;

   uint11  x, y;
   uint8   Y, U, V;

   uint8   cdf_temp;
   uint25  cdf_mult;

   //uint32 acc = 0;
   //uint8  old, val;

   float gain = (float) (GRAY_LEVELS-1) / ((float) (height * width -1));
   printf("ref gain = %f\n", gain);


   //old = 0;
   Ly: for (y=0; y<height; y++)
   {


	   Lx: for (x=0; x<width; x++)
	   {


			Y = inp_img[y*MAX_WIDTH +x].R;
			U = inp_img[y*MAX_WIDTH +x].G;
			V = inp_img[y*MAX_WIDTH +x].B;

			// compute the histogram for current image I(n)
			hist[Y] = hist[Y] + 1;

//			val = Y;
//
//			if (old==val)
//				acc++;
//			else
//			{
//				hist[old]=acc;
//				acc=hist[val]+1;
//			}
//			old = val;

			// histogram equalization
		    //float fp_mult = (float) (cdf[Y]-1) * gain;
			cdf_mult = ( (cdf[Y]-1) *(GRAY_LEVELS-1)) / (height * width -1);

			// clipping (due to the approssimation done above)
			if (cdf_mult >= (GRAY_LEVELS-1))
			   cdf_temp = (GRAY_LEVELS-1);
			else if (cdf_mult <= 0 )
			   cdf_temp = 0;
			else
			   cdf_temp = (uint8) cdf_mult;

			Y = cdf_temp;


			  out_img[y*MAX_WIDTH +x].R = Y;
			  out_img[y*MAX_WIDTH +x].G = U;
			  out_img[y*MAX_WIDTH +x].B = V;

		} // end of Lx loop

	} // end of Ly loop

//   	hist[old]=acc;



}


/* ######################################################################### */

void ref_img_hist_equaliz2( uint11 width, uint11 height, uint25 cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
							RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{
	RGB_t yuv[MAX_WIDTH*MAX_HEIGHT];    /* NOTE: in HW this is a big chunk of memory */
	RGB_t yeq[MAX_WIDTH*MAX_HEIGHT];

	// RGB to YUV conversion of current image I(n)
	ref_rgb2yuv(width, height, inp_img, yuv);

	// equalize the current image I(n) and compute its histogram for next image I(n+1)
	ref_img_hist_equaliz1(width, height, hist, cdf, yuv, yeq);

	// YUV to RGB conversion of current image I(n)
	ref_yuv2rgb(width, height, yeq, out_img);

}


/* ######################################################################### */

void ref_histogram_equaliz3( uint11 width, uint11 height, uint25 out_cdf[GRAY_LEVELS], uint19 hist[GRAY_LEVELS],
					RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{


	  uint19 prev_hist[GRAY_LEVELS] = {
			#include "histogram.h"
	};


	uint25 cdf[GRAY_LEVELS];

	//uint19 hist[GRAY_LEVELS];
	//#pragma HLS RESOURCE variable=hist core=RAM_T2P_BRAM // to force a True Dual Port BRAM core

	// compute the  CDF of current image I(n) from histogram of previous image I(n-1)
	ref_compute_cdf(prev_hist, cdf);

	ref_img_hist_equaliz2(width, height, cdf, hist, inp_img, out_img);

	// THIS function  HAS TO STAY OUT OF THIS TOP TO APPLY DATAFLOW CONCURRENCY
	// update the histogram to be used for I(n+1)
	ref_update_histogram(hist, prev_hist, cdf, out_cdf);

	return;

}
