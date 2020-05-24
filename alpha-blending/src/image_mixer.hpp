//
//  image_mixer.hpp
//  alpha-blending
//
//  Created by Alexey Kudrinsky on 03.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//
//  For info about commands visit https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=5652,182,3853,3853,1606,5152,3967,5152,4912,3967,4905,5705,3967,1289,1606,4115,3853,956,5015,3967&techs=SSE,SSE2,SSE3,SSSE3,SSE4_1,SSE4_2,AVX&text=_mm_mulhi_epu16
//

#ifndef image_mixer_hpp
#define image_mixer_hpp

#include "/Users/alexeykudrinsky/.my_lib/text.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <smmintrin.h>

int Global_effect = 0;

struct bmp_format {
    const char* pixels = nullptr;
    int width = 0;
    int height = 0;
};

int hex_to_dec (const char* beginning, int len) {
    int result = 0;
    
    for (int i = len - 1; i >= 0; --i) {
        result = result * 256 + (unsigned char) beginning[i];
    }
    
    return result;
}

bmp_format get_bmp_structure (const char* file) {
    if (file[0] != 'B' or file[1] != 'M')
        throw "not_bmp_format";
    
    bmp_format data = {};
    
    const int to_pixels = 10;
    const int to_width  = 18;
    const int to_height = 22;
    
    const int operand_size = 4;
    
    data.pixels = file + hex_to_dec (file + to_pixels, operand_size);
    
    data.width  = hex_to_dec (file + to_width, operand_size);
    data.height = hex_to_dec (file + to_height, operand_size);
    
    return data;
}

void mix_bmp (const char* base_image, const char* top_image, const char* result_image,
              int x_coord = 0, int y_coord = 0) {
    
    const char* base_data = read_text (base_image);
    const char* top_data  = read_text (top_image);
    
    const int pixel_size = 4;
    
    bmp_format base = {};
    bmp_format top  = {};
    
    try {
        base = get_bmp_structure (base_data);
        base.pixels += pixel_size * (y_coord * base.width + x_coord);
        
        top  = get_bmp_structure (top_data);
    }
    catch (const char* err) {
        free ((void*) base_data);
        free ((void*) top_data);
        
        printf ("%s\n", err);
        
        return;
    }
    
    static const __m128i get_alpha_low  = _mm_set_epi8 (0xFF,  7, 0xFF,  7,
                                                        0xFF,  7, 0xFF,  7,
                                                        0xFF,  3, 0xFF,  3,
                                                        0xFF,  3, 0xFF,  3);
    
    static const __m128i get_alpha_high = _mm_set_epi8 (0xFF, 15, 0xFF, 15,
                                                        0xFF, 15, 0xFF, 15,
                                                        0xFF, 11, 0xFF, 11,
                                                        0xFF, 11, 0xFF, 11);
    
    static const __m128i ones           = _mm_set_epi8 (0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF);

    for (int y = 0; y < top.height; ++y) {
        for (int x = 0; x < top.width; x += 4) {
            __m128i front_pixel = _mm_loadu_si128 (reinterpret_cast<const __m128i*> (top.pixels  + pixel_size * x));
            __m128i back_pixel  = _mm_loadu_si128 (reinterpret_cast<const __m128i*> (base.pixels + pixel_size * x));
            
            __m128i back_top_two = _mm_movehl_ps (back_pixel, back_pixel); // неявное приведение типов
            
            __m128i back_bot_two_16 = _mm_cvtepu8_epi16 (back_pixel);
            __m128i back_top_two_16 = _mm_cvtepu8_epi16 (back_top_two);
            
            __m128i front_alpha_low  = _mm_shuffle_epi8 (front_pixel, get_alpha_low);
            __m128i front_alpha_high = _mm_shuffle_epi8 (front_pixel, get_alpha_high);
            
            __m128i back_coefs_low  = _mm_sub_epi8 (ones, front_alpha_low);
            __m128i back_coefs_high = _mm_sub_epi8 (ones, front_alpha_high);
            
            __m128i back_new_low  = _mm_mulhi_epu16  (back_bot_two_16, back_coefs_low);
            __m128i back_new_high = _mm_mulhi_epu16  (back_top_two_16, back_coefs_high);
            
            __m128i back_new = _mm_packus_epi16 (back_new_low, back_new_high);
            
            back_new = _mm_adds_epi8 (front_pixel, back_new);
    
            _mm_storeu_si128 ((__m128i*) (base.pixels + pixel_size * x), back_new);
        }
        top.pixels  += pixel_size * top.width;
        base.pixels += pixel_size * base.width;
    }
    
    FILE* result = fopen (result_image, "wb");
    fwrite (base_data, sizeof (char), count_symbols (base_image), result);
    fclose (result);
    
    free ((void*) base_data);
    free ((void*) top_data);
}

