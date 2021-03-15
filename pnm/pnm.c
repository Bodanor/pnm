/**
 * pnm.c
 *
 * Ce fichier contient les définitions de types et
 * les fonctions de manipulation d'images PNM.
 *
 * @author: Papadopoulos Christos S203720
 * @date: 14/03/2021
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "pnm.h"


/**
 * Définition du type opaque PNM
 *
 */
struct PNM_t {
   char fileType[3];
   int columns;
   int rows;
   int intensity;
   int **matrix;
   
   
   
   
};


PNM *create_pnm()
{
   PNM *pnmStruct = malloc(sizeof(PNM));
   if (pnmStruct == NULL)
   {
      return NULL;
   }
   pnmStruct->intensity = 0;
   return pnmStruct;
   
   
}

void destroy_pnm(PNM *image)
{
   
   assert(image != NULL);
   for (int i = 0; i < image->rows; i++)
   {
      if (image->matrix[i] != NULL)
      {
         
         free(image->matrix[i]);
      }
   }
   
   if (image->matrix != NULL && image!= NULL)
   {
      
      free(image->matrix);
      free(image);
   }
   
   
}


int load_pnm(PNM **image, char* filename) {
   
   assert(image != NULL && filename != NULL);
   PNM *pnmStruct = create_pnm();
   
   if (pnmStruct == NULL)
   {
      
      return -1;
   }
   FILE *pnmFile = fopen(filename, "rb");
   if (pnmFile == NULL)
   {
      
      return -2;
   }
   
   int status = get_magic_number(filename, pnmFile, pnmStruct);
   
   if (status != 0)
   {
      fclose(pnmFile);
      free(pnmStruct);
      return -3;
      
   }
   
   status = get_header(pnmFile, pnmStruct);
   
   if (status != 0)
   {
      fclose(pnmFile);
      free(pnmStruct);
      return status;
   }
   
   status = load_matrix(pnmFile, pnmStruct);
   
   if (status != 0)
   {
      fclose(pnmFile);
      destroy_pnm(pnmStruct);
      return status;
      
   }
   
   
   (*image) = pnmStruct;
   fclose(pnmFile);
   return 0;
   
   
}

int get_header(FILE *pnmFile, PNM *pnmStruct)
{
   assert(pnmFile != NULL && pnmStruct != NULL);
   
   char character = '\0';
   char *columns = calloc(1, sizeof(char));
   char *rows = calloc(1,sizeof(char));
   char *intensity = calloc(1, sizeof(char));
   
   if (columns == NULL || rows == NULL || intensity == NULL)
   {
      
      return -1;
      
   }
   int columnFlag = 0;
   int rowFlag = 0;
   int intensityFlag = 0;
   int headerFlag = 0;
   
   while((headerFlag  == 0) && (character = getc(pnmFile)) != EOF)
   {
      
      if (character == '\r')
      {
         character = '\0';
      }
      
      
      if (character == '#')
      {
         
         while ((character = fgetc(pnmFile)) != '\n');
      }
      while (character == '\n')
      {
         
         character = fgetc(pnmFile);
      }
      
      if (columnFlag == 0)
      {
         while (character == ' ')
         {
            
            character = fgetc(pnmFile);
         }
         
         while (character != ' ' && character != '\n' && character != '\r')
         {
            
            
            char *arr = realloc(columns, strlen(columns)+2);
            if (arr == NULL)
            {
               
               
               free(rows);
               free(intensity);
               return -1;
            }
            columns = arr;
            strncat(columns, &character, 1);
            character = fgetc(pnmFile);
            
            
            
         }
         
         for (int i = 0; i < strlen(columns); i ++)
         {
            
            if (!isdigit(columns[i]))
            {
               
               
               free(columns);
               free(rows);
               free(intensity);
               return -3;
            }
            
         }
         pnmStruct->columns = atoi(columns);
         free(columns);
         columnFlag = 1;
         
      }
      
      else if (rowFlag == 0)
      {
         
         while (character == ' ')
         {
            
            character = fgetc(pnmFile);
         }
         
         while (character != ' ' && character != '\n' && character != '\r')
         {
            
            char *arr = realloc(rows, strlen(rows)+2);
            if (arr == NULL)
            {
               
               free(intensity);
               return -1;
               
            }
            rows = arr;
            strncat(rows, &character, 1);
            character = fgetc(pnmFile);
            
         }
         // Check si il y a bien deux nombres après le nombres de colonnes et de lignes. Si oui on continue, sinon on a une erreur de header.
         
         if (character != '\n')
         {
            return -3;
         }
         
         
         for (int i = 0; i < strlen(rows); i++)
         {
            if (!isdigit(rows[i]))
            {
               
               free(rows);
               free(intensity);
               
               return -3;
               
            }
            
            
         }
         
         pnmStruct->rows = atoi(rows);
         free(rows);
         rowFlag = 1;
         
      }
      
      else if (intensityFlag == 0)
      {

         if (strcmp(pnmStruct->fileType, "P3") == 0|| (strcmp(pnmStruct->fileType, "P2")) == 0)
         {
            if (intensityFlag == 0)
            {
               while (character == ' ')
               {

                  character = fgetc(pnmFile);
               }

               while (character != ' ' && character != '\n' && character != '\r')
               {

                  
                  char *arr = realloc(intensity, strlen(intensity)+2);
                  if (arr == NULL)
                  {

                     return -1;
                  }
                  
                  intensity = arr;
                  strncat(intensity, &character, 1);
                  character = fgetc(pnmFile);
               }
               
               // Check si il y a bien deux nombres après l'intensité. Si oui on continue, sinon on a une erreur de header.
               
               if (character != '\n')
               {
                  return -3;
               }
               
               for (int i = 0; i < strlen(intensity); i++)
               {
                  if (!isdigit(intensity[i]))
                  {

                     free(intensity);
                     return -3;
                  }

               }


               pnmStruct->intensity = atoi(intensity);
               free(intensity);
               intensityFlag = 1;

            }


         }
         headerFlag = 1;

      }
      
   }
   
   return 0;
}


