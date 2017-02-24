/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 12 2016
 */

#include <paymentchannel/Payor.hpp>
#include <paymentchannel/Commitment.hpp>
#include <paymentchannel/Refund.hpp>
#include <paymentchannel/Settlement.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/MultisigScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp> // DEFAULT_SEQUENCE_NUMBER
#include <cassert>
#include <cmath> // ceil

namespace joystream {
namespace paymentchannel {

    Payor::Payor()
        : _price(0)
        , _numberOfPaymentsMade(0)
        , _funds(0)
        , _settlementFee(0)
        , _refundLockTime() {
    }

    Payor::Payor(uint64_t price,
                     uint64_t numberOfPaymentsMade,
                     uint64_t funds,
                     uint64_t settlementFee,
                     Coin::RelativeLockTime refundLockTime,
                     const Coin::typesafeOutPoint & anchor,
                     const Coin::KeyPair & payorContractKeyPair,
                     const Coin::PubKeyHash &payorFinalPkHash,
                     const Coin::PublicKey & payeeContractPk,
                     const Coin::PubKeyHash &payeeFinalPkHash)
        : _price(price)
        , _numberOfPaymentsMade(numberOfPaymentsMade)
        , _funds(funds)
        , _settlementFee(settlementFee)
        , _refundLockTime(refundLockTime)
        , _anchor(anchor)
        , _payorContractKeyPair(payorContractKeyPair)
        , _payorFinalPkHash(payorFinalPkHash)
        , _payeeContractPk(payeeContractPk)
        , _payeeFinalPkHash(payeeFinalPkHash){
    }

    Commitment Payor::commitment() const {

        return Commitment(_funds, _payorContractKeyPair.pk(), _payeeContractPk, _refundLockTime);
    }

    Refund Payor::refund() const {

        return Refund(_anchor, commitment(), _payorContractKeyPair);
    }

    Settlement Payor::settlement() const {

        return Settlement::dustLimitAndFeeAwareSettlement(_anchor,
                                                          commitment(),
                                                          _payorFinalPkHash,
                                                          _payeeFinalPkHash,
                                                          amountPaid(),
                                                          _settlementFee);
    }

    Coin::Signature Payor::generatePayorSettlementSignature() const {

        // Get settelemnt
        Settlement s = settlement();

        // Generate signature
        Coin::TransactionSignature settlementSignature = s.transactionSignature(_payorContractKeyPair.sk());

        return settlementSignature.sig();
    }

    Coin::Signature Payor::makePayment() {

        // Increment payment counter
        _numberOfPaymentsMade++;

        // Generate current payment signature
        return generatePayorSettlementSignature();
    }

    uint64_t Payor::amountPaid() const {
        return _price*_numberOfPaymentsMade;
    }

    Coin::typesafeOutPoint Payor::anchor() const {
        return _anchor;
    }

    void Payor::setAnchor(const Coin::typesafeOutPoint & anchor){

        _anchor = anchor;

    }

    uint64_t Payor::price() const {
        return _price;
    }

    void Payor::setPrice(uint64_t price) {
        _price = price;
    }

    uint64_t Payor::numberOfPaymentsMade() const {
        return _numberOfPaymentsMade;
    }

    void Payor::setNumberOfPaymentsMade(uint64_t numberOfPaymentsMade) {
        _numberOfPaymentsMade = numberOfPaymentsMade;
    }

    uint64_t Payor::funds() const {
        return _funds;
    }

    void Payor::setFunds(uint64_t funds) {
        _funds = funds;
    }

    uint64_t Payor::settlementFee() const {
        return _settlementFee;
    }

    void Payor::setSettlementFee(uint64_t settlementFee) {
        _settlementFee = settlementFee;
    }

    Coin::RelativeLockTime Payor::refundLockTime() const {
        return _refundLockTime;
    }

    void Payor::setRefundLockTime(Coin::RelativeLockTime refundLockTime) {
        _refundLockTime = refundLockTime;
    }

    Coin::KeyPair Payor::payorContractKeyPair() const {
        return _payorContractKeyPair;
    }

    void Payor::setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair) {
        _payorContractKeyPair = payorContractKeyPair;
    }

    Coin::PubKeyHash Payor::payorFinalPkHash() const {
        return _payorFinalPkHash;
    }

    void Payor::setPayorFinalPkHash(const Coin::PubKeyHash & payorFinalPkHash) {
        _payorFinalPkHash = payorFinalPkHash;
    }

    Coin::PublicKey Payor::payeeContractPk() const {
        return _payeeContractPk;
    }

    void Payor::setPayeeContractPk(const Coin::PublicKey & payeeContractPk) {
        _payeeContractPk = payeeContractPk;
    }

    Coin::PubKeyHash Payor::payeeFinalPkHash() const {
        return _payeeFinalPkHash;
    }

    void Payor::setPayeeFinalPkHash(const Coin::PubKeyHash & payeeFinalPkHash) {
        _payeeFinalPkHash = payeeFinalPkHash;
    }

}
}
