/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#include "png.h"

#include "if.h"
#include "ipng.h"
#define OK 1
#define MINIMUM_DELAY_TIME 10




void read_png(il_container *ic, IL_Stream *stream, unsigned int sig_read);

void png_get_info(const unsigned char *buf, png_structp png_ptr, png_infop info_ptr);
void  png_read_strm( png_structp png_ptr, png_bytep data, png_size_t length);

extern void il_emit_row(il_container *ic, uint8 *buf, uint8 *rgbbuf,
                        int start_column, int len, int row, int row_count,
                        il_draw_mode draw_mode, int ipass);

void row_callback( png_structp png_ptr, png_bytep new_row,
   png_uint_32 row_num, int pass /*, il_container *ic*/);

extern void end_callback(png_structp png_ptr, png_infop info);
extern void info_callback(png_structp png_ptr, png_infop info);

extern void il_create_alpha_mask( il_container *, int, int, int ); 


extern png_structp png_create_read_struct(png_charp user_png_ver, png_voidp, png_error_ptr, png_error_ptr);
extern png_infop png_create_info_struct(png_structp png_ptr);

extern void png_destroy_read_struct(png_structpp, png_infopp, png_infopp);
extern void png_set_progressive_read_fn(png_structp, png_voidp, png_progressive_info_ptr, png_progressive_row_ptr, png_progressive_end_ptr);
extern void png_process_data(png_structp, png_infop, png_bytep, png_size_t);


/*-----------------------------*/
int
il_png_init(il_container *ic)
{

	png_structp read_ptr, write_ptr;
    png_infop read_info_ptr, write_info_ptr, end_info_ptr;

	ipng_struct *ipngs;
    NI_ColorSpace *src_color_space = ic->src_header->color_space;

	ipngs = XP_NEW_ZAP(ipng_struct);
	if (ipngs) 
	{
		ic->ds = ipngs;
		ipngs->state = PNG_INIT;
		ipngs->ic = ic;
	}

    /* Initialize the container's source image header. */
	/* Always decode to 24 bit pixdepth */

    src_color_space->type = NI_TrueColor;
    src_color_space->pixmap_depth = 24;
    src_color_space->bit_alloc.index_depth = 0;
	
 
       
	return (ipngs != 0);

}

/* Gather n characters from the input stream and then enter state s. */

int il_png_write(il_container *ic, const unsigned char *buf, int32 len)
{
   ipng_structp ipng_ptr;

   png_structp png_ptr;
   png_infop info_ptr;
  
   int status, done;
  
	/*------*/
   

	ipng_ptr = (ipng_structp)ic->ds;   
    if(ipng_ptr->state == PNG_INIT ){
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
    	ipng_ptr->pngs_p = png_ptr;
        /* Allocate/initialize the memory for image information.  REQUIRED. */
	    info_ptr = png_create_info_struct(png_ptr);
        ipng_ptr->info_p = info_ptr;
	    if (info_ptr == NULL){
		      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		      return !OK;
	    }
        png_set_progressive_read_fn(png_ptr, buf,
        info_callback, row_callback, end_callback); 
    }
	else{
        png_ptr = ipng_ptr->pngs_p;
        info_ptr = ipng_ptr->info_p;
    }
    /* note addition of ic to png structure.... */
    png_ptr->io_ptr = ic;
    png_process_data( png_ptr, info_ptr, buf, len );
        
    ipng_ptr->state = PNG_CONTINUE;
   
    return OK;
}

void
png_set_dims( il_container *ic, png_structp png_ptr)
{    
    int status;

    NI_PixmapHeader *img_hdr = &ic->image->header;                  
    NI_PixmapHeader *src_hdr = ic->src_header;

    src_hdr->width = img_hdr->width = png_ptr->width;
    src_hdr->height = img_hdr->height = png_ptr->height;

    if((png_ptr->channels > 3 )||(png_ptr->trans)){
        il_create_alpha_mask( png_ptr->io_ptr, 0, png_ptr->width, png_ptr->height);
        ic->image->header.is_interleaved_alpha = TRUE;
        il_init_image_transparent_pixel(ic);

    }

    status = il_size(ic);

    /*Note: all png's are decoded to RGB or RGBa and
    converted by imglib to appropriate pixdepth*/

    il_setup_color_space_converter(ic);
    
    return;
}

il_png_init_transparency(png_structp png_ptr, il_container *ic, int index)
{
    IL_IRGB *src_trans_pixel /*= ic->src_header->transparent_pixel*/;
    IL_IRGB *img_trans_pixel;
    
    if (ic->src_header) {
        ic->src_header->transparent_pixel = XP_NEW_ZAP(IL_IRGB);
    }
    src_trans_pixel = ic->src_header->transparent_pixel;

        /* Initialize the destination image's transparent pixel. */
    il_init_image_transparent_pixel(ic);

        /* Set the source image's transparent pixel color to be the preferred
           transparency color of the destination image. */
        img_trans_pixel = ic->image->header.transparent_pixel;

        src_trans_pixel->red = png_ptr->trans_values.red;
        src_trans_pixel->green = png_ptr->trans_values.green;
        src_trans_pixel->blue = png_ptr->trans_values.blue;
  
/* 
        src_trans_pixel->red = img_trans_pixel->red;
        src_trans_pixel->green = img_trans_pixel->green;
        src_trans_pixel->blue = img_trans_pixel->blue;
  */
        
    /* Set the source image's transparent pixel index.  Do this even if the source
       image's transparent pixel has previously been set, since the index can vary
       from frame to frame in an animated gif. */
    src_trans_pixel->index = index;

    return TRUE;
}


static void
il_png_destroy_transparency(il_container *ic)
{
    NI_PixmapHeader *src_header = ic->src_header;
    
    if (src_header->transparent_pixel) {
        /* Destroy the source image's transparent pixel. */
        XP_FREE(src_header->transparent_pixel);
        src_header->transparent_pixel = NULL;

        /* Destroy the destination image's transparent pixel. */
        il_destroy_image_transparent_pixel(ic);
    }
}


static void
png_delay_time_callback(void *closure)
{
	ipng_struct *ipng_ptr = (ipng_struct *)closure;

    XP_ASSERT(ipng_ptr->state == PNG_DELAY);

    ipng_ptr->delay_time = NULL;

    if (ipng_ptr->ic->state == IC_ABORT_PENDING)
        return;                                        

    ipng_ptr->delay_time = 0;         /* Reset for next image */

    
}

void il_png_complete(il_container *ic)
{
	ipng_structp ipng_ptr;

	ipng_ptr = ic->ds;

	il_png_abort(ic);
   
	/* notify observers that the current frame has completed. */
 
    il_frame_complete_notify(ic);                

    /* An image can specify a delay time before which to display
       subsequent images.  Block until the appointed time. */
	if(ipng_ptr->delay_time < MINIMUM_DELAY_TIME )
		ipng_ptr->delay_time = MINIMUM_DELAY_TIME ;
	if (ipng_ptr->delay_time){
			ipng_ptr->delay_timeout =
			FE_SetTimeout(png_delay_time_callback, ipng_ptr, ipng_ptr->delay_time);

			/* Essentially, tell the decoder state machine to wait
			forever.  The delay_time callback routine will wake up the
			state machine and force it to decode the next image. */
			ipng_ptr->state = PNG_DELAY;
     } else {
		    ipng_ptr->state = PNG_INIT;
     }
 
	return;
}

void il_png_abort(il_container *ic)
{
/*    il_abort( ic ); */
	return;
}

