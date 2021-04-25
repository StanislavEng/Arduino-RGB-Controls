# Arduino-RGB-Controls

Created Arduino Controls for customizable RGB LED's with some "storage" by way of linked lists. 
Comes predefined with red, blue and green colors, and the option to create new colors by changing
RGB values individually. You can save colors and it'll add on to the list.

I made this for fun while following a tutorial of lighting an RGB light but I wanted more control
than just turning on one of the colors, as well as the options to get more specific values and save 
them for later.

My Friend made the comparison to some of the LED lights you can see as cars which prompted me to 
create a dedicated repository and make more versions.    

This was surprisingly challenging and took a while because I had to figure out not only how to properly set up and dynamically make a linked  
list with structs (I don't like hard coding, even for beginning, probably bad)  
but making the EEPROM read AND write work properly without junk going in and out  
I think one fix was the idea of placements in struct mattering, with putting string at the end

## Version 1: Standard (in Serial atm) user input and printed values.   
Uses linked lists for finding desired colors, as well as the option to change RGB values by the color in +/-63 bit values.  
Stores memory in EEPROM and loads it back up again upon boot  
Currently only stores one at a time because I didn't want to degrade EEPROM too much, but its probably purely on cycles not on memory used  
Will need to add in up to 5 states at a time
### TODO?: Lower the change value for more precise changing
### TODO: Add in loop to allow you to change values until you're happy with it without starting from top  
^ I think I added this in but I need to double check  
### TODO: Add in TO END feature for RGB to go from 0 to 255 or vice versa immediately  

## Version 2: Adding potentiometer for more precise control of RGB values
### TODO: Add in pot ;) 

## Version 3: Adding in DIPSWITCH or something to switch from dev mode to user mode, as well as use other
buttons to switch between changing R,G or B values.
### TODO: start it









### TODO: Create some sort of scrolling feature to reduce how many get shown at a time for better
legibility. 
### ~TODO: Add in EEPROM writing to save data in case you reset Arduino~ got it
### TODO?: Some sort of output such as LCD screens so you don't need to use Arduino IDE(?) Serial
### LOOKAT: Seeing if I can find some sort of lower tech way or something to save RGB values without EEPROM
