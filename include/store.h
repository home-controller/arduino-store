/**
 * @file store.h
 * @author Joseph (you@domain.com)
 * @brief To help with very simple data saving betwean MCU resets. For now only using inbuilt EEPROM
 * @version 0.0.1
 * @date 2021-12-28
 *
 * @copyright Copyright (c) 2021
 *
 * At least for now:
 * 1: a function to call to request a block of storage.
 * 2: This unit should keep track of how much space is used and start index of each block.
 */

#ifndef _Store_h
#define _Store_h

 #include <Arduino.h>
// #include <ui_io.h>
// #include "defs.h"
//#include <EEPROM.h>

/**
 * @brief Storage block TYPE info
 *
 */
typedef struct {
    word start;  // EEprom start index
    byte slots;  //< Number of records to save space for in EEPROM for
    byte size;   ///< sizeof the record in bytes.
}storageBlock_T;

class storageBlock_C {
private:
    uint16_t eeprom_used;
    byte maxBlocks;
    byte slotsUsed = 0;
public:
    storageBlock_T* blocksA;

    //setup
    storageBlock_C( byte maxBlocks, storageBlock_T a[ ], byte startIndex = 0 );
    ~storageBlock_C();
    void leaveFirst( byte i );
    byte addBlock( byte slots, byte recSize );

    // block info.
    byte blockSize( byte id );
    byte blockStart( byte blockId );
    byte blockEnd( byte id );

    // Working with a slot in a block.
    byte getSlots(byte id) {return blocksA[id].slots;}
    byte readByte( byte blockId, byte slot, byte offset );
    bool readSlot( byte blockId, byte slot, void* r );
    bool writeByte( byte blockId, byte slot, byte offset, byte value );
    bool writeSlot( byte blockId, byte slot, void* r );

    void print( byte id );
    void printMemUsage();
};
/**
 * @brief Wrapper for storageBlock_C but working with only 1 block id
 * 
 */
class blockMem_C {// todo . can't be bother right now though.
private:
    byte id;
    storageBlock_C* memManager;
    byte blockStart() { return memManager->blockStart( id ); }
    byte blockEnd() { return memManager->blockEnd( id ); }
public:
    void setId( byte id1 ) { id = id1; }
    byte getId() { return id; }
    void setMemManager( storageBlock_C& o) { memManager = &o; }
    byte blockSize() { return memManager->blockSize( id ); }///mem for block = number of slots * slot size.
    byte readByte( byte slot, byte offset ) { return memManager->readByte( id, slot, offset ); }
    bool readSlot( byte slot, void* r );
    bool writeByte( byte slot, byte offset, byte value  ) { return memManager->writeByte( id, slot, offset, value ); }
    bool writeSlot( byte slot, void* r ){return memManager->writeSlot( id, slot, r );}
    void print() { return memManager->print( id ); }
    byte getSlots(){ return memManager->getSlots( id );;}
};




#define sBlocks(S) static storageBlock_T storageBlocksA[S]; maxStorageBlocks = S;

#endif