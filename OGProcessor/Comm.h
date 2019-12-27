#ifndef Comm_h
#define Comm_h

/*
 * Packet structures for sending data across mcus
 */
 /*
namespace COMM {
  enum OPT {
    NOOP,
    MOVE,
    HOME
  };
  
  enum FLAG {
    NONE
  };
  
  struct PACKET {
    OPT opt;
    unsigned int flags;
    unsigned long int value;
    unsigned int steps;
  };
  
  PACKET CreatePacket(OPT opt, unsigned int flags, double value, unsigned int steps) {
    PACKET packet;
    packet.opt = opt;
    packet.flags = flags;
    packet.value = static_cast<unsigned long int>(value);
    packet.steps = steps;
    return packet;
  }
};
*/

#endif
