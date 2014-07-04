//
// path_camm.c
//    convert path to Roland vinylcutter .camm
//
// Neil Gershenfeld
// CBA MIT 9/1/10
//
// (c) Massachusetts Institute of Technology 2010
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//

#include "fab.h"

void fab_write_camm(struct fab_vars *v, char *output_file_name, float force, float velocity, float ox, float oy, char loc) {
   //
   // write path to Roland vinylcutter file
   //
	FILE *output_file;
   int x,y,nsegs=0,npts=0;
   float scale,xoffset,yoffset;
   output_file = fopen(output_file_name,"w");
   fprintf(output_file,"PA;PA;!ST1;!FS%f;VS%f;\n",force,velocity);
   scale = 40.0*v->dx/(v->nx-1.0); // 40/mm
   if (loc == 'l') {
      xoffset = 40.0*(ox);
      yoffset = 40.0*(oy);
      }
   else if (loc == 'r') {
      xoffset = 40.0*(ox - v->dx);
      yoffset = 40.0*(oy);
      }
   else if (loc == 'L') {
      xoffset = 40.0*(ox);
      yoffset = 40.0*(oy - v->dy);
      }
   else if (loc == 'R') {
      xoffset = 40.0*(ox - v->dx);
      yoffset = 40.0*(oy - v->dy);
      }
   v->path->segment = v->path->last;
   while (1) {
      //
      // follow segments
      //
      v->path->segment->point = v->path->segment->first;
      x = xoffset + scale * v->path->segment->point->first->value;
      y = yoffset + scale * (v->ny - v->path->segment->point->first->next->value);
      fprintf(output_file,"PU%d,%d;\n",x,y); // move up to start point
      fprintf(output_file,"PU%d,%d;\n",x,y); // hack: repeat in case comm dropped
      fprintf(output_file,"PD%d,%d;\n",x,y); // move down
      fprintf(output_file,"PD%d,%d;\n",x,y); // hack: repeat in case comm dropped
      nsegs += 1;
      while (1) {
         //
         // follow points
         //
         if (v->path->segment->point->next == 0)
            break;
         v->path->segment->point = v->path->segment->point->next;
         x = xoffset + scale * v->path->segment->point->first->value;
         y = yoffset + scale * (v->ny - v->path->segment->point->first->next->value);
         fprintf(output_file,"PD%d,%d;\n",x,y); // move down
         fprintf(output_file,"PD%d,%d;\n",x,y); // hack: repeat in case comm dropped
         npts += 1;
         }
      //fprintf(output_file,"\n",x,y);
      if (v->path->segment->previous == 0)
         break;
      v->path->segment = v->path->segment->previous;
      }
   fprintf(output_file,"PU0,0;\n"); // pen up to origin
   fprintf(output_file,"PU0,0;\n"); // hack: repeat in case comm dropped
   fclose(output_file);
   printf("wrote %s\n",output_file_name);
   printf("   segments: %d, points: %d\n",nsegs,npts);
   }

main(int argc, char **argv) {
   //
   // local vars
   //
   struct fab_vars v;
   init_vars(&v);
   float ox,oy,force,velocity;
   char loc;
   //
   // command line args
   //
   if (!((argc == 3) || (argc == 4) || (argc == 5) || (argc == 7) || (argc == 8))) {
      printf("command line: path_camm in.path out.camm [force [velocity [x y [location]]]]\n");
      printf("   in.path = input path file\n");
      printf("   out.camm = output Roland vinylcutter file\n");
      printf("   force = cutting force (optional, grams, default 45)\n");
      printf("   velocity = cutting speed (optional, cm/s, default 2)\n");
      printf("   x = origin x (optional, mm, default 0)\n");
      printf("   y = origin y (optional, mm, default 0)\n");
      printf("   location = origin location (optional, bottom left:l, bottom right:r, top left:L, top right:R, default l)\n");
      exit(-1);
      }
   force = 45;
   velocity = 2;
   ox = 0;
   oy = 0;
   loc = 'l';
   if (argc >= 4) {
      sscanf(argv[3],"%f",&force);
      }
   if (argc >= 5) {
      sscanf(argv[4],"%f",&velocity);
      }
   if (argc >= 7) {
      sscanf(argv[5],"%f",&ox);
      sscanf(argv[6],"%f",&oy);
      }
   if (argc >= 8) {
      sscanf(argv[7],"%c",&loc);
      }
   //
   // read path
   //
   fab_read_path(&v,argv[1]);
   //
   // write .epi
   //
   fab_write_camm(&v,argv[2],force,velocity,ox,oy,loc);
   }

