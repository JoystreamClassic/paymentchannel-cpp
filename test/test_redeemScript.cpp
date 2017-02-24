/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#include <gtest/gtest.h>

/// replace with convenience header
#include <common/KeyPair.hpp>
//#include <common/RedeemScriptHash.hpp>
/// replace with convenience header

#include <paymentchannel/paymentchannel.hpp>

using namespace joystream::paymentchannel;

TEST(paymentchannelTest, redeemScript)
{
    Coin::KeyPair payorPair = Coin::KeyPair::generate();
    Coin::KeyPair payeePair = Coin::KeyPair::generate();
    Coin::RelativeLockTime locktime;
    RedeemScript rs(payorPair.pk(), payeePair.pk(), locktime);
    uchar_vector serialized = rs.serialized();
    RedeemScript rs_des = RedeemScript::deserialize(serialized);

    EXPECT_EQ(serialized, rs_des.serialized());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
