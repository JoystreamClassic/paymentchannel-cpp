/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <paymentchannel/Commitment.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <paymentchannel/RedeemScript.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::TxOut

namespace joystream {
namespace paymentchannel {

    Commitment::Commitment()
        : _value(0) {
    }

    Commitment::Commitment(int64_t value, const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, Coin::RelativeLockTime lockTime)
        : _value(value)
        , _payorPk(payorPk)
        , _payeePk(payeePk)
        , _lockTime(lockTime){
    }

    Commitment::Commitment(int64_t value, const uchar_vector redeemScript)
        : _value(value) {

        // Validate the script
        RedeemScript paychanScript = RedeemScript::deserialize(redeemScript);

        setPayorPk(paychanScript.payorPk());
        setPayeePk(paychanScript.payeePk());
        setLockTime(paychanScript.lockTime());
    }

    Commitment::Commitment(const Commitment & o)
        : Commitment::Commitment(o.value(), o.payorPk(), o.payeePk(), o.lockTime()) {
    }

    Commitment & Commitment::operator=(const Commitment & o) {

        _value = o.value();
        _payorPk = o.payorPk();
        _payeePk =  o.payeePk();
        _lockTime = o.lockTime();

        return *this;
    }

    Coin::P2SHScriptPubKey Commitment::contractOutputScriptPubKey() const {
        return Coin::P2SHScriptPubKey(Coin::RedeemScriptHash::fromRawScript(redeemScript().serialized()));
    }

    Coin::TxOut Commitment::contractOutput() const {
        return Coin::TxOut(_value, contractOutputScriptPubKey().serialize());
    }

    RedeemScript Commitment::redeemScript() const {
        return RedeemScript(_payorPk, _payeePk, _lockTime);
    }

    int64_t Commitment::value() const {
        return _value;
    }

    void Commitment::setValue(int64_t value) {
        _value = value;
    }

    Coin::PublicKey Commitment::payorPk() const {
        return _payorPk;
    }

    void Commitment::setPayorPk(const Coin::PublicKey & payorPk) {
        _payorPk = payorPk;
    }

    Coin::PublicKey Commitment::payeePk() const {
        return _payeePk;
    }

    void Commitment::setPayeePk(const Coin::PublicKey & payeePk) {
        _payeePk = payeePk;
    }

    void Commitment::setLockTime(Coin::RelativeLockTime lockTime) {
        _lockTime = lockTime;
    }

    Coin::RelativeLockTime Commitment::lockTime() const {
        return _lockTime;
    }
}
}
