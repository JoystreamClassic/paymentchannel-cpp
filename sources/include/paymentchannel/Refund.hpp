/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef PAYMENTCHANNEL_REFUND_HPP
#define PAYMENTCHANNEL_REFUND_HPP

#include <paymentchannel/Commitment.hpp>
#include <common/UnspentP2SHOutput.hpp>

namespace Coin {
    class Transaction;
    class TransactionSignature;
    class PrivateKey;
    class Payment;
}

namespace joystream {
namespace paymentchannel {

    class Commitment;
    class UnspentBuyerRefundOutput;

    class Refund {

    public:

        Refund(const Coin::typesafeOutPoint & contractOutPoint,
               const Commitment & commitment, const Coin::KeyPair &payorContractKeyPair);

        // Get an UnspentContractOutput to spend the entire commitment (full refund)
        UnspentBuyerRefundOutput* getUnspentOutput() const;

        // Wether refund is still locked or not
        bool isLocked(uint32_t currentTime, uint32_t contractMinedAt) const;

        // Whene the refund is unlocked
        uint32_t lockedUntil(uint32_t contractMinedAt) const;

    private:

        // Contract to which commitment corresponds
        Coin::typesafeOutPoint _contractOutPoint;

        // Commitment being refunded
        Commitment _commitment;

        // KeyPair of payor that controls the payment channel output
        Coin::KeyPair _payorContractKeyPair;
    };

}
}

#endif // PAYMENTCHANNEL_REFUND_HPP

