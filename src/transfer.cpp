extern "C" {
#include <solana_sdk.h>
}

// entrypoint function called by solana runtime when the program is invoked
extern "C" uint64_t entrypoint(const uint8_t* input) {
    // skip the first 8 bytes of input (typically instruction discriminator)
    // then treat the next bytes as an array of solaccountinfo structs
    solaccountinfo* accounts = (solaccountinfo*)(input + 8);

    // define pointers to specific accounts based on index:
    // from - the account sending lamports
    solaccountinfo* from = &accounts[0];
    // to - the account receiving lamports
    solaccountinfo* to = &accounts[1];
    // system_program - the system program account (to invoke transfer)
    solaccountinfo* system_program = &accounts[2];

    // prepare data for system_program transfer instruction:
    // first 4 bytes represent the enum instruction code (2 = transfer)
    // next 8 bytes represent the amount of lamports to transfer
    uint8_t data[12] = {0};
    data[0] = 2; // set instruction enum to 'transfer' (value 2)

    // set lamports amount to transfer (1 sol = 1_000_000_000 lamports)
    uint64_t lamports = 1000000000;

    // copy lamports value into data buffer starting at offset 4 bytes
    __builtin_memcpy(&data[4], &lamports, 8);

    // create array of solaccountmeta structs describing involved accounts:
    // - first account is from (writable, signer)
    // - second account is to (writable, not signer)
    solaccountmeta meta_accounts[] = {
        { .pubkey = from->key, .is_writable = true, .is_signer = true },
        { .pubkey = to->key, .is_writable = true, .is_signer = false },
    };

    // create solinstruction struct describing the instruction to invoke:
    // - program_id is system_program's public key
    // - accounts metadata is meta_accounts array with length 2
    // - data points to prepared data buffer with length 12 bytes
    solinstruction instruction = {
        .program_id = system_program->key,
        .accounts = meta_accounts,
        .account_len = 2,
        .data = data,
        .data_len = sizeof(data),
    };

    // call sol_invoke syscall to execute the transfer instruction
    // passing the instruction, all accounts array, and total accounts count (3)
    return sol_invoke(&instruction, accounts, 3);
}

