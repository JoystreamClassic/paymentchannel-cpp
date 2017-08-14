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
#include <common/TransactionSignature.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/RedeemScriptHash.hpp>
#include <paymentchannel/paymentchannel.hpp>
/// replace with convenience header

using namespace joystream::paymentchannel;

TEST(paymentchannelTest, settlement)
{
    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();
    Coin::RelativeLockTime lockTime = Coin::RelativeLockTime::fromBlockUnits(1);

    Coin::typesafeOutPoint contractOutPoint;
    joystream::paymentchannel::Commitment commitment(180, payorContractPair.pk(), payeeContractPair.pk(), lockTime);

    Coin::P2PKScriptPubKey payorScriptPubKey(payorFinalPair.pk());
    Coin::RedeemScriptHash payorScriptHash(payorScriptPubKey);

    Coin::P2PKScriptPubKey payeeScriptPubKey(payeeFinalPair.pk());
    Coin::RedeemScriptHash payeeScriptHash(payeeScriptPubKey);

    Coin::Payment toPayor(90, payorScriptHash);
    Coin::Payment toPayee(90, payeeScriptHash);

    joystream::paymentchannel::Settlement s(contractOutPoint,
                                            commitment,
                                            toPayor,
                                            toPayee);

    // Generate payee refund signature, hence using payee private key
    Coin::TransactionSignature payeePaySig = s.transactionSignature(payeeContractPair.sk());
    Coin::TransactionSignature payorPaySig = s.transactionSignature(payorContractPair.sk());

    bool validPayeeSettlementSig = s.validatePayeeSignature(payeePaySig.sig());
    EXPECT_TRUE(validPayeeSettlementSig);

    bool validPayorSettlementSig = s.validatePayorSignature(payorPaySig.sig());
    EXPECT_TRUE(validPayorSettlementSig);

    EXPECT_TRUE(s.fee() == 0);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
