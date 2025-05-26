extern "C" uint64_t sol_invoke(const SolInstruction *instruction, const SolAccountInfo *account_infos, int account_infos_len) {
    std::cout << "sol_invoke called with transfer of " << *(uint64_t*)&instruction->data[4] << " lamports\n";
    return 0; 
}


int main() {
    uint8_t buffer[256] = {0};

    SolAccountInfo fake_accounts[3] = {
        {.key = (SolPubkey*)"from", .lamports = 1000000000, .data = nullptr, .owner = nullptr, .is_signer = true, .is_writable = true},
        {.key = (SolPubkey*)"to", .lamports = 0, .data = nullptr, .owner = nullptr, .is_signer = false, .is_writable = true},
        {.key = (SolPubkey*)"system", .lamports = 0, .data = nullptr, .owner = nullptr, .is_signer = false, .is_writable = false},
    };

    std::memcpy(buffer + 8, fake_accounts, sizeof(fake_accounts));

    uint64_t result = entrypoint(buffer);
    std::cout << "EntryPoint returned: " << result << "\n";

    return 0;
}