void cycle_sse (bmp_format top, bmp_format base) {
    static const int pixel_size = 4;
    static const __m128i get_alpha_low  = _mm_set_epi8 (0xFF,  7, 0xFF,  7,
                                                        0xFF,  7, 0xFF,  7,
                                                        0xFF,  3, 0xFF,  3,
                                                        0xFF,  3, 0xFF,  3);
    
    static const __m128i get_alpha_high = _mm_set_epi8 (0xFF, 15, 0xFF, 15,
                                                        0xFF, 15, 0xFF, 15,
                                                        0xFF, 11, 0xFF, 11,
                                                        0xFF, 11, 0xFF, 11);
    
    static const __m128i ones           = _mm_set_epi8 (0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF,
                                                        0xFF, 0xFF, 0xFF, 0xFF);
    for (int y = 0; y < top.height; ++y) {
        for (int x = 0; x < top.width; x += 4) {
            __m128i front_pixel = _mm_loadu_si128 (reinterpret_cast<const __m128i*> (top.pixels  + pixel_size * x));
            __m128i back_pixel  = _mm_loadu_si128 (reinterpret_cast<const __m128i*> (base.pixels + pixel_size * x));
            
            __m128i back_top_two = _mm_movehl_ps (back_pixel, back_pixel); // неявное приведение типов
            
            __m128i back_bot_two_16 = _mm_cvtepu8_epi16 (back_pixel);
            __m128i back_top_two_16 = _mm_cvtepu8_epi16 (back_top_two);
            
            __m128i front_alpha_low  = _mm_shuffle_epi8 (front_pixel, get_alpha_low);
            __m128i front_alpha_high = _mm_shuffle_epi8 (front_pixel, get_alpha_high);
            
            __m128i back_coefs_low  = _mm_sub_epi8 (ones, front_alpha_low);
            __m128i back_coefs_high = _mm_sub_epi8 (ones, front_alpha_high);
            
            __m128i back_new_low  = _mm_mulhi_epu16  (back_bot_two_16, back_coefs_low);
            __m128i back_new_high = _mm_mulhi_epu16  (back_top_two_16, back_coefs_high);
            
            __m128i back_new = _mm_packus_epi16 (back_new_low, back_new_high);
            
            back_new = _mm_adds_epi8 (front_pixel, back_new);
            
            _mm_storeu_si128 ((__m128i*) (base.pixels + pixel_size * x), back_new);
        }
        top.pixels  += pixel_size * top.width;
        base.pixels += pixel_size * base.width;
        ++Global_effect;
    }
}

void cycle_ordinary (bmp_format top, bmp_format base) __attribute__ ((noinline)) {
    static const int pixel_size = 4;
    
    for (int y = 0; y < top.height; ++y) {
        for (int x = 0; x < top.width; ++x) {
            //printf ("%d * %d\n", x, y);
            char top_pixel[4]  = {};
            char back_pixel[4] = {};
            
            memcpy (top_pixel,  top.pixels  + pixel_size * x, 4);
            memcpy (back_pixel, base.pixels + pixel_size * x, 4);
            
            for (int color = 0; color < 3; ++color) {
                back_pixel[color] = top_pixel[3] * top_pixel[color]
                + (0xFF - top_pixel[3]) * back_pixel[color];
            }
            
            memcpy ((void*) (base.pixels + pixel_size * x), back_pixel, 4);
        }
        top.pixels  += pixel_size * top.width;
        base.pixels += pixel_size * base.width;
        ++Global_effect;
    }
}

void compare_cycles (const char* base_image, const char* top_image, const char* result_image,
                int x_coord = 0, int y_coord = 0) {
    const char* base_data = read_text (base_image);
    const char* top_data  = read_text (top_image);
    
    static const int pixel_size = 4;
    bmp_format base = {};
    bmp_format top  = {};
    
    try {
        base = get_bmp_structure (base_data);
        base.pixels += pixel_size * (y_coord * base.width + x_coord);
        
        top  = get_bmp_structure (top_data);
    }
    catch (const char* err) {
        free ((void*) base_data);
        free ((void*) top_data);
        
        printf ("%s\n", err);
        return;
    }
    
    for (int i = 0; i < 200000; ++i) {
        cycle_sse (top, base);
        cycle_ordinary (top, base);
    }
    printf ("%d\n", Global_effect);
    
    free ((void*) base_data);
    free ((void*) top_data);
}

#endif /* image_mixer_hpp */
