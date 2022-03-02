/* Include Files */

#include "HL_sys_common.h"
#include "HL_sci.h"
#include "HL_adc.h"
#include <stdlib.h>

unsigned char command[8];

int main(void)
{
    adcData_t adc_data;                                             // ADC Data Structure
    adcData_t *adc_data_ptr = &adc_data;                            // ADC Data Pointer
    unsigned int NumberOfChars, value;

    sciInit();                                                      // Initialize the SCI Module
    adcInit();                                                      // Initialize the ADC Module
    while(1)                                                        // Infinite Loop for continuous operation
    {
        adcStartConversion(adcREG1, adcGROUP1);                     // Start ADC Conversion
        while(!adcIsConversionComplete(adcREG1, adcGROUP1));        // Wait till ADC Conversion is complete
        adcGetData(adcREG1, 1U, adc_data_ptr);                      // Store conversion into ADC Data Pointer
        value = (unsigned int)adc_data_ptr -> value;                // Store the value in the value variable
        NumberOfChars= ltoa(value, (char *)command);                // Convert value into string data type and store in command
        sciSend(sciREG1, NumberOfChars, command);                   // Send the Potentiometer Value
        sciSend(sciREG1, 2, (unsigned char *)"\r\n");               // Send new line character
    }
}
