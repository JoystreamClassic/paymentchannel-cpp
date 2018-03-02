#include <paymentchannel/paymentchannel.hpp>

int main() {
  const Coin::Network network = Coin::Network::mainnet;
  joystream::paymentchannel::Payor payor(network);
  joystream::paymentchannel::Payee payee(network);
  return 0;
}
