/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef PAYMENT_CHANNEL_COMMITMENT_HPP
#define PAYMENT_CHANNEL_COMMITMENT_HPP

#include <common/PublicKey.hpp>
#include <paymentchannel/RedeemScript.hpp>

namespace Coin {
    class P2SHScriptPubKey;
    class MultisigScriptPubKey;
    class TxOut;
    class RelativeLockTime;
}

namespace joystream {
namespace paymentchannel {

    class Commitment {

    public:

        // Default constructor
        Commitment();

        // Constructor based on members
        Commitment(int64_t value, const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, Coin::RelativeLockTime lockTime);

        // Constructor from raw p2sh redeem script
        Commitment(int64_t value, const uchar_vector redeemScript);

        // Copy constructor
        Commitment(const Commitment& o);

        // Assignement operator
        Commitment & operator=(const Commitment & o);

        // p2sh 2of2 multisig scriptPubKey controlling contract output
        Coin::P2SHScriptPubKey contractOutputScriptPubKey() const;

        // Generates contract output
        Coin::TxOut contractOutput() const;

        // Multiway paychan script
        RedeemScript redeemScript() const;

        // Getters and setters
        int64_t value() const;
        void setValue(int64_t value);

        Coin::PublicKey payorPk() const;
        void setPayorPk(const Coin::PublicKey & payorPk);

        Coin::PublicKey payeePk() const;
        void setPayeePk(const Coin::PublicKey & payeePk);

        void setLockTime(Coin::RelativeLockTime lockTime);
        Coin::RelativeLockTime lockTime() const;

    private:

        // Funds allocated to output
        int64_t _value;

        // First public key controlling multisig output
        Coin::PublicKey _payorPk;

        // Second public key controlling multisig output
        Coin::PublicKey _payeePk;

        // How long the output is locked before payor can spend it
        Coin::RelativeLockTime _lockTime;
    };

}
}

#endif // PAYMENT_CHANNEL_COMMITMENT_HPP
