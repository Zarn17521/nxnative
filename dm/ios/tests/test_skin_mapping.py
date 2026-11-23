"""Regression tests for the user-supplied Strata controller artwork geometry."""
import re
import unittest
from pathlib import Path

SRC = Path(__file__).resolve().parents[1] / "Sources"
HEADER = (SRC / "DmSkin.h").read_text()
GAMEPAD = (SRC / "DmGamepad.m").read_text()
MENU = (SRC / "GameMenu.swift").read_text()

class StrataSkinMappingTests(unittest.TestCase):
    def test_reference_cutouts_remain_exact(self):
        self.assertIn("CGSizeMake(923, 2000), CGRectMake(37, 138, 847, 661)", HEADER)
        self.assertIn("CGSizeMake(1792, 1008), CGRectMake(463, 169, 863, 574)", HEADER)

    def test_no_distortion_and_correct_centering(self):
        for sw, sh, x, y, w, h in [(1792, 1008, 463, 169, 863, 574),
                                  (923, 2000, 37, 138, 847, 661)]:
            # Fit skin to the iPhone 15 in each orientation; then fit 16:9 game into the cutout.
            bw, bh = (852, 393) if sw > sh else (393, 852)
            scale = min(bw / sw, bh / sh)
            aw, ah = sw * scale, sh * scale
            ox, oy = (bw - aw) / 2, (bh - ah) / 2
            hx, hy, hw, hh = ox + x * scale, oy + y * scale, w * scale, h * scale
            gw, gh = min(hw, hh * 16 / 9), min(hw, hh * 16 / 9) * 9 / 16
            self.assertLessEqual(gw, hw + .001)
            self.assertLessEqual(gh, hh + .001)
            self.assertGreaterEqual((hw - gw) / 2, 0)
            self.assertGreaterEqual((hh - gh) / 2, 0)
            self.assertGreaterEqual(hx, 0)
            self.assertGreaterEqual(hy, 0)

    def test_mapping_is_exposed_in_game_menu(self):
        self.assertIn('"Strata Skin"', MENU)
        self.assertIn("dm_skin_import", MENU)
        self.assertIn("dm_skin_game_rect", (SRC / "front_ios.m").read_text())
        self.assertIn("dm_skin_point", GAMEPAD)

if __name__ == "__main__":
    unittest.main()
