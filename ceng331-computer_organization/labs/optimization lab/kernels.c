/********************
 * Kernels to be optimized for the Metu Ceng Performance Lab
 ********************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "dim96",                     /* Team name */

    "Şeyma Alan",             /* First member full name */
    "e2380053",                 /* First member id */

    "Rabia Kırer",                         /* Second member full name (leave blank if none) */
    "e2468239",                         /* Second member id (leave blank if none) */

    "Derya Tınmaz",                         /* Third member full name (leave blank if none) */
    "e2380947"                          /* Third member id (leave blank if none) */
};

/******
 * EXPOSURE FUSION KERNEL *
 ******/

/*******************
 * Your different versions of the exposure fusion kernel go here
 *******************/

/*
 * naive_fusion - The naive baseline version of exposure fusion
 */
char naive_fusion_descr[] = "naive_fusion: Naive baseline exposure fusion";
void naive_fusion(int dim, int *img, int *w, int *dst) {

    int i, j, k;
    for(k = 0; k < 4; k++){
        for(i = 0; i < dim; i++) {
            for(j = 0; j < dim; j++) {
                    dst[i*dim+j] += w[k*dim*dim+i*dim+j] * img[k*dim*dim+i*dim+j];
            }
        }
    }
}

/*
 * fusion - Your current working version of fusion
 * IMPORTANT: This is the version you will be graded on
 */
char fusion_descr[] = "fusion: Current working version";
void fusion(int dim, int *img, int *w, int *dst)
{
  int i,k,it;
  it=dim*dim;
  int *dst_p = dst;
  for(k=0;k<4;k++){
    dst_p=dst;
    for(i=0; i<it;i+=32){
      dst_p[0] +=w[0]*img[0];
      dst_p[1] +=w[1]*img[1];
      dst_p[2] +=w[2]*img[2];
      dst_p[3] +=w[3]*img[3];
      dst_p[4] +=w[4]*img[4];
      dst_p[5] +=w[5]*img[5];
      dst_p[6] +=w[6]*img[6];
      dst_p[7] +=w[7]*img[7];
      dst_p[8] +=w[8]*img[8];
      dst_p[9] +=w[9]*img[9];
      dst_p[10] +=w[10]*img[10];
      dst_p[11] +=w[11]*img[11];
      dst_p[12] +=w[12]*img[12];
      dst_p[13] +=w[13]*img[13];
      dst_p[14] +=w[14]*img[14];
      dst_p[15] +=w[15]*img[15];
      dst_p[16] +=w[16]*img[16];
      dst_p[17] +=w[17]*img[17];
      dst_p[18] +=w[18]*img[18];
      dst_p[19] +=w[19]*img[19];
      dst_p[20] +=w[20]*img[20];
      dst_p[21] +=w[21]*img[21];
      dst_p[22] +=w[22]*img[22];
      dst_p[23] +=w[23]*img[23];
      dst_p[24] +=w[24]*img[24];
      dst_p[25] +=w[25]*img[25];
      dst_p[26] +=w[26]*img[26];
      dst_p[27] +=w[27]*img[27];
      dst_p[28] +=w[28]*img[28];
      dst_p[29] +=w[29]*img[29];
      dst_p[30] +=w[30]*img[30];
      dst_p[31] +=w[31]*img[31];

      dst_p+=32;
      w+=32;
      img+=32;

    }
  }
}

/***********************
 * register_fusion_functions - Register all of your different versions
 *     of the fusion kernel with the driver by calling the
 *     add_fusion_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 ***********************/

void register_fusion_functions()
{
    add_fusion_function(&naive_fusion, naive_fusion_descr);
    add_fusion_function(&fusion, fusion_descr);
    /* ... Register additional test functions here */
}

/*********
 * GAUSSIAN BLUR KERNEL *
 *********/

/******************
 * Your different versions of the Gaussian blur functions go here
 ******************/

/*
 * naive_blur - The naive baseline version of Gussian blur
 */
char naive_blur_descr[] = "naive_blur The naive baseline version of Gaussian blur";
void naive_blur(int dim, float *img, float *flt, float *dst) {

    int i, j, k, l;

    for(i = 0; i < dim-5+1; i++){
        for(j = 0; j < dim-5+1; j++) {
            for(k = 0; k < 5; k++){
                for(l = 0; l < 5; l++) {
                    dst[i*dim+j] = dst[i*dim+j] + img[(i+k)*dim+j+l] * flt[k*dim+l];
                }
            }
        }
    }
}

/*
 * blur - Your current working version of Gaussian blur
 * IMPORTANT: This is the version you will be graded on
 */
char blur_descr[] = "blur: Current working version";
void blur(int dim, float *img, float *flt, float *dst)
{

  int i,j;
    float res;
    float *arr1, *arr2, *arr3;

    float flt0  = flt[0];
    float flt1  = flt[1];
    float flt2  = flt[2];
    float flt3  = flt[3];
    float flt4  = flt[4];

    flt += dim;
    float flt5  = flt[0];
    float flt6  = flt[1];
    float flt7 = flt[2];
    float flt8 = flt[3];
    float flt9 = flt[4];

    flt += dim;
    float flt10 = flt[0];
    float flt11 = flt[1];
    float flt12 = flt[2];
    float flt13 = flt[3];
    float flt14 = flt[4];

    flt += dim;
    float flt15 = flt[0];
    float flt16 = flt[1];
    float flt17 = flt[2];
    float flt18 = flt[3];
    float flt19 = flt[4];

    flt += dim;
    float flt20 = flt[0];
    float flt21 = flt[1];
    float flt22 = flt[2];
    float flt23 = flt[3];
    float flt24 = flt[4];
    
     for(i =  dim - 4; i > 0 ; --i) 
    {
        arr1 = img; 
        arr2 = dst;
        for(j =  dim - 4; j > 0 ; --j) 
        {
            arr3 = arr1;
            res = flt0 * arr3[0];
            res += (flt1 * arr3[1]) + (flt2 * arr3[2]) +
                   (flt3 * arr3[3]) + (flt4 * arr3[4]);
            arr3 += dim;
            res += (flt5 * arr3[0]) + (flt6 * arr3[1]) + (flt7 * arr3[2]) +
                   (flt8 * arr3[3]) + (flt9 * arr3[4]);
            arr3 += dim;
            res += (flt10 * arr3[0]) + (flt11 * arr3[1]) + (flt12 * arr3[2]) +
                   (flt13 * arr3[3]) + (flt14 * arr3[4]);
            arr3 += dim;
            res += (flt15 * arr3[0]) + (flt16 * arr3[1]) + (flt17 * arr3[2]) +
                   (flt18 * arr3[3]) + (flt19 * arr3[4]);
            arr3 += dim;
            res += (flt20 * arr3[0]) + (flt21 * arr3[1]) + (flt22 * arr3[2]) +
                   (flt23 * arr3[3]) + (flt24 * arr3[4]);
            arr3 += dim;
            ++arr1;
            *arr2++ = res;
        }
        img += dim;
        dst += dim;
    }


}

/***********************
 * register_blur_functions - Register all of your different versions
 *     of the gaussian blur kernel with the driver by calling the
 *     add_blur_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 ***********************/

void register_blur_functions()
{
    add_blur_function(&naive_blur, naive_blur_descr);
    add_blur_function(&blur, blur_descr);
    /* ... Register additional test functions here */
}
