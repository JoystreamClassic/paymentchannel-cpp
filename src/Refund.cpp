/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Refund.hpp>
#include <paymentchannel/UnspentBuyerRefundOutput.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER, sighash
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <ctime>

namespace joystream {
namespace paymentchannel {

    Refund::Refund(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::KeyPair & payorContractKeyPair)
        : _contractOutPoint(contractOutPoint)
        , _commitment(commitment)
        , _payorContractKeyPair(payorContractKeyPair) {
    }

    UnspentBuyerRefundOutput* Refund::getUnspentOutput() const {

        return new UnspentBuyerRefundOutput(_payorContractKeyPair,
                                     _commitment.redeemScript(),
                                     _contractOutPoint,
                                     _commitment.value());
    }

    uint32_t Refund::lockedUntil(uint32_t contractMinedAt) const {
        return _commitment.lockTime().getDuration().count() + contractMinedAt;
    }

    bool Refund::isLocked(uint32_t currentTime, uint32_t contractMinedAt) const {
        uint32_t unlockedAt = lockedUntil(contractMinedAt);

        return currentTime < unlockedAt;
    }

}
}
