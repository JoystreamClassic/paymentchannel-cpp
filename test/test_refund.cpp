/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <gtest/gtest.h>

/// replace with convenience header
#include <common/typesafeOutPoint.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/RedeemScriptHash.hpp>
/// replace with convenience header

#include <paymentchannel/paymentchannel.hpp>

using namespace joystream::paymentchannel;

TEST(paymentchannelTest, refund)
{
    Coin::KeyPair payorContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payorFinalPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeContractPair = Coin::KeyPair::generate();
    Coin::KeyPair payeeFinalPair = Coin::KeyPair::generate();

    Coin::typesafeOutPoint contractOutPoint;

    // settlement final destinations
    Coin::P2PKScriptPubKey payorScriptPubKey(payorFinalPair.pk());
    Coin::RedeemScriptHash payorScriptHash(payorScriptPubKey);

    Coin::P2PKScriptPubKey payeeScriptPubKey(payeeFinalPair.pk());
    Coin::RedeemScriptHash payeeScriptHash(payeeScriptPubKey);

    Coin::RelativeLockTime lockTime = Coin::RelativeLockTime::fromTimeUnits(3);
    uint64_t channelValue = 180;

    joystream::paymentchannel::Payor p(1, 0, channelValue, 1000, lockTime,contractOutPoint, payorContractPair, payorScriptHash, payeeContractPair.pk(), payeeScriptHash);
    joystream::paymentchannel::Refund R(p.refund());

    EXPECT_EQ(R.getUnspentOutput()->value(), channelValue);
    EXPECT_TRUE(R.getUnspentOutput()->outPoint() == contractOutPoint);

    // The output is locked until 3 * 512 seconds pass from the time that the contracted was mined (T = 0)
    EXPECT_EQ((unsigned int)lockTime.getDuration().count(), (unsigned int)1536);
    EXPECT_EQ((unsigned int)R.lockedUntil(0), (unsigned int)1536);
    EXPECT_TRUE(R.isLocked( 512, 0)); // locked
    EXPECT_TRUE(R.isLocked(1024, 0)); // locked
    EXPECT_FALSE(R.isLocked(1536, 0)); // unlocked
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
