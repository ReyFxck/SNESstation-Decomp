#!/usr/bin/env python3
"""Self-tests for the isolated historical EE stage-one bootstrap."""
from __future__ import annotations

import io
import tarfile
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path

from bootstrap_ee_gcc_stage1 import (
    AARCH64_CONFIG_PATCH,
    AARCH64_GCC_HOST_PATCH,
    ARCHIVES,
    BuildFailure,
    CXX_MODERN_HOST_PATCH,
    HOST_CFLAGS,
    apply_patch_once,
    safe_archive_path,
    safe_extract_archive,
)


class StageOneBootstrapTest(unittest.TestCase):
    def test_host_flags_accept_gcc14_legacy_configure_probes(self) -> None:
        # GCC 14 promotes these three pre-C99 diagnostics to errors by default.
        # Binutils 2.14's generated configure probes intentionally use that
        # historical source form, so downgrade only those diagnostics.
        for diagnostic in (
            "implicit-int",
            "implicit-function-declaration",
            "incompatible-pointer-types",
        ):
            self.assertIn(f"-Wno-error={diagnostic}", HOST_CFLAGS)

    def test_archive_contract_is_fully_pinned(self) -> None:
        self.assertEqual(
            [archive.name for archive in ARCHIVES],
            ["binutils-2.14.tar.gz", "gcc-3.2.2.tar.gz"],
        )
        for archive in ARCHIVES:
            self.assertTrue(archive.url.startswith("https://ftp.gnu.org/"))
            self.assertEqual(len(archive.sha256), 64)

    def test_archive_paths_must_stay_under_expected_root(self) -> None:
        self.assertEqual(
            safe_archive_path("gcc-3.2.2/gcc/tree.c", "gcc-3.2.2"),
            ("gcc-3.2.2", "gcc", "tree.c"),
        )
        with self.assertRaises(BuildFailure):
            safe_archive_path("../escape", "gcc-3.2.2")
        with self.assertRaises(BuildFailure):
            safe_archive_path("other/file", "gcc-3.2.2")

    def test_safe_extractor_accepts_regular_files(self) -> None:
        with tempfile.TemporaryDirectory(prefix="snesstation-bootstrap-test-") as name:
            root = Path(name)
            archive = root / "source.tar.gz"
            payload = b"historical source\n"
            with tarfile.open(archive, "w:gz") as output:
                directory = tarfile.TarInfo("gcc-3.2.2")
                directory.type = tarfile.DIRTYPE
                directory.mode = 0o755
                output.addfile(directory)
                member = tarfile.TarInfo("gcc-3.2.2/README")
                member.size = len(payload)
                member.mode = 0o644
                output.addfile(member, io.BytesIO(payload))

            with redirect_stdout(io.StringIO()):
                extracted = safe_extract_archive(
                    archive, root / "source", "gcc-3.2.2"
                )

            self.assertEqual((extracted / "README").read_bytes(), payload)

    def test_safe_extractor_rejects_links(self) -> None:
        with tempfile.TemporaryDirectory(prefix="snesstation-bootstrap-test-") as name:
            root = Path(name)
            archive = root / "source.tar.gz"
            with tarfile.open(archive, "w:gz") as output:
                member = tarfile.TarInfo("gcc-3.2.2/link")
                member.type = tarfile.SYMTYPE
                member.linkname = "../../escape"
                output.addfile(member)

            with self.assertRaises(BuildFailure):
                safe_extract_archive(archive, root / "source", "gcc-3.2.2")

    def test_aarch64_patch_updates_legacy_config_scripts(self) -> None:
        with tempfile.TemporaryDirectory(prefix="snesstation-bootstrap-test-") as name:
            root = Path(name)
            (root / "config.sub").write_text(
                """case $basic_machine in
	1750a | 580 \\
	| a29k \\
	| alpha | alphaev[4-8] | alphaev56 | alphaev6[78] | alphapca5[67] \\
	| alpha64 | alpha64ev[4-8] | alpha64ev56 | alpha64ev6[78] | alpha64pca5[67] \\
	| arc | arm | arm[bl]e | arme[lb] | armv[2345] | armv[345][lb] | avr \\
	| clipper \\
	| d10v | d30v | dlx | dsp16xx \\
	| fr30 | frv \\
*) ;;
case $basic_machine in
	| a29k-* \\
	| alpha-* | alphaev[4-8]-* | alphaev56-* | alphaev6[78]-* \\
	| alpha64-* | alpha64ev[4-8]-* | alpha64ev56-* | alpha64ev6[78]-* \\
	| alphapca5[67]-* | alpha64pca5[67]-* | arc-* \\
	| arm-*  | armbe-* | armle-* | armeb-* | armv*-* \\
	| avr-* \\
	| bs2000-* \\
*) ;;
""",
                encoding="utf-8",
            )
            (root / "config.guess").write_text(
                """case x in
    i*86:Minix:*:*)
	echo ${UNAME_MACHINE}-pc-minix
	exit 0 ;;
    arm*:Linux:*:*)
	echo ${UNAME_MACHINE}-unknown-linux-gnu
	exit 0 ;;
esac
""",
                encoding="utf-8",
            )

            with redirect_stdout(io.StringIO()):
                apply_patch_once(root, AARCH64_CONFIG_PATCH, ".stamp")

            self.assertIn("| aarch64 | arc | arm", (root / "config.sub").read_text())
            self.assertIn("aarch64:Linux:*:*)", (root / "config.guess").read_text())
            self.assertTrue((root / ".stamp").is_file())

    def test_gcc_patch_accepts_aarch64_only_as_a_build_host(self) -> None:
        with tempfile.TemporaryDirectory(prefix="snesstation-bootstrap-test-") as name:
            root = Path(name)
            (root / "gcc").mkdir()
            (root / "gcc" / "config.gcc").write_text(
                """case $machine in
*local*)
	rest=`echo $machine | sed -e \"s/$cpu_type-//\"`
	if test -f $srcdir/config/$cpu_type/t-$rest
	then tmake_file=$cpu_type/t-$rest
	fi
	;;
1750a-*-*)
	# 1750a is only supported as a target.
	case \"$build,$host\" in 1750a*,* | *,1750a* )
		exit 1
	esac
	;;
esac
""",
                encoding="utf-8",
            )

            with redirect_stdout(io.StringIO()):
                apply_patch_once(root, AARCH64_GCC_HOST_PATCH, ".host-stamp")

            config = (root / "gcc" / "config.gcc").read_text()
            self.assertIn("aarch64-*-linux*)", config)
            self.assertIn("supported only as a build/host", config)
            self.assertTrue((root / ".host-stamp").is_file())

    def test_cxx_patch_restores_modern_host_lvalue_compatibility(self) -> None:
        with tempfile.TemporaryDirectory(prefix="snesstation-bootstrap-test-") as name:
            root = Path(name)
            source = root / "gcc" / "cp"
            source.mkdir(parents=True)
            (source / "decl.c").write_text(
                """
/* The binding level currently in effect.  */

#define current_binding_level\t\t\t\\
  (cfun && cp_function_chain->bindings\t\t\\
   ? cp_function_chain->bindings\t\t\\
   : scope_chain->bindings)

/* The binding level of the current class, if any.  */
""",
                encoding="utf-8",
            )

            with redirect_stdout(io.StringIO()):
                apply_patch_once(root, CXX_MODERN_HOST_PATCH, ".cxx-host-stamp")

            text = (source / "decl.c").read_text(encoding="utf-8")
            self.assertIn("? &cp_function_chain->bindings", text)
            self.assertIn(": &scope_chain->bindings))", text)


if __name__ == "__main__":
    unittest.main()
