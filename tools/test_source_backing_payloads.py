"""Source integration must not silently substitute privately extracted bytes."""
import copy
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
import data_backing as g


class SourceBackingPayloadTests(unittest.TestCase):
    def setUp(self):
        self.payload = b"abcdef"
        self.owner = {"unit": "test", "symbol": "array", "address": 0x1000,
                      "size": 6, "sha256": g.digest(self.payload)}
        self.section = {"section": ".data.stage3f.test", "target_address": "0x1001",
                        "extent_hex": "0x3", "origin": g.HISTORICAL, "region": "initialized",
                        "sha256": g.digest(b"bcd")}
        self.payloads = {("test", "array"): self.payload}

    def slices(self, owners=None, payloads=None):
        return g.source_backing_payloads([self.section], [self.owner] if owners is None else owners,
                                        self.payloads if payloads is None else payloads)

    def test_subtracting_old_ownership_uses_only_the_source_subrange(self):
        self.assertEqual({self.section["section"]: b"bcd"}, self.slices())

    def test_missing_source_bytes_fail_even_with_a_valid_target_fingerprint(self):
        with self.assertRaisesRegex(g.DataBackingError, "absent"):
            self.slices(payloads={})

    def test_changed_full_provider_is_rejected_even_if_selected_bytes_equal(self):
        with self.assertRaisesRegex(g.DataBackingError, "changed"):
            self.slices(payloads={("test", "array"): b"Xbcdef"})

    def test_truncated_provider_rejected(self):
        with self.assertRaises(g.DataBackingError):
            self.slices(payloads={("test", "array"): self.payload[:-1]})

    def test_ambiguous_or_missing_owner_rejected(self):
        for owners in ([], [self.owner, dict(self.owner)]):
            with self.assertRaises(g.DataBackingError):
                self.slices(owners=owners)

    def test_section_cannot_extend_beyond_the_typed_provider(self):
        self.section["extent_hex"] = "0x6"
        with self.assertRaises(g.DataBackingError):
            self.slices()

    def test_piece_fingerprint_must_match_its_frozen_layout(self):
        self.section["sha256"] = g.digest(b"wrong")
        with self.assertRaisesRegex(g.DataBackingError, "frozen"):
            self.slices()

    def test_private_minimum_access_bytes_are_not_claimed_as_source(self):
        self.section["origin"] = g.NEW
        self.assertEqual({}, self.slices())

    def test_assembly_has_no_private_fallback_for_historical_rows(self):
        with self.assertRaisesRegex(g.DataBackingError, "fresh source"):
            g.render_additions(Path("original.bin"), [self.section])

    def test_assembly_uses_source_path_and_zero_offset(self):
        text = g.render_additions(Path("original.bin"), [self.section],
                                  {self.section["section"]: Path("source payload.bin")})
        self.assertNotIn("original.bin", text)
        self.assertIn('source payload.bin",0,3', text)

    def test_link_helper_performs_fresh_build_before_writing_any_piece(self):
        frozen = {"owners": [self.owner]}
        with tempfile.TemporaryDirectory() as tmp:
            args = g.parse_args(["link", "--build-dir", tmp])
            def build(settings, output):
                settings.build_dir.mkdir(parents=True)
                output.update(self.payloads)
                return copy.deepcopy(frozen)
            with patch.object(g.historical_data, "validate", return_value=frozen), \
                 patch.object(g.historical_data, "build_manifest", side_effect=build) as rebuild:
                result = g.rebuild_source_backing(args, [self.section])
                rebuild.assert_called_once()
                self.assertEqual(b"bcd", result[self.section["section"]].read_bytes())

    def test_changed_recipe_cannot_be_reused_as_cached_source_evidence(self):
        with tempfile.TemporaryDirectory() as tmp:
            args = g.parse_args(["link", "--build-dir", tmp])
            with patch.object(g.historical_data, "validate", return_value={"owners": [self.owner]}), \
                 patch.object(g.historical_data, "build_manifest", return_value={"owners": []}):
                with self.assertRaisesRegex(g.DataBackingError, "fresh source rebuild"):
                    g.rebuild_source_backing(args, [self.section])
            self.assertEqual([], list(Path(tmp).rglob("provider-*.bin")))
