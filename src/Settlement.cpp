/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Settlement.hpp>
#include <common/RedeemScriptHash.hpp>
#include <common/Bitcoin.hpp> // BITCOIN_DUST_LIMIT
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/CoinNodeData.h> // TxOut

namespace joystream {
namespace paymentchannel {

    Settlement::Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                           const Commitment & commitment,
                           const Coin::Payment & toPayor,
                           const Coin::Payment & toPayee)
        : _includePayee(true)
        , _toPayee(toPayee)
        , _contractOutPoint(contractOutPoint)
        , _commitment(commitment)
        , _toPayor(toPayor) {
    }

    Settlement::Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                           const Commitment & commitment,
                           const Coin::Payment & toPayor)
        : _includePayee(false)
        , _contractOutPoint(contractOutPoint)
        , _commitment(commitment)
        , _toPayor(toPayor) {
    }

    Settlement Settlement::dustLimitAndFeeAwareSettlement(const Coin::typesafeOutPoint & contractOutPoint,
                                                          const Commitment & commitment,
                                                          const Coin::PubKeyHash &payorPkHash,
                                                          const Coin::PubKeyHash &payeePkHash,
                                                          uint64_t paid,
                                                          uint64_t fee) {

        if(paid > (uint64_t)commitment.value())
            throw std::runtime_error("Funds do not cover payment.");

        // If payments cover tx fee and dust limit, then
        // send everything above fee to payee,
        // otherwise, everything goes to miners
        if(paid > (BITCOIN_DUST_LIMIT + fee))
            return Settlement(contractOutPoint,
                              commitment,
                              Coin::Payment(commitment.value() - paid, payorPkHash),
                              Coin::Payment(paid - fee, payeePkHash));
        else
            return Settlement(contractOutPoint,
                              commitment,
                              Coin::Payment(commitment.value() - paid, payorPkHash));

    }

    Coin::Transaction Settlement::unSignedTransaction() const {

        // Build refund transaction
        Coin::Transaction tx;

        // Add payor refund output
        tx.addOutput(_toPayor.txOut());

        // Add (unsigned) input spending contract output
        tx.addInput(Coin::TxIn(_contractOutPoint.getClassicOutPoint(), uchar_vector(), DEFAULT_SEQUENCE_NUMBER));

        // Payee output if
        if(_includePayee)
            tx.addOutput(_toPayee.txOut());

        return tx;
    }

    int64_t Settlement::fee() const {
        return _commitment.value() - (_toPayor.value() + (_includePayee ? _toPayee.value() : 0));
    }

    uchar_vector Settlement::sighash(Coin::SigHashType type) const {

        return Coin::sighash(unSignedTransaction(),
                       _contractOutPoint,
                       _commitment.redeemScript().serialized(),
                       type);
    }

    Coin::TransactionSignature Settlement::transactionSignature(const Coin::PrivateKey & sk) const {

        Coin::SigHashType type = Coin::SigHashType::standard();

        uchar_vector hash = sighash(type);

        return Coin::TransactionSignature(sk.sign(hash), type);
    }

    Coin::Transaction Settlement::signedTransaction(const Coin::TransactionSignature & payorTransactionSignature,
                                                    const Coin::TransactionSignature & payeeTransactionSignature) const {

        // Verify that both have appropriate sighash type
        if(!payorTransactionSignature.type().isStandard())
            throw std::runtime_error("First signature was not of type SIGHASH_ALL type");
        else if(!payeeTransactionSignature.type().isStandard())
            throw std::runtime_error("Second signature was not of type SIGHASH_ALL type");

        // Create (unsigned) transaction
        Coin::Transaction tx = unSignedTransaction();

        // Create input script with signature
        uchar_vector scriptSig;

        scriptSig += payorTransactionSignature.opPushForScriptSigSerialized();
        scriptSig += payeeTransactionSignature.opPushForScriptSigSerialized();
        scriptSig += RedeemScript::PayeeOptionalData();
        uchar_vector redeemScript(_commitment.redeemScript().serialized());
        scriptSig += Coin::opPushData(redeemScript.size());
        scriptSig += redeemScript;

        tx.inputs[0].scriptSig = scriptSig;

        return tx;
    }

    bool Settlement::validate(const Coin::PublicKey & pk, const Coin::Signature & sig) const {

        // Compute sighash for unsigned refund
        uchar_vector h = sighash(Coin::SigHashType::standard());

        // Check if signature is valid
        return pk.verify(h, sig);
    }

    bool Settlement::validatePayorSignature(const Coin::Signature & sig) const {
        return validate(_commitment.payorPk(), sig);
    }

    bool Settlement::validatePayeeSignature(const Coin::Signature & sig) const {
        return validate(_commitment.payeePk(), sig);
    }

}
}
