/*
 * CRC.c
 *
 *  Created on: Oct 30, 2019
 *      Author: Amir Samary
 */
#define ZF_DLL  /* Required only for dynamically linked libraries. */
#include <cdzf.h>  /* Required for all Callout code. */
#include <string.h>
#include <stdlib.h>
#include <callin.h>
#include <wchar.h>
#include <stdio.h>


#define POLY    0x8408
#define DEFAULT_CRC ((unsigned long)-1)

unsigned short CalcCRCImpl(unsigned char *startAdrptr, unsigned long lenght, unsigned long startCRC)
{
    unsigned char i;
    unsigned short crc;
    unsigned short data;

    if (startCRC == DEFAULT_CRC)
    {
        crc = 0xffff; //Preset value
    }
    else
    {
        crc = (unsigned short)startCRC;
    }

    if(lenght > 0)
    {
        do
        {
            for(i = 0, data = (unsigned short)(0xff &*startAdrptr++);i<8; i++, data >>=1)
            {
                if(((crc & 0x0001)^(data & 0x0001)) == 1)
                    crc = (unsigned short)((crc >> 1)^POLY);
                else
                    crc >>= 1;
            }
            --lenght;
        }
        while(lenght>0);
        crc = (unsigned short)(~crc);
        data = crc;
        crc = (unsigned short)((crc <<8)|(data >> 8 & 0xff));
    }

    return (unsigned short)(crc & 0xFFFF);

}

/*
 * arg1 is startAdrptr
 * arg2 is startCRC
 */
int CalcCRCProxy(IRIS_EXSTRP arg1, IRIS_EXSTRP arg2)
{
	char* startCRCString;
	unsigned long startCRC;
	unsigned int startCRCLen;
	unsigned short crc;
	char crcString[200]; //more than enough

	// PROCESS startCRC
	startCRCLen = arg2->len;

    // To use function strtoul() we must transform this into a zero ended string
	startCRCString = (char*)calloc(startCRCLen+1, sizeof(unsigned char));

    // Copying the string...
	memcpy(startCRCString, arg2->str.ch, startCRCLen*sizeof(unsigned char));

    // Done with this
    IRISEXSTRKILL(arg2);

    // Converting the zero terminated string to an unsigned long as expected by the crc funtion
    // If it is -1 (which is signed) it will become a huge number and that is ok.
	startCRC = strtoul(startCRCString, NULL, 0);

    // Done with this
    free(startCRCString);
    
	// PROCESS CRC
	crc = CalcCRCImpl(arg1->str.ch, arg1->len, startCRC);

    // Done with this
    IRISEXSTRKILL(arg1);
    
    // Transforming crc from unsigned short into a zero terminated string
    sprintf(crcString, "%i", crc);

    // Need to recreate arg2 because it is going to be our return value
    // It must have the exact size of the string
    if (!IRISEXSTRNEW(arg2, strlen(crcString))) {return ZF_FAILURE;}

    // Transform the returned crc to an IRIS String
    memcpy(arg2->str.ch, crcString, arg2->len);

	return ZF_SUCCESS;
}


int TestImpl(CACHE_EXSTRP retorno)
{
    int len;
    unsigned char* tempStr[20]; //20 is more than enough to hold our returning message

    len = retorno->len;

    sprintf((char*)tempStr, "String size: %d", len);

    //Now that we have used the string we received, we MUST destroy it.
    //We could reuse it but the size would be wrong and we would be sending trash trailing data
    //to the caller
    CACHEEXSTRKILL(retorno);

    //CACHE_EXSTRP is no a zero ended string. We must tell exactly how many
    //characters we want. That is why we use a temp buffer to build our string first.
    if (!CACHEEXSTRNEW(retorno, strlen((char*)tempStr))) {return ZF_FAILURE;}

    //Now we can copy the characters from our temp buffer into the structure...
    memcpy(retorno->str.ch, tempStr, retorno->len);

    return ZF_SUCCESS;
}


ZFBEGIN
    ZFENTRY("CalcCRC","1j1J",CalcCRCProxy)
    ZFENTRY("Test","1J",TestImpl)
ZFEND

