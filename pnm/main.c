/**
 * main.c
 *
 * Ce fichier contient la fonction main() du programme de manipulation
 * de fichiers pnm.
 *
 * @author: Papadopoulos Christos S203720
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

#include "pnm.h"

//TODO: Rajouter les asserts, verifier les messages d'erreur, nettoyer code, vérifier le output avec les caractères spéciaux, rajouter des commentaires, nettoyer noms des variables.

//TODO: Dans le futur, enlever la detecttion du format pour prendre en charge n'importe quel fichier et ainsi convertir et detecter automatiquement le type de fichier grace au nombre magique.

//TODO: Regarder l'ordre des membres d'une structure car la variable columns onterfere avec la variable intensity qui se trouve dans la structure quand on free ava,t de crée la variable intensity


//TODO VErifier si la matrice existe sinon envoyer un message d'erreur
//TODO: si il y a juste le header mais pas de matrice --> boucle infinie
//TODO: Verifier que le nombre de colonnes et de lignes corresponds bien


int main(int argc, char *argv[]) {
   
   char *optstring = ":f:i:o:";
   char *inputName = NULL;
   char *outputName = NULL;
   char fileType[4] = {0};
   
   int val = 0;
   
   while ((val = getopt(argc, argv, optstring)) != EOF)
   {
      switch (val) {
         case 'i':
            inputName = (char*) malloc((strlen(optarg)+1)*sizeof(char));
            if (inputName == NULL)
            {
               printf("Malloc error, input name is too long !\n");
               if (outputName != NULL)
               {
                  free(outputName);
                  
               }
               return -1;
            }
            strcpy(inputName, optarg);
            
            break;
         case 'o':
            
            outputName = (char*) malloc((strlen(optarg)+1)*sizeof(char));
            if (outputName == NULL)
            {
               printf("Malloc error, output name is too long !\n");
               if (inputName != NULL)
               {
                  free(inputName);
               }
               return -1;
            }
            strcpy(outputName, optarg);
            
            break;
         case 'f':
            if (strlen(optarg) > 3)
            {
               printf("File type argument is too long ! MAX 3 characters !\n");
               if (inputName !=NULL)
               {
                  
                  free(inputName);
                  
               }
               if (outputName != NULL)
               {
                  
                  free(outputName);
               }
               return -1;
            }
            strcpy(fileType, optarg);
            
            break;
         case '?':
            printf("Unknown option -%c\n", optopt);
            break;
         case ':':
            printf("Missing arg for -%c\n", optopt);
            break;
      }
      
   }
   
   
   for (int index = optind; index < argc; index++)
   {
      printf("Non-option argument : %s\n", argv[index]);
   }
   
   if (inputName != NULL && outputName != NULL && (strlen(fileType)) != 0)
   {
      char *extensionInput = inputName;
      if((extensionInput = strrchr(inputName, '.')) != NULL)
      {
         //On rajoute +1 pour enlever le '.' au debut de l'extension
         extensionInput = extensionInput+1;
         
         if (strcmp(extensionInput, fileType) != 0)
         {
            printf("The file extension in the input does not match the file format entered !\n");
            return -1;
            
         }
         
         
         
      }
      else
      {
         printf("The file extension in the input does not match the file format entered !\n");
         return -1;
      
      }
      
      char *extensionOutput = outputName;
      if((extensionOutput = strrchr(outputName, '.')) != NULL)
      {
         
         //On rajoute +1 pour enlever le '.' au debut de l'extension
         extensionOutput = extensionOutput+1;
         
         if (strcmp(extensionOutput, fileType) != 0)
         {
            printf("The file extension in the output does not match the file format entered !\n");
            return -1;
            
            
         }
         
      }
      
      else{
         char *arr = realloc(outputName, strlen(outputName)+5);
         if (arr == NULL)
         {
            printf("Error with malloc !\n");
            if (inputName != NULL)
            {
               free(inputName);
            }
            return -1;
            
         }
         outputName = arr;
         strcat(outputName, ".");
         strcat(outputName, fileType);
         extensionOutput = fileType;
      }
      
      
      PNM *pnm_struct;
      int status = load_pnm(&pnm_struct, inputName);
      
      switch (status)
      {
         case -1:
            printf("Could not allocate memory ! Maybe the file is too big !\n");
            return -1;
            break;
         case -2:
            printf("File \"%s\" does not exist !\n", inputName);
            return -1;
         case -3:
            printf("The file is corrupted!\n");
            return -1;
      }
      
      status = 0;
      status = write_pnm(pnm_struct, outputName);
      switch(status)
      {
            
         case -1:
            printf("Could not allocate memory ! Maybe the file is too big !\n");
            return -1;
            break;
         case -2:
            printf("Coudn't save the output at path : %s\n", outputName);
            break;
      }
      free(outputName);
      free(inputName);
      destroy_pnm(pnm_struct);
      
      
      
      
      
   }
   
   
   else{
      printf("Usage :\n");
      printf("\t -i : input File to read from.\n");
      printf("\t -o : output File to write to.\n");
      printf("\t -f : the file format we read from.\n\n");
      
   }
   return 0;
}