int load_matrix(FILE *pnmFile, PNM *pnmStruct)
{
   
   int columns = pnmStruct->columns;
   int rows = pnmStruct->rows;
   
   if (strcmp(pnmStruct->fileType, "P3") == 0)
   {
      columns *=3;
      
   }
   pnmStruct->matrix = (int **)malloc(sizeof(int*)*rows);
   if (pnmStruct->matrix == NULL)
   {
      
      return -1;
      
      
   }
   
   for (int i = 0; i < rows; i++)
   {
      pnmStruct->matrix[i] = (int*)malloc(sizeof(int)*(columns));
      if (pnmStruct->matrix[i] == NULL)
      {
         return -1;
         
      }
      
   }
   
   //Copier des valeurs dans la matrice :
   char *number = calloc(1, sizeof(char));
   if (number == NULL)
   {
      return -1;
   }
   
   for (int i = 0; i < rows; i++)
   {
      for (int j = 0; j < columns; j++)
      {
         
         fscanf(pnmFile,"%s", number);
         
         for (int x = 0; x < strlen(number); x++)
         {
            
            if (!isdigit(number[x]))
            {
               
               return -3;
            }
         }
         
         pnmStruct->matrix[i][j] = atoi(number);
         
      }
      
   }
   free(number);
   
   
   return 0;
}



int write_pnm(PNM *image, char* filename) {
   
   assert(image != NULL && filename != NULL);
   
   
   FILE *outputFile = fopen(filename, "wb");
   if (outputFile == NULL)
   {
      return -2;
      
   }
   
   fprintf(outputFile, "%s\n", image->fileType);
   fprintf(outputFile, "%d %d\n", image->columns, image->rows);
   
   if (image->intensity !=0)
   {
      fprintf(outputFile, "%d\n", image->intensity);

   }
   
   
   if (strcmp(image->fileType, "P3") == 0)
   {
      int spacingCount = 0;
      for (int i = 0; i < image->rows; i++)
      {
         for (int j = 0; j < image->columns*3; j++)
         {
            
            fprintf(outputFile, "%d", image->matrix[i][j]);
            spacingCount++;
            if (spacingCount == 3)
            {
               fprintf(outputFile, "\t");
               
               spacingCount = 0;
            }
            else{
               fprintf(outputFile, " ");
               
            }
            
            
         }
         fprintf(outputFile, "\n");
         spacingCount = 0;
         
         
      }
      
      
   }
   
   else{
      
      for (int i = 0; i < image->rows; i++)
      {
         for (int j = 0; j < image->columns; j++)
         {
            fprintf(outputFile, "%d ", image->matrix[i][j]);
            
         }
         fprintf(outputFile, "\n");
         
      }
      
   }
   fclose(outputFile);
   return 0;
}


int get_magic_number(char *filename, FILE *pnmFile, PNM *image)
{
   //Rajouter assert
   
   char *magicNumber = calloc(1,sizeof(char));
   int magicFlag = 0;
   //Malloc erreur :
   char character = '\0';
   
   while ((magicFlag == 0) && (character = getc(pnmFile)) != EOF)
   {
      if (character == '\r')
      {
         character = '\0';
      }
      if (character == '#')
      {
         while ((character = fgetc(pnmFile)) != '\n');
         
      }
      
      while (character == '\n')
      {
         
         character = fgetc(pnmFile);
      }
      
      
      while (character == ' ')
      {
         character = fgetc(pnmFile);
      }
         
      while (character != ' ' && character != '\n' && character != '\r')
      {
         
         char *arr = realloc(magicNumber, strlen(magicNumber)+2);
         if (arr == NULL)
         {
            return -1;
            
         }
         magicNumber = arr;
         
         strncat(magicNumber, &character, 1);
         character = fgetc(pnmFile);
         
      }
      magicFlag = 1;
      
   }
   
   
   if (strcmp(magicNumber, "P1") == 0)
   {
      strcpy(image->fileType, magicNumber);
      
      
   }
   else if (strcmp(magicNumber, "P2") == 0)
   {
      strcpy(image->fileType, magicNumber);
      
   }
   
   else if (strcmp(magicNumber, "P3") == 0)
   {
      strcpy(image->fileType, magicNumber);
      
   }
   else{
      
      return -3;
   }
   free(magicNumber);
   return 0;
}

