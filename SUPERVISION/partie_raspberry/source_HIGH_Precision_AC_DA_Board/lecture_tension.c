#include <stdio.h>

#include "ADS1256.h"


/*
*********************************************************************************************************
*	name: main
*	function:  
*	parameter: NULL
*	The return value:  NULL
*********************************************************************************************************
*/

int  main()
{
	/*
		Ce code est une adaptation du code original destiné à pourvoir etre lancer sous python
	*/
	uint8_t id;
	int32_t adc[8];
	int32_t volt[8];
	uint8_t i;
	uint8_t ch_num;
	int32_t iTemp;
	uint8_t buf[3];

	if (!bcm2835_init())
		return 1;

	//Initialisation:
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST );      
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024); 
	bcm2835_gpio_fsel(SPICS, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(SPICS, HIGH);
	bcm2835_gpio_fsel(DRDY, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(DRDY, BCM2835_GPIO_PUD_UP);    	

	//ADS1256_WriteReg(REG_MUX,0x01);
	//ADS1256_WriteReg(REG_ADCON,0x20);
	// ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
	id = ADS1256_ReadChipID();

	//decommenter pour le debug:
	/*
	printf("\r\n");
	printf("ID=%d\r\n",id);  

	if (id != 3)
	{
		printf("Error, ASD1256 Chip ID = 0x%d\r\n", (int)id);
	}
	else
	{
		printf("Ok, ASD1256 Chip ID = 0x%d\r\n", (int)id);
	}
	*/


	ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_15SPS);
	ADS1256_StartScan(0);
	ch_num = 8;	

	//if (ADS1256_Scan() == 0)
	//{
	//continue;
	//}

	int k;
	for(k=0;k<ch_num;k++){
		while((ADS1256_Scan() == 0));
		for (i = 0; i < ch_num; i++)
		{
		adc[i] = ADS1256_GetAdc(i);
		volt[i] = (adc[i] * 100) / 167;	
		}

		bsp_DelayUS(100000);	
	}
	for (i = 0; i < ch_num; i++)
	{
		iTemp = volt[i];	/* uV  */
		if (iTemp < 0)
		{
			iTemp = -iTemp;
			printf("-%ld.%03ld%03ld\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);
		}
		else
		{
			printf("%ld.%03ld%03ld\n", iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);                    
		}
	}



	bcm2835_spi_end();
	bcm2835_close();

	
    return 0;
}
