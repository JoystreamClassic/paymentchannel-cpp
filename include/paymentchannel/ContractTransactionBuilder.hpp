/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 20 2016
 */

#ifndef PAYMENTCHANNEL_CONTRACT_HPP
#define PAYMENTCHANNEL_CONTRACT_HPP

#include <common/UnspentOutputSet.hpp>
#include <common/Payment.hpp>
#include <paymentchannel/Commitment.hpp>

#include <boost/optional.hpp> // std::optional<> in c++17
#include <vector>

namespace Coin {
    class Transaction;
}

namespace joystream {
namespace paymentchannel {

class ContractTransactionBuilder {

public:

    typedef std::vector<Commitment> Commitments;

    ContractTransactionBuilder();

    boost::optional<Coin::UnspentOutputSet> funding() const;
    void setFunding(const boost::optional<Coin::UnspentOutputSet> &funding);

    Commitments commitments() const;
    void setCommitments(const Commitments &commitments);

    boost::optional<Coin::Payment> change() const;
    void setChange(const boost::optional<Coin::Payment> &change);

    /**
     * @brief Contract transaction for payment channel setup, which
     * is also signed if funding was provided.
     * @return contract transaction
     */
    Coin::Transaction transaction() const;

    // Transaction fee for contract with given terms
    static uint64_t totalFee(uint32_t numberOfCommitments, bool hasChange, uint64_t feePerKb, uint64_t sizeOfAllInputs);

private:

    // The size of a contract transaction with given terms
    static uint64_t transactionSize(uint32_t, bool);

    // Funding contract
    boost::optional<Coin::UnspentOutputSet> _funding;

    // Commitments for end to end channels
    Commitments _commitments;

    // Change in contract back to payor
    boost::optional<Coin::Payment> _change;
};

}
}

#endif // PAYMENTCHANNEL_CONTRACT_HPP
