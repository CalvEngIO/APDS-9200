/*
 * FILENAME:	APDS9200.cpp
 * AUTHOR:	Trevor D. Meyer
 * EMAIL:	mail2trevorm@gmail.com
 * VERSION:	0.0
 *
 *
 * AFFILIATIONS
 * Calvary Engineering Family Group, USA
 *	A group of electronics and science enthusiasts who enjoy building things
 *
 *
 * DESCRIPTION
 * This source file provides function definitions for controlling the APDS9200.
 * This is a very basic code that allows the user to declare a Light Sensor object
 * and programmatically read UV and ambient light readings along with adjusting 
 * settings of the chip. This library utilizes high level Arduino functions.
 *
 * UPDATES
 * Version 0.0
 * 2017/07/04:2000>
 *			Started writing the code. 
 *
 *
 * DISCLAIMER
 * This code is in the public domain. Please feel free to use, modify, distribute,
 * etc. as needed, but please give reference to original author as a courtesy to
 * open source developing/-ers.
 *
 * If you find any bugs in the code, or have any questions, please feel free to
 * contact me.
 */
 
 // OVERALL QUESTIONS
 // Not sure how to handle Adjustable Measurement Rate
 // Not sure how to handle Adjustable Gain
 // Still need to find a way to convert raw data to wavelength?
 //   (or is that what the sensor gives me?)
 
 #include "APDS9200.h"
 
 long read3byte();
 
 //Constructors
 //*****************************************************************************
 
 
 //Default Constructor
 APDS9200::APDS9200()
 {
 }
 
 //Light(int intrpPin)
 //This method is a constructor that allows the user to set the pin
 //that will be connected to the chip interrupt functionality
 APDS9200::APDS9200(int intrpPin)
 {
	 interruptPin = intrpPin;
 }
 
 
 //Public Functions
 //*****************************************************************************
 
 
 //--------------------------------------------------------------------------------------------
 // Functions Dealing with Light Sensing
 //--------------------------------------------------------------------------------------------
 
void APDS9200::enableLight() const
{
    // Enable and set chip to read UV Light
    Wire.beginTransmission(APDS9200_ADDRESS);
    Wire.write(APDS9200_REGISTER_MAIN_CTRL);
    Wire.write(0x02);  //Sets value 00000010
    Wire.endTransmission();
}


