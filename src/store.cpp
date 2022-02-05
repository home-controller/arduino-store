/**
 * @file store.h
 * @author Joseph (you@domain.com)
 * @brief To help with very simple data saving betwean MCU resets. For now only using inbuilt EEPROM
 * @version 0.0.1
 * @date 2021-12-28
 *
 * @copyright Copyright (c) 2021
 *
 */
#include "store.h"
#include <EEPROM.h>
#include <ui_io.h>
#include "defs.h"

storageBlock_C::storageBlock_C( byte maxBlocks, storageBlock_T a[ ], byte startIndex ) {
    blocksA = a;
    maxBlocks = maxBlocks;
    eeprom_used = startIndex;
}
storageBlock_C::~storageBlock_C() { }

/**
 * @brief get the storage space size in bytes reserved for the block.
 *
 * @param id The block id/array index.
 * @return byte block size in bytes.
 */
byte storageBlock_C::blockSize( byte id ) {
    return blocksA[ id ].slots * blocksA[ id ].size; //mem for block = number of slots * slot size.
}

/**
 * @brief get block end index. i.e. the address of last byte used by the block.
 * If block size is 0 there isn't a block end so result will be meningless.
 *
 * @param id Block id, index into blocksA[]
 * @return byte
 */
byte storageBlock_C::blockEnd( byte id ) {
    return blocksA[ id ].start + blocksA[ id ].slots * blocksA[ id ].size - 1;
    //mem for block = number of slots * slot size. 
    //mem used before block = blocksA[ id ].start - 1.
    // so: i = (blocksA[ id ].start - 1) + blocksA[ id ].slots * blocksA[ id ].size
    // but as start could be 0 so sub 1 at end.
}

void storageBlock_C::leaveFirst( byte i ) {
    eeprom_used = i;
};

void storageBlock_C::print( byte id ) {
    io_print( " storage start address: " ); io_print_n( blocksA[ id ].start );
    io_print( ", end address: " ); io_print_n( blockEnd( id ) );
    io_print( ", bytes used: " ); io_println_n( blockSize( id ) );
}

void storageBlock_C::printMemUsage() {
#ifdef ATmega328P
    io_println( "MCU = ATmega328P" );
#endif
    io_println( "ATmega328P has 1kByte EEPROM, ATmega1280 and ATmega2560 4KB" );
    //    io_println( "EEPROM use:" );
    io_print( "Storage used:\n\r\tFor vars: " );
    io_print_n( blocksA[ 0 ].start );
    //io_print( "bytes, For switch sender = " );
    //io_println_n( resMemForSwitchCon );
    io_print( "bytes,\n\r\tFor Blocks: " );
    io_println_n( eeprom_used - blocksA[ 0 ].start );
    io_print( "\tTotal EEPROM memory used in bytes = " );
    io_print_n( eeprom_used );
    io_print( "bytes, EEPROM memory left bytes = " );
    io_print_n( ( mem_eeprom - ( eeprom_used ) ) );
    io_print( "bytes, persentage left \u2248 " );//useing this ≈ works to
    io_print_n( ( ( mem_eeprom - ( eeprom_used ) ) * 100ul ) / mem_eeprom ); io_println( "%" );

}

/**
 * @brief Add a storage block.
 * 
 * No range checking
 * 
 * @param slots number of slots to add
 * @param recSize size of each block in bytes.
 * @return byte returns the id of the block.
 */
byte storageBlock_C::addBlock( byte slots, byte recSize ) {
    blocksA[ slotsUsed ].start = eeprom_used;
    blocksA[ slotsUsed ].slots = slots;
    blocksA[ slotsUsed ].size = recSize;
    eeprom_used += slots * recSize;
    slotsUsed++;
    return slotsUsed - 1;
};

byte storageBlock_C::blockStart( byte blockId ) {
    return blocksA[ blockId ].start;
}

byte storageBlock_C::readByte( byte blockId, byte slot, byte offset ) {
    word i;
    i = blocksA[ blockId ].start + ( slot * blocksA[ blockId ].size );
    return EEPROM.read( i );

}

// void *memcpy(void *dst, const void *src, size_t len)

/**
 * @brief Read a var from permanent storage eg. EEPROM into a var/struct, copys mem of size = to the slot size given when addBlock was called.
 *
 * @param[in] blockId the block of storage, ie. the index to the array
 * @param[in] slot The slot to read, mem copy len is = the size of the rec/slot
 * @param[out] r pointer to the memory to copy the slot into. As not using templates need to make sure sizeof(var pointed to by r) >= slot size.
 * @return true if no problems
 * @return false if slot > num of slots in block.
 */
bool storageBlock_C::readSlot( byte blockId, byte slot, void* r ) {
    word i, x;
    //io_print( ", id: " ); io_print_n( blockId );// checked to = 1 for 1-wire.
    //io_print( ", slot: " ); io_print_n( slot );
    //io_print( ", &blocksA:" ); io_print_n( ( word )&blocksA );
    //io_print( ", &this:" ); io_print_n( ( word )this );

    //io_print( ", .slots: " ); io_print_n( blocksA[ blockId ].slots );
    //io_print( ", sizeof(r):");io_print_n( sizeof(r) );
    //io_print( ", r:");io_print_n( (word)r );
    //io_print( ", ( byte* )r:"); io_print_n( (word)(( byte* )r) );

    if ( slot > blocksA[ blockId ].slots ) { return false; }
    i = blocksA[ blockId ].start + ( slot * blocksA[ blockId ].size );
    //io_print( ", i(EEPROM addr): " ); io_print_n( i );
    byte* d = ( byte* )r;
    //io_print( ", d:");io_print_n( (word)d );
    for ( x = 0; x < blocksA[ blockId ].size; x++ ) {
        d[ x ] = EEPROM.read( i + x );
        //io_print( "." );
    }
    return true;
}

/**
 * @brief Write a var into permanent storage eg. EEPROM into a var/struct, copys mem of size = to the slot size given when addBlock was called.
 *
 * @param[in] blockId the block of storage, ie. the index to the array
 * @param[in] slot The slot to update, mem copy len is = the size of the rec/slot
 * @param[out] r pointer to the memory to copy into the slot. As not using templates need to make sure sizeof(var pointed to by r) >= slot size.
 * @return true if no problems
 * @return false if slot > num of slots in block.
 */
bool storageBlock_C::writeSlot( byte blockId, byte slot, void* r ) {
    word i, x;
    if ( slot > blocksA[ blockId ].slots ) { return false; }
    i = blocksA[ blockId ].start + ( slot * blocksA[ blockId ].size );
    byte* d = ( byte* )r;
    for ( x = 0; x < blocksA[ blockId ].size; x++ ) {
        EEPROM.update( i + x, d[ x ] );
    }
    return true;
}

bool storageBlock_C::writeByte( byte blockId, byte slot, byte offset, byte value ){
     word i;
     i = blocksA[ blockId ].start + ( slot * blocksA[ blockId ].size ) + offset;
     EEPROM.update( i,  value);
     return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++ class blockMem_C+++++++===================
bool blockMem_C::readSlot( byte slot, void* r ) {
    //io_print( ", sizeof(memManager):" ); io_print_n( sizeof( memManager ) );
    //io_print( ", memManager:" ); io_print_n( ( word )( &( *memManager ) ) );
    return memManager->readSlot( id, slot, r );
}
