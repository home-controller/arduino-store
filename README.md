Main Page for Arduino storage.
============

### For storing setup variables etc. to more permanent storage like eeprom, flash etc.

* This is a work in progress.
* I am getting the parts working as I need then for my home auto projects.
* I Am using this to store the room, 1-wire setup etc.
Not much more than using EEPROM directly but should be less likely to make address errors 
and be easier to change the storage type to flash SD card etc.

- [x] incomplete
- [ ] single variable storage.
- [X] mega328p eeprom blocks of storage split into slots of same length.
- [x] Think blocks of storage slots working for eprom on the mega328p
- [ ] RP2040 flash.
- [ ] complete