void APDS9200::enableUV() const
{
    // Enable and set chip to read UV Light
    Wire.beginTransmission(APDS9200_ADDRESS);
    Wire.write(APDS9200_REGISTER_MAIN_CTRL);
    Wire.write(0x0A);  //Sets value 00001010
    Wire.endTransmission();
}
 
 
 //int standby()
 //puts the chip in standby mode to reduce energy consumption
 void APDS9200::standby()
 {
	// Put chip in standby
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_MAIN_CTRL);
	 Wire.write(0x08);  //Sets value 00001000
	 Wire.endTransmission();
 }
 
 
 //long readLight()
 //Reads a single ambint light measurement raw value (filtered by chip logic)
 long APDS9200::getLight()
 {
	 // Reads chip mode to assure it is in the correct mode
	 // If incorrect mode is detected, returns zero
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_MAIN_CTRL);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 1);
	 long chipMode = Wire.read();
	 
	 if (chipMode & 0x02 != 0) //ERROR, Not in ambient light mode or not enabled
		 return 0;
	 
	// FIXME Do I need a wait time for it to take a measuerment????
	// Most important if the chip was just enables/switched modes???
	// Note: Default measurement rate is 100 ms, goes up to 2000 ms
	
	// Reads light value
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_ALS_DATA_0);
     Wire.endTransmission();
     
	 Wire.requestFrom(APDS9200_ADDRESS, 3);
	 
	 long light;
	 
	 if ( Wire.available() )
		 light = read3byte();
	 else
		 return 0;
	 
	 //Wire.endTransmission();
	 return light;
	 
	 /* according to datasheet, transmission should go as follows:
	  * Save Addr (Wrtie) - Address - Slave Addr (Read) - *read data* - Stop
	  * I think this is what the above does
	  */
 }
 
 //long readLight()
 //Reads a single UV light measurement raw value (filtered by chip logic)
 long APDS9200::getUV()
 {
	 // Reads chip mode to assure it is in the correct mode
	 // If incorrect mode is detected, returns zero
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_MAIN_CTRL);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 1);
	 long chipMode = Wire.read();
	 
	 if (chipMode & 0x0A != 0) //ERROR, Not in UV light mode or not enabled
		 return 0;
	 
	// FIXME Do I need a wait time for it to take measurements????
	// Note: Default measurement rate is 100 ms, goes up to 2000 ms
	
	// Reads UV Light value
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_UVS_DATA_0);
     Wire.endTransmission();
     
	 Wire.requestFrom(APDS9200_ADDRESS, 3);
	 
	 long light;
	 
	 if ( Wire.available() )
		 light = read3byte();
	 else
		 return 0;
	 
	 //Wire.endTransmission();
	 return light;
 }
 
 //long readLightUnfiltered()
 //THIS FUNCTION WILL NOT WORK AS WELL AS THE UV-UNFILTERED READING
 //DUE TO THE LACK OF BLOCK READ AVAILABILITY FOR THE TWO REGISTERS
 //Reads a single ambint light measurement unfiltered raw value
 //The filtered value and offset are red, then the offset is added
 //back into the read value.
 long APDS9200::getLightUnfiltered()
 {
	 // Reads chip mode to assure it is in the correct mode
	 // If incorrect mode is detected, returns zero
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_MAIN_CTRL);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 1);
	 long chipMode = Wire.read();
	 
	 if (chipMode & 0x02 != 0) //ERROR, Not in ambient light mode or not enabled
		 return 0;
	 
	// FIXME Do I need a wait time????
	// Note: Default measurement rate is 100 ms, goes up to 2000 ms
		
	// Reads Light Value
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_ALS_DATA_0);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 3);
	 
	 long filtered_data, comp;
	 
	 if ( Wire.available() )
		 filtered_data = read3byte();
	 else
		 return 0;
	 
	 Wire.endTransmission();
	 
	// Reads offset value
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_COMP_DATA_0);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 3);
	 
	 if ( Wire.available() )
		 comp = read3byte();
	 
	 else
		 return 0;
	 
	 Wire.endTransmission();
	 
	 return filtered_data + comp;
 }
 
 
 //long readUVUnfiltered()
 //Reads a single UV light measurement unfiltered raw value
 //The filtered value and offset are red, then the offset is added
 //back into the read value.
 long APDS9200::getUVUnfiltered()
 {
	 // Reads chip mode to assure it is in the correct mode
	 // If incorrect mode is detected, returns zero
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_MAIN_CTRL);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 1);
	 long chipMode = Wire.read();
	 
	 if (chipMode & 0x0A != 0) //ERROR, Not in UV light mode or not enabled
		 return 0;
	 
	// FIXME Do I need a wait time????
	// Note: Default measurement rate is 100 ms, goes up to 2000 ms
	
	// Reads UV Light Value
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_UVS_DATA_0);
	 Wire.endTransmission();
	 
	 Wire.requestFrom(APDS9200_ADDRESS, 6);
	 
	 long filtered_data, comp;
	 
	 if ( Wire.available() )
	 {
		 //Blockreads these values because they are sequential
		 filtered_data = read3byte();
		 comp          = read3byte();
	 } else
		 return 0;
	 Wire.endTransmission();
	 
	 return filtered_data + comp;
 }
 
 
 //--------------------------------------------------------------------------------------------
 // Functions Dealing with Interrupt by Threshold
 //--------------------------------------------------------------------------------------------
  
 //enableLightInterrupt()
 //Configures chip to monitor interrupt from a light sensor
 //threshold value.  Sets enable bit to enable interrupt pin
 void APDS9200::enableLightInterrupt()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x14); //Writes the value 00010100
	 Wire.endTransmission();
 }
 
 //disableLightInterrupt()
 //Disables interrupt pin
 //Intentded to be used with interrupt set in light sensor threshold mode
 void APDS9200::disableLightInterrupt()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x10); //Writes the value 00010000
	 Wire.endTransmission();
 }
 
 //enableUVInterrupt()
 //Configures chip to monitor interrupt from a UV
 //threshold value.  Sets enable bit to enable interrupt pin
 void APDS9200::enableUVInterrupt()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x34); //Writes the value 00110100
	 Wire.endTransmission();
 }
 
 //disableUVInterrupt()
 //Disables interrupt pin
 //Intentded to be used with interrupt set in UV threshold mode
 void APDS9200::disableUVInterrupt()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x30); //Writes the value 00110000
	 Wire.endTransmission();
 }
 
 //setInterruptPersistence(int numReadings)
 //This sets how many consecutive values out of range must occur 
 //before the interrupt is triggered.  Valid values include 0-15.
 //Returns 1 if successful and 0 if an error occured.
 // Ex. If numReadings = 1 then one out of range value does nothing,
 // and every 2nd consecutive out of range value asserts an interrupt.
 //Default Persistence is Zero
 //  (every value out of range asserts an interrupt)
 boolean setInterruptPersistence(int numReadings)
 {	 
	 if (numReadings < 16 && numReadings >= 0)
	 {	 
		 uint8_t pers = (uint8_t) numReadings;
		 
		 Wire.beginTransmission(APDS9200_ADDRESS);
		 Wire.write(APDS9200_REGISTER_INT_PERSISTNECE);
		 Wire.write( (pers << 4) );
		 Wire.endTransmission();
		 return 1;
	 }
	 else
		 return 0;
 }
 
 //setLightInterrupt(long upper, long lower)
 //This function only sets thresh values, the correct interrupt mode
 //must be set and enabled or disabled separately with above functions
 //This function sets the upper and lower thersholds that define
 //what values will trigger the interrupt pin to be asserted.
 boolean APDS9200::setInterruptThresh(long upper, long lower)
 {
	 //Check that valid limits were requested
	 if ((upper > 1048575) || (lower < 0))
		 return 0;
	 
	 uint8_t ULSB = (uint8_t) ( upper & 0xFF);  // Grabs the 8 most least sig bits
	 uint8_t UITB = (uint8_t) ((upper & 0xFF00  ) >> 8); // 9-16th most least sig bits
	 uint8_t UMSB = (uint8_t) ((upper & 0x0F0000) >> 16);// last 17-20 most least sig bits
	 
	 uint8_t LLSB = (uint8_t) ( lower & 0xFF);  // Grabs the 8 most least sig bits
	 uint8_t LITB = (uint8_t) ((lower & 0xFF00  ) >> 8); // 9-16th most least sig bits
	 uint8_t LMSB = (uint8_t) ((lower & 0x0F0000) >> 16);// last 17-20 most least sig bits
	 
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_LS_THRES_UP_0);
	 Wire.write(ULSB);
	 Wire.write(UITB);
	 Wire.write(UMSB);
	 Wire.write(LLSB);
	 Wire.write(LLSB);
	 Wire.write(LLSB);
	 Wire.endTransmission();

	 return 1;
 }
 
 //setLightInterruptUpper(long upper)
 //This function only sets thresh values, the correct interrupt mode
 //must be set and enabled or disabled separately with above functions
 //This function sets the upper thersholds that define
 //what values will trigger the interrupt pin to be asserted.
 //The lower threshold is set to zero.
 boolean APDS9200::setInterruptThreshUpper(long upper)
 {
	 //Check that valid limits were requested
	 if (upper > 1048575)
		 return 0;
	 
	 uint8_t LSB = (uint8_t) ( upper & 0xFF);  // Grabs the 8 most least sig bits
	 uint8_t ITB = (uint8_t) ((upper & 0xFF00  ) >> 8); // 9-16th most least sig bits
	 uint8_t MSB = (uint8_t) ((upper & 0x0F0000) >> 16);// last 17-20 most least sig bits
	 
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_LS_THRES_UP_0);
	 Wire.write(LSB);
	 Wire.write(ITB);
	 Wire.write(MSB);
	 //FIXME Better to leave the lower threshold as is rather than overwrite it?
	 Wire.write(0x00);
	 Wire.write(0x00);
	 Wire.write(0x00);
	 Wire.endTransmission();
	 
	 return 1;
 }
 
 //setLightInterruptLower(long lower)
 //This function only sets thresh values, the correct interrupt mode
 //must be set and enabled or disabled separately with above functions
 //This function sets the lower thersholds that define
 //what values will trigger the interrupt pin to be asserted.
 //The upper threshold is set to the max value.
 boolean APDS9200::setInterruptThreshLower(long lower)
 {
	 //Check that valid limits were requested
	 if (lower < 0)
		 return 0;
	 
	 uint8_t LSB = (uint8_t) ( lower & 0xFF);  // Grabs the 8 most least sig bits
	 uint8_t ITB = (uint8_t) ((lower & 0xFF00  ) >> 8); // 9-16th most least sig bits
	 uint8_t MSB = (uint8_t) ((lower & 0x0F0000) >> 16);// last 17-20 most least sig bits
	 
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_LS_THRES_UP_0);
	 // Better to leave the upper threshold as is rather than overwrite it?
	 Wire.write(0xFF);
	 Wire.write(0xFF);
	 Wire.write(0x0F);
	 Wire.write(LSB);
	 Wire.write(ITB);
	 Wire.write(MSB);
	 Wire.endTransmission();
	 
	 return 1;
 }

 //--------------------------------------------------------------------------------------------
 // Functions Dealing with Interrupt by Variance
 //--------------------------------------------------------------------------------------------
 
 //enableLightInterruptVar()
 //Configures chip to monitor interrupt from a monitoring
 //Light variance values.  Sets enable bit to enable interrupt pin
 void APDS9200::enableLightInterruptVar()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x1C); //Writes the value 00011100
	 Wire.endTransmission();
 }
 
 //disableLightInterruptVar()
 //Disables interrupt pin
 //Intentded to be used with interrupt set in Light variance mode
 void APDS9200::disableLightInterruptVar()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x18); //Writes the value 00011000
	 Wire.endTransmission();
 }
  
 //enableUVInterruptVar()
 //Configures chip to monitor interrupt from a monitoring
 //UV variance values.  Sets enable bit to enable interrupt pin
 void APDS9200::enableUVInterruptVar()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x3C); //Writes the value 00111100
	 Wire.endTransmission();
 }
 
 //disableUVInterruptVar()
 //Disables interrupt pin
 //Intentded to be used with interrupt set in UV variance mode
 void APDS9200::disableUVInterruptVar()
 {
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_INT_CFG);
	 Wire.write(0x38); //Writes the value 00111000
	 Wire.endTransmission();
 }
 
 //setUVInterruptVar()
 //This function only sets variance values, the correct interrupt mode
 //must be set and enabled or disabled separately with above functions
 //This function sets the variance level that will trigger the
 //Threshold pin to be asserted.  See below for examples.
 //
 //VAR must be set as a three bit number
 //
 //Examples:
 //  000: if data varries by  8 counts compared to previous result
 //  001: if data varries by 16 counts compared to previous result
 //  010: if data varries by 32 counts compared to previous result
 //  011: if data varries by 64 counts compared to previous result
 //  111: if data varries by 1024 counts compared to previous result
 
 boolean APDS9200::setLightInterruptVar(int var)
 {
	 if ((var > 1024) || (var < 0))
		 return 0;
	 
	 Wire.beginTransmission(APDS9200_ADDRESS);
	 Wire.write(APDS9200_REGISTER_LS_THRES_VAR);
	 Wire.write( ((uint8_t) var & 0x07) ); //Pulls the last 3 sig bits
	 Wire.endTransmission();
	 return 1;
 }
 
 
 //--------------------------------------------------------------------------------------------
 // Higher Level Functions
 //--------------------------------------------------------------------------------------------
 
 int APDS9200::getUVIndex()
 {
	 long UV = this->getUV();
	 // Is there a way to do this? Not sure how to define the object calling it
	 // from inside the class itself.  I found this syntax online but I'm not
	 // sure if it's completely right.
	 
	 
	 if (UV < 275)
		 return 1;
	 else if (UV < 525)
		 return 2;
	 else if (UV < 775)
		 return 3;
	 else if (UV < 1050)
		 return 4;
	 else if (UV < 1325)
		 return 5;
	 else if (UV < 1675)
		 return 6;
	 else
		 return 7;
 }
 
 
 // Private Functions
 //*****************************************************************************
 
 long APDS9200::read3byte()
 {
    long x = Wire.read();
    long y = Wire.read() << 8;
    long z = Wire.read() << 16;
    
    return ( x | y | z );    
 }
