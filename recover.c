#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
  //Check command-line arguments
  if(argc != 2){
    
    printf("ERROR: Usage = ./volume imageToRecover.raw \n");
    return 1;
  }

  //Open memorycard en check of het goed gelezen wordt
  char *input_name = argv[1];
  FILE *input_pointer = fopen(argv[1], "r");
  if (input_pointer == NULL){
    printf("ERROR: Could not open %s. \n", input_name);
    return 2;
  }

  //create a buffer for a block of data
  BYTE buffer[BLOCK];
  int count = 0;

  //create filename
  char filename[8]; //elke filename bestaat uit 8 delen: 7 karakters + Null
  sprintf(filename, "%03d.jpg", count);

  //open output file + check if not null
  FILE *output_pointer = fopen(filename, "w");
  if (output_pointer == NULL){
    printf("ERROR: Cannot create %s file. \n", filename);
    fclose(input_pointer);
    return 3;
  }

  // while there is still date left to read from te card
  while (fread(&buffer, sizeof(BYTE), BLOCK, input_pointer) == BLOCK){
    //look for jpeg file
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){
        if (count == 0){
          fwrite(&buffer, sizeof(BYTE), BLOCK, output_pointer);
          count++;
        }else{
          //close de vorige file
          fclose(output_pointer);
          
          //update de counter en creeer een nieuwe file
          count++;
          sprintf(filename, "%03d.jpg", count);
          
          //open output file + check of output file oke is;
          output_pointer = fopen(filename, "w");
          if (output_pointer == NULL){
            printf("ERROR: Cannot create %s file. \n", filename);
            fclose(output_pointer);
            return 4;
          }
          //schrijf blok weg naar de output file
          fwrite(&buffer, sizeof(BYTE), BLOCK, output_pointer);
        }
      //indien het geen begin is, ga verder met schrijven in dezelfde file
      }else{ 
      fwrite(&buffer, sizeof(BYTE), BLOCK, output_pointer);
      }
  }
  //zorg dat je alle files gesloten hebt
  fclose(output_pointer);
  fclose(input_pointer);
  return 0;
}
