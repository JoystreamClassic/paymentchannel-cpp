/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef PAYMENTCHANNEL_UNSPENTCONTRACTOUTPUT_HPP
#define PAYMENTCHANNEL_UNSPENTCONTRACTOUTPUT_HPP

#include <common/UnspentP2SHOutput.hpp>
#include <CoinCore/CoinNodeData.h>

namespace Coin {
    class KeyPair;
    class typesafeOutPoint;
    class UnspentOutput;
}
namespace joystream {
namespace paymentchannel {

    class RedeemScript;

    typedef std::function<Coin::UnspentOutput*(const int &chainType, const Coin::KeyPair &,
                                           const uchar_vector &script, const uchar_vector &optionalData,
                                           const Coin::typesafeOutPoint, const int64_t &value,
                                           bool &processNextSelector)> UnspentBuyerRefundOutputSelector;

// This class is primarily used to to safely construct an UnspentOutput
// for the buyer to finance a transaction to claim a full refund
// It can be constructed from the information stored in the spvwallet for a UTXO
class UnspentBuyerRefundOutput : public Coin::UnspentP2SHOutput {
public:

    UnspentBuyerRefundOutput(const Coin::KeyPair & keyPair, const RedeemScript &redeemScript, const Coin::typesafeOutPoint & outPoint, uint64_t outputValue);

    virtual uint32_t spendingInputSequenceNumber() const;

};

}
}
#endif // PAYMENTCHANNEL_UNSPENTCONTRACTOUTPUT_HPP
