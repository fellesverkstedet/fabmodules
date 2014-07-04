//
// path_epi.c
//    convert path to Epilog lasercutter .epi
//
// Neil Gershenfeld 8/18/13
// (c) Massachusetts Institute of Technology 2013
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge the fab modules project. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include "fab.h"

void fab_write_epi(struct fab_vars *v, char *output_file_name, int power, int speed, int focus, float ox, float oy, char loc, int rate, int max_power) {
   //
   // write path to Epilog lasercutter file
   //
	FILE *output_file;
   int i,x,y,z,current_z,layer_power,nsegs=0,npts=0;
   float scale,xoffset,yoffset;
   output_file = fopen(output_file_name,"w");
   scale = 600.0*v->dx/(25.4*(v->nx-1.0)); // 600 DPI
   if (loc == 'l') {
      xoffset = 600.0*(ox)/25.4;
      yoffset = 600.0*(oy - v->dy)/25.4;
      }
   else if (loc == 'r') {
      xoffset = 600.0*(ox - v->dx)/25.4;
      yoffset = 600.0*(oy - v->dy)/25.4;
      }
   else if (loc == 'L') {
      xoffset = 600.0*(ox)/25.4;
      yoffset = 600.0*(oy)/25.4;
      }
   else if (loc == 'R') {
      xoffset = 600.0*(ox - v->dx)/25.4;
      yoffset = 600.0*(oy)/25.4;
      }
   if (focus == 0)
      // 
      // init with autofocus off
      //
      fprintf(output_file,"%%-12345X@PJL JOB NAME=%s\r\nE@PJL ENTER LANGUAGE=PCL\r\n&y0A&l0U&l0Z&u600D*p0X*p0Y*t600R*r0F&y50P&z50S*r6600T*r5100S*r1A*rC%%1BIN;XR%d;YP%d;ZS%d;\n",output_file_name,rate,power,speed);
   else
      //
      // init with autofocus on
      //
      fprintf(output_file,"%%-12345X@PJL JOB NAME=%s\r\nE@PJL ENTER LANGUAGE=PCL\r\n&y1A&l0U&l0Z&u600D*p0X*p0Y*t600R*r0F&y50P&z50S*r6600T*r5100S*r1A*rC%%1BIN;XR%d;YP%d;ZS%d;\n",output_file_name,rate,power,speed);
   current_z = 0;
   fprintf(output_file,"YP%d;\n",power);
   v->path->segment = v->path->last;
   while (1) {
      //
      // follow segments in reverse order
      //
      v->path->segment->point = v->path->segment->first;
      x = xoffset + scale * v->path->segment->point->first->value;
      y = yoffset + scale * v->path->segment->point->first->next->value;
      if (v->path->dof >= 3) {
         z = v->path->segment->point->first->next->next->value;
         if (z != current_z) {
            layer_power = power + (max_power-power) * z / (v->nz - 1.0);
            fprintf(output_file,"YP%d;\n",layer_power);
            current_z = z;
            }
         }
      if (x < 0) x = 0;
      if (y < 0) y = 0;
      fprintf(output_file,"PU%d,%d;",x,y);
      nsegs += 1;
      while (1) {
         //
         // follow points
         //
         if (v->path->segment->point->next == 0)
            break;
         v->path->segment->point = v->path->segment->point->next;
         x = xoffset + scale * v->path->segment->point->first->value;
         y = yoffset + scale * v->path->segment->point->first->next->value;
         if (v->path->dof >= 3) {
            z = v->path->segment->point->first->next->next->value;
            if (z != current_z) {
               layer_power = power + (max_power-power) * z / (v->nz - 1.0);
               fprintf(output_file,"YP%d;\n",layer_power);
               current_z = z;
               }
            }
         if (x < 0) x = 0;
         if (y < 0) y = 0;
         fprintf(output_file,"PD%d,%d;",x,y);
         npts += 1;
         }
      fprintf(output_file,"\n");
      if (v->path->segment == v->path->first)
         break;
      v->path->segment = v->path->segment->previous;
      }
   fprintf(output_file,"%%0B%%1BPUE%%-12345X@PJL EOJ \r\n");
   //
   // end-of-file padding hack from Epilog print driver
   //
   for (i = 0; i < 10000; ++i)
      fprintf(output_file," ");
   // fprintf(output_file,"%c",26); // ^z
   //
   // close and return
   //
   fclose(output_file);
   printf("wrote %s\n",output_file_name);
   printf("   segments: %d, points: %d\n",nsegs,npts);
   }

int main(int argc, char **argv) {
   //
   // local vars
   //
   struct fab_vars v;
   init_vars(&v);
   int power,max_power,speed,focus,rate;
   float ox,oy;
   char loc;
   //
   // command line args
   //
   if (!((argc == 3) || (argc == 4) || (argc == 5) || (argc == 6) || (argc == 8) || (argc == 9) || (argc == 10) || (argc == 11))) {
      printf("command line: path_epi in.path out.epi [power [speed [focus [x y [ location [rate [max_power]]]]]]]\n");
      printf("   in.path = input path file\n");
      printf("   out.epi= output Epilog lasercutter file\n");
      printf("   power = percent power, for minimum z value (optional, 0-100, default 50)\n");
      printf("   speed = percent speed (optional, 0-100, default 50)\n");
      printf("   focus = autofocus (optional, 0=off | 1=on, default on)\n");
      printf("   x = origin x (optional, mm, default 0 = left side of bed)\n");
      printf("   y = origin y (optional, mm, default 0 = back side of bed, front positive)\n");
      printf("   location = origin location (optional, bottom left:l, bottom right:r, top left:L, top right:R, default l)\n");
      printf("   rate = pulse rate (optional, frequency, default 2500)\n");
      printf("   max_power = percent power, for maximum z value (optional, 0-100, default power)\n");
      exit(-1);
      }
   power = 50;
   speed = 50;
   focus = 1;
   ox = 0;
   oy = 0;
   loc = 'l';
   rate = 2500;
   max_power = power;
   if (argc >= 4) {
      sscanf(argv[3],"%d",&power);
      }
   if (argc >= 5) {
      sscanf(argv[4],"%d",&speed);
      }
   if (argc >= 6) {
      sscanf(argv[5],"%d",&focus);
      }
   if (argc >= 8) {
      sscanf(argv[6],"%f",&ox);
      sscanf(argv[7],"%f",&oy);
      }
   if (argc >= 9) {
      sscanf(argv[8],"%c",&loc);
      }
   if (argc >= 10) {
      sscanf(argv[9],"%d",&rate);
      }
   if (argc >= 11) {
      sscanf(argv[10],"%d",&max_power);
      }
   //
   // read path
   //
   fab_read_path(&v,argv[1]);
   //
   // write .epi
   //
   fab_write_epi(&v,argv[2],power,speed,focus,ox,oy,loc,rate,max_power);
   }

