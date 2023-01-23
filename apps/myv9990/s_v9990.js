//*****************************************************************************
// PROJECT CONFIG OVERWRITE
//*****************************************************************************

//-- Target MSX machine version (string)
Machine = "1";

//-- List of library modules to build (array)
LibModules = [ "v9990", ...LibModules];

//-- Target program format (string)
// Target = "ROM_48K_ISR";

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
// CustomISR = "V9990";

//-- Add V9990 video-chip extension (boolean)
EmulV9990 = true;
