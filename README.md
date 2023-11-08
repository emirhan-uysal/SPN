# SPN

This is a *SPN*(Substitution-permutaion network) implementation in C language.
I use this implemention as a Round Function in my project.

**Details About my Implemention**

I use this implementation to encrypt a 32bits block in a feistel network as a round function. So the size of the S-Box and P-Box are given accordingly.

- S-Box is 16x32bits size. Each output line of the S-Box is the XOR of the 16 random input lines.
- P-Box is 16x16bits size.