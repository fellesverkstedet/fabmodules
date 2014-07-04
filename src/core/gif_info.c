//
// gif_info.c
//    report .gif info
//
// Neil Gershenfeld 3/24/14
// (c) Massachusetts Institute of Technology 2013
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge the fab modules project. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#define MAX_LINE 10000

#include "fab.h"

int main(int argc, char **argv) {
   //
   // local vars
   //
   GifFileType *GIFfile;
   GifRecordType GIFtype;
   GifByteType *GIFextension;
   GifPixelType *GIFline;
   int x,y,i,n,imin,imax;
   int image_width,image_height,image_count,color_resolution,GIFcode,ret;
   float voxel_size;
   char comment[256];
   struct fab_vars v;
   init_vars(&v);
   //
   // command line args
   //
   if (!(argc == 2)) {
      printf("command line: gif_info in.gif\n");
      printf("   in.gif = input GIF file\n");
      exit(-1);
      }
   voxel_size = -1;
   image_width = -1;
   image_height = -1;
   image_count = -1;
   //
   // scan the file 
   //
   printf("read %s\n",argv[1]);
   color_resolution = -1;
   GIFfile = DGifOpenFileName(argv[1]);
   if (GIFfile == NULL) {
      printf("gif_info: oops -- can not open %s\n",argv[1]);
      exit(-1);
      }
   GIFline = malloc(MAX_LINE*sizeof(GifPixelType));
   imin = 256;
   imax = 0;
   do {
      DGifGetRecordType(GIFfile,&GIFtype);
      switch (GIFtype) {
         case IMAGE_DESC_RECORD_TYPE:
            DGifGetImageDesc(GIFfile);
            image_width = GIFfile->SWidth;
            image_height = GIFfile->SHeight;
            image_count = GIFfile->ImageCount;
            color_resolution = GIFfile->SColorResolution;
            for (y = 0; y < GIFfile->SHeight; ++y) {
               ret = DGifGetLine(GIFfile,GIFline,GIFfile->SWidth);
               if (ret != GIF_OK) {
                  printf("gif_info: oops -- error reading line\n");
                  exit(-1);
                  }
               for (x = 0; x < GIFfile->SWidth; ++x) {
                  if (GIFline[x] < imin) imin = GIFline[x];
                  if (GIFline[x] > imax) imax = GIFline[x];
                  }
               }
            break;
         case EXTENSION_RECORD_TYPE:
            DGifGetExtension(GIFfile,&GIFcode,&GIFextension);
            if (GIFcode == COMMENT_EXT_FUNC_CODE) {
               n = GIFextension[0];
               for (i = 1; i <= n; ++i)
                  comment[i-1] = GIFextension[i];
               comment[n] = 0;
               if (voxel_size == -1)
                  sscanf(comment,"mm per pixel: %f;",&voxel_size);
               }
            while (GIFextension != NULL)
               DGifGetExtensionNext(GIFfile,&GIFextension);
            break;
         case SCREEN_DESC_RECORD_TYPE:
            DGifGetScreenDesc(GIFfile);
            break;
         case TERMINATE_RECORD_TYPE:
            break;
         case UNDEFINED_RECORD_TYPE:
            printf("gif_info: oops -- undefined GIF record type\n");
            exit(-1);
            break;
         }
      } while (GIFtype != TERMINATE_RECORD_TYPE);
   if (GIFfile == NULL) {
      printf("gif_info: oops -- can not open %s\n",argv[1]);
      exit(-1);
      }
   if (voxel_size == -1) {
      voxel_size = 1.0;
      printf("   no pixel size found, assuming 1 mm\n");
      }
   printf("   voxel size (mm): %f, color resolution (bits): %d\n",voxel_size,color_resolution);
   printf("   intensity min: %d max: %d\n",imin,imax);
   printf("   number of images: %d, image width %d, image height %d\n",image_count,image_width,image_height);
   //
   // exit
   //
   DGifCloseFile(GIFfile);
   exit(0);
   }
