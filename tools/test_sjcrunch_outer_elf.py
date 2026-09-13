import copy
import hashlib
import json
import tempfile
import unittest
import zipfile
from pathlib import Path

from sjcrunch_outer_elf import (
    DEFAULT_MANIFEST,
    OuterElfError,
    assembly_source,
    load_manifest,
    validate_manifest,
    verify_archive,
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


class SJCrunchOuterElfTests(unittest.TestCase):
    def test_public_manifest_closes_complete_packed_elf(self) -> None:
        manifest = load_manifest(DEFAULT_MANIFEST)
        self.assertEqual("2.1", manifest["package"]["version"])
        self.assertEqual(726_968, manifest["build"]["output_size"])
        self.assertEqual(
            "4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487",
            manifest["build"]["output_sha256"],
        )

    def test_generated_assembly_preserves_historical_empty_pdr(self) -> None:
        source = assembly_source()
        self.assertIn('.incbin "container.bin"', source)
        self.assertIn('.section .pdr,""', source)
        self.assertIn(".globl PackedElf", source)

    def test_rejects_private_payload_fields(self) -> None:
        manifest = load_manifest(DEFAULT_MANIFEST)
        changed = copy.deepcopy(manifest)
        changed["private_payload"] = "forbidden"
        with self.assertRaisesRegex(OuterElfError, "forbidden private field"):
            validate_manifest(changed)

    def test_verifies_archive_and_historical_members(self) -> None:
        members = {
            "script/crunch_crt0.o": b"crt",
            "script/libsjcrunch.a": b"library",
            "script/linkfile": b"linker",
            "ee/main.c": b"source",
        }
        rows = [
            {"path": name, "size": len(data), "sha256": sha256(data)}
            for name, data in members.items()
        ]
        manifest = {
            "linked_inputs": rows[:3],
            "source_evidence": rows[3:],
        }
        with tempfile.TemporaryDirectory() as temporary:
            archive_path = Path(temporary) / "sjcrunch.zip"
            with zipfile.ZipFile(archive_path, "w") as archive:
                for name, data in members.items():
                    archive.writestr(name, data)
            archive_data = archive_path.read_bytes()
            manifest["archive"] = {
                "size": len(archive_data),
                "sha256": sha256(archive_data),
            }
            verify_archive(archive_path, manifest)

            changed = json.loads(json.dumps(manifest))
            changed["source_evidence"][0]["sha256"] = "0" * 64
            with self.assertRaisesRegex(OuterElfError, "unexpected SHA-256"):
                verify_archive(archive_path, changed)


if __name__ == "__main__":
    unittest.main()
