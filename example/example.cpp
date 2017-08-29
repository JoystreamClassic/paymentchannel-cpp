#include <paymentchannel/paymentchannel.hpp>
#include <common/P2PKHAddress.hpp>
#include <common/P2PKHScriptPubKey.hpp>
#include <iostream>

int main(int argc, char* argv[]) {

  const uint64_t price = 100;
  const uint64_t numberOfPaymentsMade = 10;
  const uint64_t funds = 20000;
  const uint64_t settlementFee = 2500;
  const uint64_t refundFee = 2500;
  const uint64_t refundAmount = funds - refundFee;

  Coin::RelativeLockTime refundLockTime = Coin::RelativeLockTime::fromBlockUnits(5);
  const Coin::typesafeOutPoint anchor;
  const Coin::KeyPair payorContractKeyPair = Coin::KeyPair(Coin::PrivateKey::generate());
  const Coin::PubKeyHash payorFinalPkHash;
  const Coin::PublicKey payeeContractPk = Coin::PrivateKey::generate().toPublicKey();
  const Coin::PubKeyHash payeeFinalPkHash;

  joystream::paymentchannel::Payor payor(price, numberOfPaymentsMade, funds, settlementFee, refundLockTime, anchor, payorContractKeyPair, payorFinalPkHash, payeeContractPk, payeeFinalPkHash);

  auto refund = payor.refund();

  auto refundUnspentOutput = refund.getUnspentOutput();

  Coin::UnspentOutputSet refundUnspentOutputSet;
  refundUnspentOutputSet.insert(refundUnspentOutputSet.end(), std::shared_ptr<Coin::UnspentOutput>(refundUnspentOutput));

  Coin::Transaction refundTx;

  // To indicate that we accept new consensus rules (BIP68 - Relative Lock Time, we must set the transaction
  // version to be greater than 1
  refundTx.version = 2;

  auto refundDestination = Coin::PrivateKey::generate().toPublicKey().toPubKeyHash();

  auto refundOutput = Coin::TxOut(refundAmount, Coin::P2PKHScriptPubKey(refundDestination).serialize());

  refundTx.addOutput(refundOutput);

  // finance the transaction
  refundUnspentOutputSet.finance(refundTx, Coin::SigHashType::standard());

  return 0;
}
