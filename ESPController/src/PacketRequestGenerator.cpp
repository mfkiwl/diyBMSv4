#include "PacketRequestGenerator.h"


void PacketRequestGenerator::sendCellVoltageRequest() {
  //Read voltage (broadcast) to bank 00
  setPacketAddress(true,0,0);

  //Command 1 - read voltage
  _packetbuffer.command = B00000001;

  //AVR MCUs are little endian (least significant byte first in memory)
  clearmoduledata();

  pushPacketToQueue();
}


void PacketRequestGenerator::sendIdentifyModuleRequest(uint8_t b,uint8_t m)
{
  //Read settings from single module
  setPacketAddress(false,b,m);
  //Command 3 - identify
  _packetbuffer.command = B00000010;

  //AVR MCUs are little endian (least significant byte first in memory)
  clearmoduledata();

  pushPacketToQueue();

}

void PacketRequestGenerator::sendGetSettingsRequest(uint8_t b,uint8_t m) {
  //Read settings from single module
  setPacketAddress(false,b,m);
  //Command 5 - read settings
  _packetbuffer.command = B00000101;

  //AVR MCUs are little endian (least significant byte first in memory)
  clearmoduledata();

  pushPacketToQueue();
}


void PacketRequestGenerator::sendCellTemperatureRequest() {
  //Read voltage (broadcast) to bank 00
  setPacketAddress(true,0,0);
  //Command 3 - read temperatures
  _packetbuffer.command = B00000011;

  //AVR MCUs are little endian (least significant byte first in memory)
  clearmoduledata();

  pushPacketToQueue();
}


void PacketRequestGenerator::pushPacketToQueue() {
  _packetbuffer.crc = uCRC16Lib::calculate((char*)&_packetbuffer, sizeof(packet) - 2);

  _requestq->push(&_packetbuffer);
  
}

void PacketRequestGenerator::setPacketAddress(bool broadcast,uint8_t bank,uint8_t module) {
  if (broadcast) {
    _packetbuffer.address = B10000000;
  } else {
    _packetbuffer.address = ((bank & B00000011)<<4) + (module & B00001111);
  }
}

void PacketRequestGenerator::clearmoduledata() {
  //todo replace with memset/memclr
  for ( int a = 0; a < maximum_cell_modules; a++ ) {
    _packetbuffer.moduledata[a] = __builtin_bswap16(0x0000);
  }
}
