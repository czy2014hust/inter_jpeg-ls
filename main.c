/*************************************************************************\
 * JPEG-LS Lossless image encoder/decoder                                *
 * Copyright (C) 2010 Davide Bardone <davide.bardone@gmail.com>          *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
\*************************************************************************/

/* Conforming to ITU-T Reccomendation T.87 */

#include <stdio.h>
#include "type_defs.h"
#include "parameters.h"
#include "codingvars.h"
#include "pnm.h"
#include "predictivecoding.h"
#include "bitstream.h"

static inline uint32 min(uint32 a,uint32 b){ return (a > b) ? b : a; }
static inline uint32 max(uint32 a,uint32 b){ return (a > b) ? a : b; }
static inline int32 CLAMP(int32 a,int32 b,int32 c){ return (a > c || a < b) ? b : a; }

int main(int argc, char* argv[])
{
  FILE *fd;
	fd = fopen("predictor_error.txt", "w+");

  FILE *fd1,*fd2;
  fd1 = fopen("predictor1_error.txt", "w+");
  fd2 = fopen("predictor2_error.txt", "w+");


	parameters params;
	codingvars vars;


  image_data* pre_im_data = NULL;//n-1 frame image
  image_data* im_data = NULL;//n frame image

	// parsing command line parameters and/or JPEG-LS header
	params = coding_parameters(argc, argv);

		// encoding process
    //loading previous image data

  pre_im_data = load_image(params.input_file0);
  printf("%s**\n", params.input_file0);
	// loading image data
	im_data = load_image(params.input_file);
  printf("%s**\n", params.input_file);

	// bitstream initialization
	init_bitstream(params.output_file, 'w');
	params.MAXVAL = im_data->maxval;
	write_header(params, im_data);



	// setting parameters
	if(params.specified_T == false)
	{
		/* C.2.4.1.1 Default threshold values */
		if(params.MAXVAL>=128)
		{
			vars.FACTOR = floor((float64)(min(params.MAXVAL,4095)+128)/256);
			params.T1 = CLAMP(vars.FACTOR*(vars.BASIC_T1-2)+2+3*params.NEAR,params.NEAR+1,params.MAXVAL);
			params.T2 = CLAMP(vars.FACTOR*(vars.BASIC_T2-3)+3+5*params.NEAR,params.T1,params.MAXVAL);
			params.T3 = CLAMP(vars.FACTOR*(vars.BASIC_T3-4)+4+7*params.NEAR,params.T2,params.MAXVAL);
		}
		else
		{
			vars.FACTOR = floor( 256.0/(params.MAXVAL + 1) );
			params.T1 = CLAMP(max(2,floor((float64)vars.BASIC_T1/vars.FACTOR)+3*params.NEAR),params.NEAR+1,params.MAXVAL);
			params.T2 = CLAMP(max(2,floor((float64)vars.BASIC_T2/vars.FACTOR)+5*params.NEAR),params.T1,params.MAXVAL);
			params.T3 = CLAMP(max(2,floor((float64)vars.BASIC_T3/vars.FACTOR)+7*params.NEAR),params.T2,params.MAXVAL);
		}
	}
	if(params.verbose)
	{
		fprintf(stdout,	"Encoding %s...\n\n\twidth\t\t%d\n\theight\t\t%d\n\tcomponents\t%d\n",
		params.input_file, im_data->width, im_data->height, im_data->n_comp);
		fprintf(stdout, "\tMAXVAL\t\t%d\n\tNEAR\t\t%d\n\tT1\t\t%d\n\tT2\t\t%d\n\tT3\t\t%d\n\tRESET\t\t%d\n",
		params.MAXVAL, params.NEAR, params.T1, params.T2, params.T3, params.RESET);
	}


	/* A.2 Initializations and conventions */
	init_codingvars(&vars, params);

	//for(vars.comp=0; vars.comp<im_data->n_comp; vars.comp++)
	for(vars.comp=0; vars.comp<1; vars.comp++)
		for(vars.row=0; vars.row<im_data->height; vars.row++)
			for(vars.col=0; vars.col<im_data->width; vars.col++)
			{
				/* A.3 Context determination */
				context_determination(&vars, params, im_data);
        //printf("%s\n", "HERE IT IS!");
        motion_vector(&vars, params, pre_im_data, im_data);
        if((vars.row == 100) && (vars.col >= 200) && (vars.col < 220))
          printf("motion_vector is (%d,%d) and Dif_min is %d.\n", vars.m0,vars.n0,vars.temp);


        prev_context(&vars, params, pre_im_data);
				// regular mode
				/* A.4 Prediction*/
				predict_sample_value(&vars, params);

        predictor1(&vars, params);
        predictor2(&vars, params);

        encode_prediction_error(&vars, params, im_data);
        fprintf(fd, "%d\n", vars.Errval_temp3);

        encode_prediction_error_intra(&vars, params);
        fprintf(fd1, "%d\n", vars.Errval_temp1);
        fprintf(fd2, "%d\n", vars.Errval_temp2);

					/* A.6 Update variables */
					update_codingvars(&vars, params);
			}

	end_bitstream();

	if(params.decoding_flag == true)
		write_image("lena_decoded.ppm", im_data);
	else
		print_bpp(im_data);

	return 0;
}
