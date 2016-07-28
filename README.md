***clone from https://github.com/davidebardone/jpeg-ls.git***
# JPEG-LS(帧间) C语言实现代码模块
***
## 1.读取图片文件-->pnm.c
> **函数**

>> void free_image_mem(uint16*** image, uint16 height, uint8 n_of_components)

>> void free_image_data(image_data* im_data)

>> image_data* load_image(char* image_name)

>> image_data* allocate_image_data()

>> void allocate_image(image_data* im_data)

>> void write_image(char* image_name, image_data* im_data)

> **结构体**(pnm.h)

>> <pre><code>struct image_data{
        uint16*** 	image;
        uint16 		width;
        uint16 		height;
        uint16 		maxval;
        uint8 		n_comp;
} typedef image_data;</code></pre>



## 2.参数-->parameters.c
> **函数**

>> void usage(int exit_type)-->命令行帮助

>> parameters coding_parameters(int argc, char* argv[])-->命令行参数解析

> **结构体**

>> <pre><code>
struct parameters{
	char input_file0[MAX_FILENAME_LEN];
	char input_file[MAX_FILENAME_LEN];	// input filename
	char output_file[MAX_FILENAME_LEN];	// output filename
	bool decoding_flag;			// encoding/decoding flag
	bool verbose;				// verbose flag
	uint8 NEAR;				// difference bound for near lossless coding
	uint16 MAXVAL;				// max image sample value
	uint16 T1, T2, T3;			// thresholds for local gradients
	bool specified_T;
	uint16 RESET;				// threshold value at which A,B, and N are halved
	uint8 ILV;				// interleave
} typedef parameters;</code>
</pre>

## 3.变量-->codingvars.c
> **函数**

>> void init_codingvars(codingvars* vars, parameters params)

>> void update_codingvars(codingvars* vars, parameters params)

> **结构体**

>>  <pre><code>struct codingvars{
	bool RunModeProcessing;		// Regular/Run mode processing flag
	uint16 RANGE;			// range of prediction error representation
	uint8 bpp;			// number of bits needed to represent MAXVAL
	uint8 qbpp;			// number of bits needed to represent a mapped error value
	uint8 LIMIT;			// max length in bits of Golomb codewords in regular mode
	int8 mv;
	int8 nv;//search fanwei
	int8 m0;
	int8 n0;//motion vector
	uint16 comp, row, col;
	int32 N[CONTEXTS + 2];		// occurrences counters for each context
	uint32 A[CONTEXTS + 2];		// prediction error magnitudes accumulator
	int32 B[CONTEXTS];		// bias values
	int32 C[CONTEXTS];		// prediction correction values
	uint8 RUNindex	;		// index for run mode order
	uint8 RUNindex_val;
	uint16 RUNval;			// repetitive reconstructed sample value in a run
	uint16 RUNcnt;			// repetetive sample count for run mode
	uint32 TEMP;			// auxiliary variable for Golomb variable calculation in run interruption coding
	uint8 map;			// auxiliary variable for error mapping at run interruption
	uint8 RItype;			// index for run interruption coding
	uint8* J;			// order of run-length codes
	int32 Nn[2];			// counters for negative prediction error for run interruption
	uint16 Ra, Rb, Rc, Rd, prevRa, Ix;	// pixels used in the causal template
	uint16 Ram0,Rbm0,Rcm0,Rdm0,prevRam0,Ixm0;
	int32 Rx;				// reconstructed value of the current sample
	int32 Px;				// predicted value for the current sample
  int32 PxNO;
	int32 Px1;
	int32 Px2;
	int32 Px3;//thress predictors
	int32 Errval;				// prediction error
	int32 temp;
	int32 Errval_temp1;//test
	int32 Errval_temp2;//test
	int32 Errval_temp3;//test
	uint32 MErrval;				// Errval mapped to a non-negative integer
	uint32 EMErrval;			// Errval mapped to non-negative integers in run interruption mode
	uint16 Q;				// context
	int8 SIGN;				// sign of the current context
	uint8 k;				// Golomb coding variable
	int32 MAX_C;				// maximum allowed value for C[0..364]
	int32 MIN_C;				// minimum allowed value for C[0..364]
	uint16 BASIC_T1;			// basic default values for gradient quantization thresholds
	uint16 BASIC_T2;
	uint16 BASIC_T3;
	uint16 FACTOR;
} typedef codingvars;</code></pre>

## 4.预测编码模块-->predictivecoding.c
> **函数**

>> void prev_context(codingvars* vars, parameters params, image_data* pre_im_data)

>> void context_determination(codingvars* vars, parameters params, image_data* im_data)

>> void predict_sample_value(codingvars* vars, parameters params)

>> void predictor1(codingvars* vars, parameters params)

>> void predictor2(codingvars* vars, parameters params)

>> void motion_vector(codingvars* vars, parameters params, image_data* pre_im_data, image_data* im_data)

>> void encode_prediction_error_intra(codingvars* vars, parameters params)

>> void encode_prediction_error(codingvars* vars, parameters params, image_data* im_data)

>>> limited_length_Golomb_encode((*vars).MErrval, (*vars).k, (*vars).LIMIT, (*vars).qbpp)

>> void decode_prediction_error(codingvars* vars, parameters params, image_data* im_data)

## 5.哥伦布编码-->golomb.c
> **函数**

>>void limited_length_Golomb_encode(uint32 MErrval, uint8 k, uint8 LIMIT, uint8 qbpp)

>> uint32 limited_length_Golomb_decode(uint8 k, uint8 LIMIT, uint8 qbpp)

## 6.码流整合-->bitstream.c
> **函数**

>> void init_bitstream(char* filename, char mode)

>>void end_bitstream()

>> void print_bpp(image_data* im_data)

>>void append_bit(uint8 bit)


>> void append_bits(uint32 value, uint8 bits)

>> void write_header(parameters params, image_data* im_data)

>> image_data* read_header(parameters* params)

> **结构体**

>><pre><code>
struct bitstream{
	FILE* bitstream_file;
	uint8 byte_bits;
	uint8 buffer;
	uint32 tot_bits;
} typedef bitstream
</code></pre>
