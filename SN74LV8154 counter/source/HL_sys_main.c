// include files
#include "HL_sys_common.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_het.h"
#include <stdio.h>

#define GAL 2       // HET1_2
#define GAU 18      // HET1_18
#define GBL 16      // HET1_16
#define GBU 30      // HET1_30

#define RCLK 0      // GIOB_0
#define CCLR 1      // GIOB_1

#define CLK 2       // GIOB_2

char count[20];

void main()
{
    uint32 i, freq = 0;
    uint8 b, b0,b1,b2,b3;

    // Initialize GIO driver
    gioInit();

    // Initialize SCI driver
    sciInit();

    // Initialize HET driver
    hetInit();

    // setting HET1 driver pins as output


    // setting GIOA driver pins as input
    gioSetDirection(gioPORTA, 0x00000000);

    // setting GIOB driver pins as output (with GIOB.3 as input)
    gioSetDirection(gioPORTB, 0xFFFF0FFF);

    while(1)
    {
        // initialization of the register
        gioSetBit(hetPORT1, GAL, 1);
        gioSetBit(hetPORT1, GAU, 1);
        gioSetBit(hetPORT1, GBL, 1);
        gioSetBit(hetPORT1, GBU, 1);
        gioSetBit(gioPORTB, RCLK, 0);
        gioSetBit(gioPORTB, CCLR, 0);
        __delay_cycles(200);
        gioSetBit(gioPORTB, CCLR, 1);
        gioSetBit(gioPORTB, CLK, 0);

        for ( i=0; i<2000000; i++ )
        {
            freq = 0;

            // set CLOCK pulse
            gioSetBit(gioPORTB, CLK, 1);
            __delay_cycles(35);
            gioSetBit(gioPORTB, CLK, 0);

            // set RCLK pulse
            gioSetBit(gioPORTB, RCLK, 1);
            __delay_cycles(11);
            gioSetBit(gioPORTB, RCLK, 0);

            // read GBU byte
            gioSetBit(gioPORTB, GBU, 0);

            // read bits
            b3 = gioGetPort(gioPORTA);
            freq = freq | b3;

            b = gioGetBit(gioPORTB, 3);
            if (b == 0)
            {
                freq = freq & 239;
            }

            // set GBU to HIGH again
            gioSetBit(gioPORTB, GBU, 1);

            // read GBL byte
            gioSetBit(gioPORTB, GBL, 0);

            // read bits
            b2 = gioGetPort(gioPORTA);
            freq = (freq << 8) | b2;

            b = gioGetBit(gioPORTB, 3);
            if (b == 0)
            {
                freq = freq & 239;
            }

            // set GBL to HIGH again
            gioSetBit(gioPORTB, GBL, 1);

            // read GAU byte
            gioSetBit(gioPORTB, GAU, 0);

            // read bits
            b1 = gioGetPort(gioPORTA);
            freq = (freq << 8) | b1;

            b = gioGetBit(gioPORTB, 3);
            if (b == 0)
            {
                freq = freq & 239;
            }

            // set GAU to HIGH again
            gioSetBit(gioPORTB, GAU, 1);

            // read GAL byte
            gioSetBit(gioPORTB, GAL, 0);

            // read bits
            b0 = gioGetPort(gioPORTA);
            freq = (freq << 8) | b0;

            b = gioGetBit(gioPORTB, 3);
            if (b == 0)
            {
                freq = freq & 239;
            }

            // set GAL to HIGH again
            gioSetBit(gioPORTB, GAL, 1);

            printf("%d\r\n", freq);
            //sciSend(sciREG1, sizeof(count), count);
        }
    }
}
