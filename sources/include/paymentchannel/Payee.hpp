/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP
#define JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/RelativeLockTime.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class Transaction;
    class P2SHScriptPubKey;
    class TxOut;
    class RelativeLockTime;
}

namespace joystream {
namespace paymentchannel {

    class Commitment;
    class Refund;
    class Settlement;

    /**
     * Manages the payee side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/JoyStream/CBEP
     */
    class Payee {

    public:

        Payee();

        Payee(uint64_t numberOfPaymentsMade,
              Coin::RelativeLockTime lockTime,
              uint64_t price,
              uint64_t funds,
              uint64_t settlementFee,
              const Coin::typesafeOutPoint & contractOutPoint,
              const Coin::KeyPair & payeeContractKeys,
              const Coin::PubKeyHash & payeeFinalPkHash,
              const Coin::PublicKey & payorContractPk,
              const Coin::PubKeyHash & payorFinalPkHash,
              const Coin::Signature & lastValidPayorPaymentSignature);

        // Attempts to register payment if signature is valid
        // ==================================================
        // A valid signature will lead to an increase of _numberOfPaymentsMade,
        // and storing signature in _lastValidPayorPaymentSignature
        bool registerPayment(const Coin::Signature &);

        // Generates transaction for last payment
        Coin::Transaction lastPaymentTransaction() const;

        // Checks contract validitity
        // ==========================
        // 1) There is tx with _contractOutputPoint in longest chain
        // 2) tx has contract output point
        // 3) scriptSig of tx is controlled by given keys and has the correct quanitity of funds
        // 4) channel has to correct number of participants.
        bool isContractValid(const Coin::Transaction &) const;

        // Commitment
        Commitment commitment() const;

        // Settlement
        Settlement settlement(int) const;

        // Amount of funds paid
        uint64_t amountPaid() const;

        // Getters and setters
        uint64_t numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(uint64_t);

        Coin::RelativeLockTime lockTime() const;
        void setLockTime(Coin::RelativeLockTime);

        uint64_t price() const;
        void setPrice(uint64_t);

        uint64_t funds() const;
        void setFunds(uint64_t);

        uint64_t settlementFee() const;
        void setSettlementFee(uint64_t);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint &);

        Coin::KeyPair payeeContractKeys() const;
        void setPayeeContractKeys(const Coin::KeyPair &);

        Coin::PubKeyHash payeeFinalPkHash() const;
        void setPayeeFinalPkHash(const Coin::PubKeyHash &);

        Coin::PublicKey payorContractPk() const;
        void setPayorContractPk(const Coin::PublicKey &);

        Coin::PubKeyHash payorFinalPkHash() const;
        void setPayorFinalPkHash(const Coin::PubKeyHash &);

        Coin::Signature lastValidPayorPaymentSignature() const;
        void setLastValidPayorPaymentSignature(const Coin::Signature &);

    private:

        // The number of payments which have been successfully made
        uint64_t _numberOfPaymentsMade;

        // Payment channel lock time
        Coin::RelativeLockTime _lockTime;

        // Price (#satoshies) increment per payment
        uint64_t _price;

        // Amount (#satoshies) assigned to contract output
        uint64_t _funds;

        // Amount (#satoshies) used in fee for settlement
        uint64_t _settlementFee;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Controls payee portion of contract output
        Coin::KeyPair _payeeContractKeys;

        // Controls payee output in payment _lastValidPaymentSignature
        Coin::PubKeyHash _payeeFinalPkHash;

        // Payor key in contract output
        Coin::PublicKey _payorContractPk;

        // Payor P2SH output in refund/payment
        Coin::PubKeyHash _payorFinalPkHash;

        // The last valid payment signature received, corresponds to _numberOfPaymentsMade
        Coin::Signature _lastValidPayorPaymentSignature;

    };

}
}

#endif // JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP
