import { Connection, Keypair, PublicKey, SystemProgram, sendAndConfirmTransaction, Transaction, TransactionInstruction } from "@solana/web3.js";
import { expect, test } from "bun:test";

const connection = new Connection("https://api.devnet.solana.com", "confirmed");

const PROGRAM_ID = new PublicKey("F8T62UmXoaA9N8JZuFGh5dLWgfqVuvvnGBvmuEQ4Qi1z");

test("transfer 1 SOL from fromWallet to toWallet", async () => {
  const fromWallet = Keypair.generate();
  const toWallet = Keypair.generate();

  const airdropSig = await connection.requestAirdrop(fromWallet.publicKey, 2e9);
  await connection.confirmTransaction(airdropSig);

  const ix = new TransactionInstruction({
    keys: [
      { pubkey: fromWallet.publicKey, isSigner: true, isWritable: true },
      { pubkey: toWallet.publicKey, isSigner: false, isWritable: true },
      { pubkey: SystemProgram.programId, isSigner: false, isWritable: false },
    ],
    programId: PROGRAM_ID,
    data: Buffer.alloc(0),
  });

  const tx = new Transaction().add(ix);
  await sendAndConfirmTransaction(connection, tx, [fromWallet]);

  const toBalance = await connection.getBalance(toWallet.publicKey);
  expect(toBalance).toBeGreaterThan(0.9e9); // should be approx 1 SOL
});
