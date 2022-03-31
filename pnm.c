/**
 * pnm.c
 * 
 * Ce fichier contient les définitions de types et 
 * les fonctions de manipulation d'images PNM.
 * 
 * @author: Nom Prenom Matricule
 * @date: 
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pnm.h"


static char forbidden_char[] = 
{
   '/',
   '\\',
   ':',
   '*',
   '?',
   '\"',
   '<',
   '>',
   '|',
   '.'
};


/**
 * Définition du type opaque PNM
 *
 */
struct PNM_t {

   char file_format[4];
   char header_format[3];
   int pixel_intensity;
   int rows;
   int columns;
   int **matrix;


};

static FILE *openFile(char *filename)
{
   assert(filename != NULL);

   FILE *fp = fopen(filename, "r+b");
   if (fp == NULL)
   {
      fp = fopen(filename, "w+b");
      if (fp == NULL)
      {
         printf("Couldn't load file %s\nReason : %s", filename, strerror(errno));
         return NULL;
      }
   }
   return fp;
}

static int verify_matching_extension(PNM *image, char *filename)
{
   short i;
   char *ext_pt = filename + (strlen(filename) - 3);

   for (i = 0; i < 3; i++)
      ext_pt[i] = tolower(ext_pt[i]);
   
   if (strcmp(image->file_format, ext_pt) == 0)

      return 0;
   else
   {
      printf("extension passed differs from actual file extension !\n\n");
      printf("File extension --> %s\n", image->file_format);
      printf("Extension passed --> %s\n", ext_pt);
      return -1;

   }   
   
}

static int verify_header_format(PNM *image,FILE *input_file)
{
   assert(image != NULL);
   assert(input_file != NULL);

   int c;
   char *pt_format = image->header_format;

   while ((c = fgetc(input_file)) != '\n' && c != '\r')
   {
      if (c == '#')
         while ((c = fgetc(input_file)) != '\n' && c != '\r')
            ;
      else
         *pt_format++ = c;
   }
   *pt_format = '\0';
   if (strcmp(image->header_format, "P1") != 0 && strcmp(image->header_format, "P2") != 0 && strcmp(image->header_format, "P3") != 0)
   {
      printf("Header describing file format is corrupted\n");
      return -1;
   }

   if (strcmp(image->header_format, "P1") == 0 && strcmp(image->file_format, "pbm") != 0)
   {
      printf("Extension passed differs from the one in the header !\n\n");
      printf("Header extension --> %s\n", image->header_format);
      printf("Extension passed --> %s (which must be \"pbm\" for a \"P1\" in the header !)\n", image->file_format);
      return -2;
   }
   else if (strcmp(image->header_format, "P2") == 0 && strcmp(image->file_format, "pgm") != 0)
   {
      printf("Extension passed differs from the one in the header !\n\n");
      printf("Header extension --> %s\n", image->header_format);
      printf("Extension passed --> %s (which must be \"pgm\" for a \"P2\" in the header !)\n", image->file_format);
      return -2;
   }
   else if (strcmp(image->header_format, "P3") == 0 && strcmp(image->file_format, "ppm") != 0)
   {
      printf("Extension passed differs from the one in the header !\n\n");
      printf("Header extension --> %s\n", image->header_format);
      printf("Extension passed --> %s (which must be \"ppm\" for a \"P3\" in the header !)\n", image->file_format);
      return -2;
   }
   else
      return 0;

}
static int get_matrix_size(PNM *image, FILE *input_file)
{
   assert(image != NULL);
   assert(input_file != NULL);

   int c, i;
   char columns[10];
   char rows[10];
   char *pt_columns= columns;
   char *pt_rows = rows;

   while ((c = fgetc(input_file)) != ' ')
   {
      if (c == '#')
         while ((c = fgetc(input_file)) != '\n' && c != '\r')
            ;
      else
         *pt_columns++ = c;
   }

   *pt_columns= '\0';

   while ((c = fgetc(input_file)) != '\n' && c != '\r')
   {
      if (c == '#')
         while ((c = fgetc(input_file)) != '\n' && c != '\r')
            ;
      else
         *pt_rows++ = c;
   }
   *pt_rows = '\0';

   for (i = 0; i < (int)strlen(rows); i++)
      if (isdigit(rows[i]) == 0)
      {
         printf("Columns numbers in the header file is corrupted. Non digit char detected !\n");
         return -1;
      }
   
   image->columns = atoi(columns);
   for (i = 0; i < (int)strlen(rows); i++)
      if (isdigit(rows[i]) == 0)
         {
            printf("Rows numbers in the header file is corrupted. Non digit char detected !\n");
            return -1;
         }


   image->rows = atoi(rows);
   return 0;
}

