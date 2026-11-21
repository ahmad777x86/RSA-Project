# RSA-Project

A from-scratch implementation of the **RSA cryptosystem in C++**, built on 64-bit integers with no external libraries. It covers the full pipeline: modular arithmetic, key generation, block-based text encryption/decryption, and a brute-force demo that breaks a weak key by factoring `n`.

Built as a Discrete Structures project to connect number theory (modular exponentiation, the extended Euclidean algorithm, primality) to a working cipher.

> ⚠️ **Educational use only.** Keys here are tiny (`n` fits in 64 bits) and there is no padding. This is not secure and must not be used to protect real data.

---

## Features

- **Modular arithmetic**: overflow-safe `mul_mod`, fast `mod_exp` (square-and-multiply)
- **Number theory**: `gcd`, iterative `extended_gcd` (stack-based back-substitution), `mod_inverse`
- **Primality testing**: trial division up to √n
- **Key generation**: validates `p`, `q`, and `e`, then computes `n = p·q` and `d = e⁻¹ mod φ(n)`
- **Text encryption/decryption**: letters are packed into blocks, encrypted block-by-block, and output as zero-padded numeric ciphertext
- **Key-breaking demo**: factors a weak `n` to recover the private key and decrypt a message
- **Assertion-based test suite**

---

## Project Structure

| File | Description |
|---|---|
| `rsa.h` | Function declarations and the `uint64` typedef |
| `rsa.cpp` | All RSA logic (modules B0–B8 below) |
| `demo.cpp` | Interactive program: generate keys, encrypt, decrypt |
| `test_rsa.cpp` | Assertion tests for every module |
| `break_key.cpp` | Cryptanalysis demo: factor `n` and decrypt a ciphertext |
| `LICENSE` | MIT License |

---

## Modules

| ID | Function | Purpose |
|---|---|---|
| B0 | `mul_mod(a, b, m)` | `(a·b) mod m` via double-and-add, avoiding 64-bit overflow |
| B1 | `mod_exp(b, n, m)` | `bⁿ mod m` via square-and-multiply |
| B2 | `gcd(a, b)` | Euclidean algorithm |
| B3 | `extended_gcd(a, b, s, t)` | Returns `g = gcd(a, b)` and Bézout coefficients with `g = s·a + t·b` |
| B4 | `mod_inverse(a, m)` | Modular inverse; throws `invalid_argument` if none exists |
| B5 | `is_prime(n)` | Trial-division primality test |
| B6 | `generate_keys(p, q, e, n, d)` | Validates inputs and produces the key pair |
| B7 | `encrypt` / `decrypt` | Single-integer RSA: `Mᵉ mod n` and `Cᵈ mod n` |
| B8 | `block_size`, `encrypt_text`, `decrypt_text` | Text ↔ numeric blocks |

### Key generation rules

`generate_keys` throws `std::invalid_argument` if:

- `p` or `q` is not prime
- `p == q`
- `e ≤ 1`, `e ≥ φ(n)`, or `gcd(e, φ(n)) ≠ 1`

### Text encoding scheme

1. Non-letters are stripped and the text is uppercased (`"Stop!"` → `STOP`).
2. Each letter maps to two digits (`A=00 … Z=25`).
3. The message is padded with `X` to a multiple of the block size `k`, where `k` is the number of letters per block, chosen so that every block value stays below `n`.
4. Each block is encrypted and written as a `2k`-digit zero-padded number; blocks are separated by spaces.

Example with `n = 2537`, `e = 13` (block size 2):

```
"STOP"  →  ST OP  →  1819 1415  →  2081 2182
```

---

## Build & Run

Requires a C++11 (or newer) compiler such as `g++` or `clang++`.

**Run the tests**

```bash
g++ -std=c++17 -o test_rsa test_rsa.cpp rsa.cpp
./test_rsa
```

**Interactive demo**

```bash
g++ -std=c++17 -o demo demo.cpp rsa.cpp
./demo
```

```
Enter p q e: 43 59 13
Public Key :n = 2537 e = 13
Private Key: d = 937
Help me
Block size: 2
Message: Help me
Cleaned: HELPME
Encrypted text: 0981 0461 0301
Decrypted text: HELPME
```

Enter two distinct primes `p`, `q` and a public exponent `e`, then type a message on the next line.

**Break a weak key**

```bash
g++ -std=c++17 -o break_key break_key.cpp rsa.cpp
./break_key
```

```
P: 1000003 | Q: 1000033
Decrypted text: NUMBERTHEORYISFUNX
```

---

## How the Key Break Works

`break_key.cpp` is given only the **public** key (`n = 1000036000099`, `e = 65537`) and a ciphertext. Because `n` is small, trial division factors it instantly into `p = 1000003` and `q = 1000033`. From there:

```
φ(n) = (p − 1)(q − 1)
d    = e⁻¹ mod φ(n)
```

and the message decrypts. Real RSA uses 2048-bit or larger moduli, where factoring is computationally infeasible. That gap is the whole point of the demo.

---

## Limitations

- Supports only the letters A–Z; other characters are discarded and the output is uppercase
- Messages are padded with trailing `X`, which is not removed on decryption
- `p`, `q` should stay below 2³¹ so that `p·q` fits in 64 bits
- No padding scheme (e.g. OAEP), so encryption is deterministic and insecure
- Trial-division primality testing is only practical for small inputs

---

## Concepts Demonstrated

Modular arithmetic · Fast exponentiation · Euclidean and extended Euclidean algorithms · Modular inverses · Euler's totient · Primality testing · Public-key cryptography · Integer factorization attacks

---

## Authors

- [mubashirehsan10-bit](https://github.com/mubashirehsan10-bit)
- [ahmad777x86](https://github.com/ahmad777x86) (original repository)

## License

Released under the [MIT License](LICENSE).
