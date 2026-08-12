import unittest

from objdump_listing_to_binary import rebuild_listing_range


class ObjdumpListingToBinaryTests(unittest.TestCase):
    def test_rebuilds_requested_range_and_zero_fills_gaps(self) -> None:
        listing = """
  1000: 08 00 e0 03  jr $ra
  1004: 04 00 0c 46  sqrt.s $f0, $f12
  1010: 05 60 00 46  abs.s $f0, $f12
"""
        image, count = rebuild_listing_range(listing, 0x1000, 0x1014)

        self.assertEqual(count, 3)
        self.assertEqual(image[:8], bytes.fromhex("08 00 e0 03 04 00 0c 46"))
        self.assertEqual(image[8:16], bytes(8))
        self.assertEqual(image[16:20], bytes.fromhex("05 60 00 46"))

    def test_rejects_empty_requested_range(self) -> None:
        with self.assertRaisesRegex(ValueError, "no instruction bytes"):
            rebuild_listing_range("  2000: 00 00 00 00", 0x1000, 0x1010)


if __name__ == "__main__":
    unittest.main()
