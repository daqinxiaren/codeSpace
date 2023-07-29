#include "fft_max.h"
#include "math.h"

void fft_max(float *buff,float begin_fre,float end_fre,float fre_resolution,float *max_val,float *fft_val)
{
    int begin_index = 0;
    int end_index = 0;
    int cnt_i = 0;
    float max_value = 0.0;

    if(begin_fre<=0)
    {
        begin_fre=1;
    }
    begin_index = (int)(round((double)begin_fre/(double)fre_resolution));
    end_index =(int)(round((double)end_fre/(double)fre_resolution));

    for(cnt_i = begin_index;cnt_i <= end_index;cnt_i ++)
    {
        if(buff[cnt_i] > max_value)
        {
            max_value = buff[cnt_i];
            *max_val = max_value;
            *fft_val = (float) cnt_i*fre_resolution;
        }
    }
}