static int get_pixel_intensity(PNM *image, FILE *input_file)
{
   assert(image != NULL);
   assert(input_file != NULL);

   int c, i;
   char pixel_intensity[10];
   char *pt_pixel = pixel_intensity;

   while ((c = fgetc(input_file)) != '\n' && c != '\r')
   {
      if (c == '#')
         while ((c = fgetc(input_file)) != '\n' && c != '\r')
            ;
      else
         *pt_pixel++ = c;
   }

   *pt_pixel = '\0';

   for (i = 0; i < (int)strlen(pixel_intensity); i++)
      if (isdigit(pixel_intensity[i]) == 0)
      {
         printf("Pixel intensity in the header file is corrupted. Non digit char detected !\n");
         return -1;
      }
   
   image->pixel_intensity = atoi(pixel_intensity);
   return 0;


}
static int allocate_matrix(PNM *image)
{
   assert(image != NULL);

   int i;

   image->matrix = (int **)malloc(sizeof(int*)*image->rows);
   if (image->matrix == NULL)
      return -1;

   for (i = 0; i < image->rows; i++)
   {
      *(image->matrix + i) = (int*)malloc(image->columns * sizeof(int));
      if (*(image->matrix + i) == NULL)
         return -1;
   }
   
   return 0;

}
static int load_matrix(PNM *image, FILE *input_file)
{
   int c;
   int i, j;
   i = j = 0;

   char buffer[10];
   char *pt_buff = buffer;

   while ((c = fgetc(input_file)) != EOF)
   {
      if (c == '#')
         while ((c = fgetc(input_file)) != '\n' && c != '\r')
            ;
      else if (c != '\n' && c != ' ' && c != '\r')
      {
         if (isdigit(c) == 0)
         {
            printf("Non digit char detected in the pixel descriptor of the file %d!\n",i);
            return -1;
         }
         *pt_buff++ = c;
      }
      else if (pt_buff != buffer)
      {
         *pt_buff = '\0';

         if (i == image->columns)
         {
            i = 0;
            j++;
         }
         *(*(image->matrix + j) + i) = atoi(buffer);
         i++;
         pt_buff = buffer;

      }
   }
   return 0;
}
int load_pnm(PNM **image, char* filename) {

   assert(*image != NULL);
   assert (filename != NULL);

   FILE *input_file = openFile(filename);
   if (input_file == NULL)
      return -1;
   
   if (verify_matching_extension(*image, filename) == -1)
      return -2;

   printf("Loading data from \"%s\"...\n", filename);

   if(verify_header_format(*image, input_file) == -1)
      return -3;

   if (get_matrix_size(*image, input_file) == -1)
      return -1;
   
   if (allocate_matrix(*image) == -1)
      return -1;
   
   if (strcmp((*image)->header_format, "P2") == 0 || strcmp((*image)->header_format, "P3") == 0)
      if (get_pixel_intensity(*image, input_file) == -1)
         return -1;
   
   if (load_matrix(*image, input_file) == -1)
      return -1;
   
   fclose(input_file);
   
   return 0;
}

int write_pnm(PNM *image, char* filename) {

   FILE *output_file = openFile(filename);
   int i, j;

   if (output_file == NULL)
      return -1;
   
   printf("Writing data to \"%s\"...", filename);

   fprintf(output_file, "%s\n", image->header_format);
   fprintf(output_file, "%d %d\n", image->columns, image->rows);
   if (strcmp((image)->header_format, "P2") == 0 || strcmp((image)->header_format, "P3") == 0)
   {
      fprintf(output_file, "%d\n", image->pixel_intensity);
   }

   for (j = 0; j < image->rows; j++)
   {
      for (i = 0; i < image->columns; i++)
      {
         fprintf(output_file, "%d\t",  *(*(image->matrix + j) + i));
      }
      fputc('\n', output_file);
   }
   fclose(output_file);
   
   return 0;
}

PNM *createPNM(char *filetype)
{
   PNM *image = (PNM*)malloc(sizeof(PNM));
   if (image == NULL)
      printf("Malloc error !\n");
   
   strcpy(image->file_format, filetype);
   return image;

}
int verify_forbidden_input_char(char *string)
{
   assert(string != NULL);

   size_t i;
   size_t x;
   char *str_pt = string;

   for (i = 0; i < strlen(string); i++)
   {
      for (x = 0; x < sizeof(forbidden_char); x++)
      {
         if (*str_pt == forbidden_char[x])
            return -1;
      }
      str_pt++;
   }
   return 0;
}