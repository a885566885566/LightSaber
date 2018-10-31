# LightSaber
This project is a light saber based on Arduino Mini. This reposity includes four parts, arduino code, pcb layout file, pcb layout gcode, and sound effect.

## Code
There are two version of the arduino code, the general one and the silence one. 

* General Version: There are two mode, light saber and light gun. 
* Silence Version: We added a new mode, silence saber.

## PCB
We designed the circuit by *easyEDA*, which provided online circuit editor and gerber file generation.

## Gcode
We made lots of efforts to generate the gcode from gerber files, because most CAM software are not open source. In the end, we find *FlatCam*, which provide gcode viewer, milling, routing, double side PCB, etc.

There are serveral complicated step to generate the pcb files.
1. **Generate Gcode From FlatCAM:** Download the flatCAM from http://flatcam.org/. Load the gerber file and output the gcodes, including top_layer, drill and outline.
2. **Translate Gcode To Fit Roland**: Because the gcode we obtained from flatCAM is not compatible with the CNC mechine we used, we need to translate it into a correct format, and write a python script called ```gcode_trans.py```  to do this.

## Sound Effects
These effects should be copied into the SD card, so that arduino can read the HEX audio files.