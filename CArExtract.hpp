#ifndef CAREXTRACT_H
#define CAREXTRACT_H

#include <stdio.h>

int ExtractArchive(char* cpArchiveName, char* cpExtractionDirectory)
{
	FILE* filep = fopen(cpArchiveName, "rb");

	if(filep == NULL)
	{
		printf("\n%s does not exist!", cpArchiveName);
		return -1;
	}

	fseek(filep , 0 , SEEK_END);
  unsigned int unFileSize = ftell (filep);
  rewind(filep);

  char* cpFileBuffer = new char[unFileSize];
  fread(cpFileBuffer, 1, unFileSize, filep);
  fclose(filep);

  int nObjectCount = 0;
  int nHeadIndex = -1;
  int nTailIndex = -1;
  char cpObjectName[200];

  	for(int nIndex = 0; nIndex < unFileSize; nIndex++)
  	{
  		if(cpFileBuffer[nIndex] == 0x7F)
  			if(cpFileBuffer[nIndex + 1] == 'E' && cpFileBuffer[nIndex + 2] == 'L' && cpFileBuffer[nIndex + 3] == 'F')
        {
  				printf("\nFound ELF at %d", nIndex);
          if(nObjectCount == 0) //First encounter
          {
            nHeadIndex = nIndex;
          }
          else
          {
            nTailIndex = nIndex;  //Note tail points to one char after obj ends
            
            sprintf(cpObjectName, "%sobj_%d",cpExtractionDirectory, nObjectCount);
            
            filep = fopen(cpObjectName,"wb");
            fwrite(cpFileBuffer + nHeadIndex, 1, nTailIndex - nHeadIndex, filep);
            fclose(filep);

            nHeadIndex = nTailIndex;
          }


          nObjectCount++;
        }
  	} 
    nTailIndex = unFileSize;  //Note tail points to one char after obj ends
            
    sprintf(cpObjectName, "%sobj_%d",cpExtractionDirectory, nObjectCount);
            
    filep = fopen(cpObjectName,"wb");
    fwrite(cpFileBuffer + nHeadIndex, 1, nTailIndex - nHeadIndex, filep);
    fclose(filep);

    return nObjectCount;
}











#endif
