/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <paymentchannel/RedeemScript.hpp>
#include <common/RelativeLockTime.hpp>
#include <common/PublicKey.hpp>
#include <common/Utilities.hpp>

namespace joystream {
namespace paymentchannel {

RedeemScript::RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, const Coin::RelativeLockTime &relativeLockTime)
    : _payorPk(payorPk)
    , _payeePk(payeePk)
    , _lockTime(relativeLockTime) {
}

uchar_vector RedeemScript::serialized() const {
    uchar_vector script;

    script.push_back(0x63); // OP_IF
    // Branch for when channel is settled with payment and refund
    script += Coin::opPushData(Coin::PublicKey::compressedLength());
    script += _payeePk.toCompressedRawVector();
    script.push_back(0xad); // OP_CHECKSIGVERIFY

    script.push_back(0x67); // OP_ELSE
    // Branch for when channel is settled with full refund to payor
    script += _lockTime.toScriptData();
    script.push_back(0xb2); // OP_CHECKSEQUENCEVERIFY (BIP 68)
    script.push_back(0x75); // OP_DROP

    script.push_back(0x68); // OP_ENDIF
    // Check that payor has agreed to this spend
    script += Coin::opPushData(Coin::PublicKey::compressedLength());
    script += _payorPk.toCompressedRawVector();
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}

RedeemScript RedeemScript::deserialize(const uchar_vector & script) {
    // check size (minimum size when locktime is encoded as a single byte)
    if(script.size() < 76) {
        throw std::runtime_error("redeem script too short");
    }

    // get a subscript starting at expected payee public key push data operation
    uchar_vector subscript(script.begin() + 1, script.end());

    uchar_vector rawPayeePk;

    subscript = Coin::popData(subscript, rawPayeePk);

    if(rawPayeePk.size() != Coin::PublicKey::compressedLength()) {
        throw std::runtime_error("Unable to retreive payee public key from redeem script");
    }

    Coin::PublicKey payeePk = Coin::PublicKey::fromCompressedRaw(rawPayeePk);

    // get a subscript to the start of the locktime push data operation
    subscript = uchar_vector(subscript.begin() + 2, subscript.end());

    uchar_vector rawLockTime;

    subscript = Coin::popData(subscript, rawLockTime);

    Coin::RelativeLockTime locktime = Coin::RelativeLockTime::fromScriptData(rawLockTime);

    // get a subscript to the start of the payor publick key push data operation
    subscript = uchar_vector(subscript.begin() + 3, subscript.end());

    uchar_vector rawPayorPk;
    subscript = Coin::popData(subscript, rawPayorPk);

    if(rawPayorPk.size() != Coin::PublicKey::compressedLength()) {
        throw std::runtime_error("Unable to retreive payor public key from redeem script");
    }

    Coin::PublicKey payorPk = Coin::PublicKey::fromCompressedRaw(rawPayorPk);

    RedeemScript result(payorPk, payeePk, locktime);

    if(result.serialized() == script) {
        return result;
    }

    throw std::runtime_error("script is not a valid paychan script");

}

uchar_vector RedeemScript::PayorOptionalData() {
    return Coin::opPushNumber(0); // false
}

uchar_vector RedeemScript::PayeeOptionalData() {
    return Coin::opPushNumber(1); // true
}

bool RedeemScript::isPayorPublicKey(const Coin::PublicKey & pk) const {
    return _payorPk == pk;
}

bool RedeemScript::isPayeePublicKey(const Coin::PublicKey & pk) const {
    return _payeePk == pk;
}

Coin::RelativeLockTime RedeemScript::lockTime() const {
    return _lockTime;
}

Coin::PublicKey RedeemScript::payorPk() const {
    return _payorPk;
}

Coin::PublicKey RedeemScript::payeePk() const {
    return _payeePk;
}

}}
