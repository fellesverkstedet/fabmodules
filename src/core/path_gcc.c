//
// path_gcc.c
//    convert path to GCC lasercutter .gcc
// Based on path_epi.c
//
// Neil Gershenfeld 8/18/13
// (c) Massachusetts Institute of Technology 2013
// Jon Nordby 9/1/14
// (c) Jon Nordby, Fellesverkstedet 2014
//
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose, but must
// acknowledge the fab modules project. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include "fab.h"

// Machine expects 16 pen values for PPI/power/speed, encoded as ascii
static int write_pen_values(char *str, size_t n, int v[16]) {

   int written = snprintf(str, n,
                "%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d%04d",
                v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8],
                v[9], v[10], v[11], v[12], v[13], v[14], v[15], v[16], v[17]);
   return written;
}

void fab_write_gcc(struct fab_vars *v, char *output_file_name,
                    int power, int speed, int focus,
                    float ox, float oy, char loc,
                    int rate, int max_power) {
   //
   // write path to GCC lasercutter file
   //
	FILE *output_file;
   int i,x,y,z,current_z,layer_power,nsegs=0,npts=0;
   float scale,xoffset,yoffset;
   output_file = fopen(output_file_name,"w");
//   scale = 600.0*v->dx/(25.4*(v->nx-1.0)); // 600 DPI
   // FIXME: calculate proper scale
   scale = 3.35f;

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

   // Limit filename to 80 chars 
   char *filename[80];
   snprintf(filename, 80, "%s", output_file_name);

   // Start
   fprintf(output_file,"@PJL JOB NAME=%s\r\nE@PJL ENTER LANGUAGE=PCL\r\n%%-12345X", filename);

   // Reset
   fprintf(output_file,"E");

   // Unit
   fprintf(output_file,"&u%dD", 254);

   // Cursor
   fprintf(output_file,"*p%dX", 100);
   fprintf(output_file,"*p%dY", 100);

   // Set filename
   fprintf(output_file,"!m%dN%s", strlen(filename), filename); 

   // Set raster power+velocity. Works but only affects raster. Shows in display before start
   fprintf(output_file,"!r%dI", rate);
   fprintf(output_file,"!r%dP", power*10);

    // Select first (and only) pen
//   fprintf(output_file,"!v%dD", 1);
//   fprintf(output_file,"!m%dA", 2); // somehow triggers auto-focus??

   // Use same value for all 16 pens
   int velocities[16];
   int ppis[16];
   int powers[16];
   {
   int i;
   for (i=0; i<16; i++) {
      velocities[i] = speed*10; // thousands
      powers[i] = power*10; // thousands
      ppis[i] = rate;
   }
   }

   char buf[65];

   // Set power
   write_pen_values(buf, 65, powers);
   fprintf(output_file,"!v%dP%s", 64, buf);
   // Velocity
   write_pen_values(buf, 65, velocities);
   fprintf(output_file,"!v%dV%s", 64, buf);
   // PPI
   write_pen_values(buf, 65, ppis);
   fprintf(output_file,"!v%dI%s", 64, buf);

    // Enter HPGL vector mode
   fprintf(output_file,"%%1B");

   current_z = 0;
//   fprintf(output_file,"YP%d;\n",power);
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
            //fprintf(output_file,"YP%d;\n",layer_power);
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
               //fprintf(output_file,"YP%d;\n",layer_power);
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

   // Leave HPGL vector mode
   fprintf(output_file,"%%1A");

   // Reset
   fprintf(output_file,"E");

   // End
   fprintf(output_file,"%%-12345X@PJL EOJ \r\n");


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
      printf("command line: path_gcc in.path out.gcc [power [speed [focus [x y [ location [rate [max_power]]]]]]]\n");
      printf("   in.path = input path file\n");
      printf("   out.gcc= output GCC lasercutter file\n");
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
   // write .gcc
   //
   fab_write_gcc(&v,argv[2],power,speed,focus,ox,oy,loc,rate,max_power);
   }

