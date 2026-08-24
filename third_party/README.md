# Third-party historical material

Small source fragments, object/listing fingerprints, patches and two rare
compiler-candidate archives are retained here with provenance and hashes.

Official GNU source releases are not duplicated in the current working tree.
The maintained tools download them on demand into ignored `build/` storage and
verify their SHA-256 before use:

```bash
make bootstrap-ee-stage1
make fetch-newlib
```

The former committed copies of `binutils-2.14.tar.gz`, `gcc-3.2.2.tar.gz` and
`newlib-1.10.0.tar.gz` remain recoverable from Git history. Removing them from
the current tree avoids presenting download caches as source, but does not
rewrite repository history.

The archives under `toolchain/archive/` are different: they are rare historical
EE compiler fingerprint candidates and remain preserved until a durable,
hash-verified external home is established.
