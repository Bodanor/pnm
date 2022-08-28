/**
 * main.c
 * 
 * Ce fichier contient la fonction main() du programme de manipulation
 * de fichiers pnm.
 *
 * @author: Nom Prenom Matricule
 * @date: 
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

#include "libpnm/pnm.h"


int main(int argc, char *argv[]) {

   char *optstring = "i:o:f:";
   char *input_path = NULL;
   char *output_path = NULL;
   char file_format[4] = {0};
   int c;

   while ((c = getopt(argc, argv, optstring)) != -1)
   {
      switch (c)
      {
         case 'i':
            input_path = (char *)malloc(sizeof(char)*strlen(optarg) + 1);
            if (input_path == NULL)
            {
               printf("Malloc error for input path !\n");
               return -1;
            }
            else
               strcpy(input_path, optarg);
            break;
         case 'o':
            output_path = (char*)malloc(sizeof(char)*strlen(optarg) + 1);
            if (output_path == NULL)
            {
               printf("Malloc error for output path !\n");
               if (input_path != NULL)
                  free(input_path);
               return -1;
            }
            else
               strcpy(output_path, optarg);
            break;

         case 'f':
            strncpy(file_format, optarg, 4);
            break;
         
         case '?':
               return -1;

         case ':':
            return -1;
            break;
         
         default:
            abort();


      }
   }

   if (input_path == NULL)
   {
      printf("No input file path !\n");
      return -1;
   }
   else if (output_path == NULL)
   {
      printf("No output file path !\n");
      return -1;
   }
   else if (*file_format == '\0')
   {
      printf("No file format given !\n");
      return -1;
   }

   PNM *image = createPNM(file_format);
   if (image == NULL)
      return -1;
   
   if (load_pnm(&image, input_path) != 0)
      return -1;

   write_pnm(image, output_path);
   
   return 0;
}

