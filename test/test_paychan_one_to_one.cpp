/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <gtest/gtest.h>

/// replace with convenience header
#include <common/Payment.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/RedeemScriptHash.hpp>
/// replace with convenience header
#include <paymentchannel/paymentchannel.hpp>
#include <CoinCore/CoinNodeData.h> // Transaction

using namespace joystream::paymentchannel;

TEST(paymentchannelTest, paychan_one_to_one)
{
    // replace all "generate" with fixed private keys from bitcore playground

    // Setup keys
    Coin::KeyPair payorContractKeyPair = Coin::KeyPair::generate();
    Coin::RedeemScriptHash payorFinalKeyHash;

    Coin::KeyPair payeeContractKeyPair = Coin::KeyPair::generate();
    Coin::RedeemScriptHash payeeFinalKeyHash;

    // Funding & allocation
    uint64_t source_amount = 3000000,
            change_amount = 200,
            contract_fee_amount = 50,
            amount_in_channel = source_amount - (change_amount + contract_fee_amount);

    // Construct contract
    auto funding = std::make_shared<Coin::UnspentP2PKHOutput>(Coin::KeyPair::generate(), Coin::typesafeOutPoint(), source_amount);
    ContractTransactionBuilder c;

    c.setFunding(Coin::UnspentOutputSet({funding}));

    // Terms
    Coin::RelativeLockTime lockTime;
    uint64_t price = 8;

    c.setCommitments(ContractTransactionBuilder::Commitments({Commitment(amount_in_channel, payorContractKeyPair.pk(), payeeContractKeyPair.pk(), lockTime)}));

    Coin::Payment change(change_amount, Coin::RedeemScriptHash());
    c.setChange(change);

    // Derive anchor
    Coin::Transaction contractTx = c.transaction();
    Coin::typesafeOutPoint anchor(Coin::TransactionId::fromTx(contractTx), 0);

    // Setup payor
    joystream::paymentchannel::Payor payor(price,
                                          0,
                                          amount_in_channel,
                                          0,
                                          lockTime,
                                          anchor,
                                          payorContractKeyPair,
                                          payorFinalKeyHash,
                                          payeeContractKeyPair.pk(),
                                          payeeFinalKeyHash);

    // Setup payee
    joystream::paymentchannel::Payee payee(0,
                                           lockTime,
                                           price,
                                           amount_in_channel,
                                           1,
                                           anchor,
                                           payeeContractKeyPair,
                                           payeeFinalKeyHash,
                                           payorContractKeyPair.pk(),
                                           payorFinalKeyHash,
                                           Coin::Signature());

    // Make series of payments
    int number_of_payments = 10;

    for(int i = 0; i < number_of_payments; i++) {
        // Payor makes payment i
        Coin::Signature paymentSignature = payor.makePayment();

        // Payee validates payment i
        EXPECT_TRUE(payee.registerPayment(paymentSignature));
    }

}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
