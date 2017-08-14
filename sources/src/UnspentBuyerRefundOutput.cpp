/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <paymentchannel/UnspentBuyerRefundOutput.hpp>
#include <paymentchannel/RedeemScript.hpp>

namespace joystream {
namespace paymentchannel {

UnspentBuyerRefundOutput::UnspentBuyerRefundOutput(const Coin::KeyPair &keyPair, const RedeemScript &paychanScript, const Coin::typesafeOutPoint &outPoint, uint64_t outputValue)
    : UnspentP2SHOutput(keyPair, paychanScript.serialized(), uchar_vector(), outPoint, outputValue)
{

    //Are we the buyer(payor) or the seller(payee)?
    if(paychanScript.isPayorPublicKey(keyPair.pk())) {
        setOptionalData(RedeemScript::PayorOptionalData());
    } else {
        throw std::runtime_error("key does not match payor public key in payment channel script");
    }
}

uint32_t UnspentBuyerRefundOutput::spendingInputSequenceNumber() const {
    auto paychan = RedeemScript::deserialize(redeemScript());
    return paychan.lockTime().toSequenceNumber();
}


}
}
