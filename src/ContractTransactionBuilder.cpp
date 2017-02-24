/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 20 2016
 */

#include <paymentchannel/ContractTransactionBuilder.hpp>
#include <paymentchannel/Commitment.hpp>
#include <common/Payment.hpp>
#include <common/Utilities.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <common/P2SHScriptPubKey.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/UnspentOutputSet.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <cmath>

namespace joystream {
namespace paymentchannel {

ContractTransactionBuilder::ContractTransactionBuilder() {
}

boost::optional<Coin::UnspentOutputSet> ContractTransactionBuilder::funding() const {
    return _funding;
}

void ContractTransactionBuilder::setFunding(const boost::optional<Coin::UnspentOutputSet> &funding) {
    _funding = funding;
}

ContractTransactionBuilder::Commitments ContractTransactionBuilder::commitments() const {
    return _commitments;
}

void ContractTransactionBuilder::setCommitments(const Commitments &commitments) {
    _commitments = commitments;
}

boost::optional<Coin::Payment> ContractTransactionBuilder::change() const {
    return _change;
}

void ContractTransactionBuilder::setChange(const boost::optional<Coin::Payment> &change) {
    _change = change;
}

Coin::Transaction ContractTransactionBuilder::transaction() const {

    // Create transaction
    Coin::Transaction transaction;

    // Iterate channels and generate outputs
    for(const Commitment & c : _commitments)
        transaction.addOutput(c.contractOutput());

    // Add change output if set
    if(_change.is_initialized())
        transaction.addOutput(_change.get().txOut());

    // Add funding if set
    if(_funding.is_initialized())
        _funding.get().finance(transaction, Coin::SigHashType::standard());

    return transaction;
}

uint64_t ContractTransactionBuilder::totalFee(uint32_t numberOfCommitments, bool hasChange, uint64_t feePerKb, uint64_t sizeOfAllInputs) {

    // Sizeof transaction
    uint64_t txByteSize = transactionSize(numberOfCommitments, hasChange) + sizeOfAllInputs;

    // Seed on fee estimate at http://bitcoinfees.com/
    return ceil(feePerKb*((float)txByteSize/1024));
}

uint64_t ContractTransactionBuilder::transactionSize(uint32_t numberOfCommitments, bool hasChange) {

    ContractTransactionBuilder c;

    // set commitments
    c.setCommitments(Commitments(numberOfCommitments));

    // set change
    if(hasChange)
        c.setChange(Coin::Payment(0, Coin::PubKeyHash(uchar_vector(PUBKEY_HASH_BYTE_LENGTH,0xff))));

    // Generate transaction
    Coin::Transaction tx = c.transaction();

    // return total size
    return tx.getSize();
}

}
}
