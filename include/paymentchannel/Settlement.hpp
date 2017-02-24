/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef PAYMENT_CHANNEL_SETTLEMENT_HPP
#define PAYMENT_CHANNEL_SETTLEMENT_HPP

#include <paymentchannel/Commitment.hpp>
#include <common/Payment.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Utilities.hpp>

namespace Coin {
    class Transaction;
    class typesafeOutPoint;
    class TransactionSignature;
    class PrivateKey;
}

namespace joystream {
namespace paymentchannel {

    class Settlement {

    public:

        Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::Payment & toPayor,
                   const Coin::Payment & toPayee);

        Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                   const Commitment & commitment,
                   const Coin::Payment & toPayor);

        // Creates to appropriate settlement given the dust
        // limit and transaction fee
        static Settlement dustLimitAndFeeAwareSettlement(const Coin::typesafeOutPoint & contractOutPoint,
                                                         const Commitment & commitment,
                                                         const Coin::PubKeyHash & payorPkHash,
                                                         const Coin::PubKeyHash & payeePkHash,
                                                         uint64_t paid,
                                                         uint64_t fee);

        // Implicit fee by comparing commitment and (payor + payee) payment
        int64_t fee() const;

        // Transaction signature
        Coin::TransactionSignature transactionSignature(const Coin::PrivateKey & sk) const;

        // Signed transaction with given signatures
        Coin::Transaction signedTransaction(const Coin::TransactionSignature & payorTransactionSignature,
                                            const Coin::TransactionSignature & payeeTransactionSignature) const;

        // Validate signature for unsigned transaction against payor public key
        bool validatePayorSignature(const Coin::Signature & sig) const;

        // Validate signature for unsigned transaction against payee public key
        bool validatePayeeSignature(const Coin::Signature & sig) const;

    private:

        // Unsigned settelment transaction
        Coin::Transaction unSignedTransaction() const;

        //
        uchar_vector sighash(Coin::SigHashType type) const;

        // Validate signature for unsigned transaction against public key
        bool validate(const Coin::PublicKey & pk, const Coin::Signature & sig) const;

        // Whether to pay payor
        // When paid quantity to payee is lower than
        // sum of tx fee and dust limit, then everything
        // is spent to fees, and there is no need to pay payee in that case.
        // *** not sure if this is the best way to model this, but ok for now ***
        bool _includePayee;

        // Payment to payee
        Coin::Payment _toPayee;

        // Contract to which commitment corresponds
        Coin::typesafeOutPoint _contractOutPoint;

        // Commitment being terminated
        Commitment _commitment;

        // Payment back to payor
        Coin::Payment _toPayor;
    };

}
}

#endif // PAYMENT_CHANNEL_SETTLEMENT_HPP

