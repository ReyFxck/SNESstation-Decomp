#!/usr/bin/env python3
"""Extract and validate SNES Station v0.23 assets from the unpacked image.

The extracted files are private products of a user-supplied reference binary.
They are written below ``build/`` by default and must not be committed.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import zlib
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[1]
REFERENCE_BASE = 0x00100000
REFERENCE_SIZE = 3_304_936
REFERENCE_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
XOR_KEY = 0x96695AA5


@dataclass(frozen=True)
class AssetSpec:
    name: str
    kind: str
    va: int
    size: int
    sha256: str
    output: str
    size_word_va: int | None = None
    decoded_sha256: str | None = None

    @property
    def end_va(self) -> int:
        return self.va + self.size


ASSETS = (
    AssetSpec(
        "cdvd_irx",
        "IRX ELF32",
        0x001EC300,
        0x7DD4,
        "0dbf147d0f0cb2a49c7d734e92df0570b079c52dde19478bc94b5283345050de",
        "irx/cdvd.irx",
        0x001F40D4,
    ),
    AssetSpec(
        "sjpcm_irx",
        "IRX ELF32",
        0x001F4100,
        0x1FC5,
        "690d69decfd2abaed46a06c402be0b835d3329bb49a1012ecc29a7a4a9ad579f",
        "irx/sjpcm.irx",
        0x001F60C8,
    ),
    AssetSpec(
        "amigamod_irx",
        "IRX ELF32",
        0x001F6140,
        0x4E5D,
        "25d8b8b8e0a9ec1a28ff944eb2a21f53b87125d4f7f74ddb5f93d4621005a3e3",
        "irx/amigamod.irx",
        0x001FAFA0,
    ),
    AssetSpec(
        "frontend_background_iif",
        "IIF1 GS texture",
        0x001FAFD0,
        0x96010,
        "4e840cd29cad2d322745ce0e229bcb8ee87ccb9953f258e4085057f43cca9d6c",
        "graphics/frontend_background.iif",
    ),
    AssetSpec(
        "frontend_logo_iif",
        "IIF1 GS texture",
        0x00290FF0,
        0x1A1E0,
        "b77c27f9ecbe25f0f781675e52c7558130f48653b2189b5b027c48c29444a3ad",
        "graphics/frontend_logo.iif",
    ),
    AssetSpec(
        "frontend_panel_corner_iif",
        "IIF1 GS texture",
        0x002AB1E0,
        0x910,
        "7ad58b6011d746fc89b28a78ce41a60c1619d67bf7281fe85e673e46ec39bfb0",
        "graphics/frontend_panel_corner.iif",
    ),
    AssetSpec(
        "frontend_font_bfnt",
        "BFNT GS font",
        0x002ABB00,
        0x40120,
        "65deef51212e1aa73ef78a8991d38dee56125db97c9d272c5ab8baab856f63f6",
        "graphics/frontend_font.bfnt",
    ),
    AssetSpec(
        "credits_text",
        "XOR-obfuscated text",
        0x002EBC30,
        0x5C1,
        "c0df5f69b9d92c4bfaee26e6b67aff543c84c245325c6bdae0559fda8dbbcf8c",
        "text/credits.txt",
        0x002EC1F4,
        "baf9e6d8f17ce72c7414caed3cc575377856d33ac64d8e6ae82bf9296ce1c6fc",
    ),
    AssetSpec(
        "disclaimer_text",
        "XOR-obfuscated text",
        0x002EC200,
        0x33A,
        "9c63830df59415ff015df28941d7724c28ea539c68b83e25cdf05a6f5d67c6ed",
        "text/disclaimer.txt",
        0x002EC53C,
        "90a3cd5bf0c0945a3c9ad049d2741affe0c2a72f95b3613de2c7d674742d8c99",
    ),
    AssetSpec(
        "azazel_mod",
        "ProTracker MOD",
        0x002EC540,
        0x3640C,
        "a60a9079c4971454eb5b501fa024cd1e9e93c890811bddfe49ec553fd699adf5",
        "audio/azazel-cant-stop-coming.mod",
        0x0032294C,
    ),
    AssetSpec(
        "memory_card_icon",
        "PS2 Memory Card 3D icon",
        0x00322980,
        0x128F8,
        "e3360a2f45353f2d8d222bd427c018587e8e48d8fd40df4f1449924e015fc571",
        "memory-card/snes_emu.ico",
        0x00335278,
    ),
)

ASSET_BY_NAME = {asset.name: asset for asset in ASSETS}


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def image_slice(image: bytes, asset: AssetSpec) -> bytes:
    offset = asset.va - REFERENCE_BASE
    end = offset + asset.size
    if offset < 0 or end > len(image):
        raise ValueError(
            f"{asset.name}: VA range 0x{asset.va:08x}..0x{asset.end_va:08x} "
            "is outside the reference image"
        )
    data = image[offset:end]
    digest = sha256(data)
    if digest != asset.sha256:
        raise ValueError(
            f"{asset.name}: SHA-256 mismatch: expected {asset.sha256}, got {digest}"
        )
    return data


def validate_size_word(image: bytes, asset: AssetSpec) -> int | None:
    if asset.size_word_va is None:
        return None
    offset = asset.size_word_va - REFERENCE_BASE
    if offset < 0 or offset + 4 > len(image):
        raise ValueError(f"{asset.name}: size word lies outside the reference image")
    stored_size = struct.unpack_from("<I", image, offset)[0]
    if stored_size != asset.size:
        raise ValueError(
            f"{asset.name}: expected size word 0x{asset.size:x}, got 0x{stored_size:x}"
        )
    return stored_size


def parse_irx_elf(data: bytes) -> dict[str, int]:
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ValueError("IRX does not contain an ELF32 header")
    if data[4:6] != b"\x01\x01":
        raise ValueError("IRX is not a little-endian ELF32 file")
    header = struct.unpack_from("<16sHHIIIIIHHHHHH", data)
    e_type, e_machine, entry = header[1], header[2], header[4]
    phoff, phentsize, phnum = header[5], header[9], header[10]
    if e_machine != 8:
        raise ValueError(f"IRX has unexpected ELF machine {e_machine}")
    if phoff + phentsize * phnum > len(data):
        raise ValueError("IRX program-header table is truncated")
    return {
        "elf_type": e_type,
        "machine": e_machine,
        "entry": entry,
        "program_headers": phnum,
    }


def parse_iif(data: bytes) -> dict[str, int]:
    if len(data) < 16 or data[:4] != b"IIF1":
        raise ValueError("texture does not contain an IIF1 header")
    width, height, psm = struct.unpack_from("<III", data, 4)
    bytes_per_pixel = {0x00: 4, 0x01: 3, 0x02: 2, 0x0A: 2}.get(psm)
    if not width or not height or bytes_per_pixel is None:
        raise ValueError(f"unsupported IIF geometry/PSM: {width}x{height}, PSM=0x{psm:x}")
    expected = 16 + width * height * bytes_per_pixel
    if len(data) != expected:
        raise ValueError(f"IIF size mismatch: expected {expected}, got {len(data)}")
    return {
        "width": width,
        "height": height,
        "psm": psm,
        "bytes_per_pixel": bytes_per_pixel,
        "pixel_offset": 16,
    }


def _expand_5(value: int) -> int:
    return (value << 3) | (value >> 2)


def _ps2_alpha(value: int) -> int:
    # The GS convention uses 0x80 as fully opaque. Some source art contains
    # 0xff, so saturate instead of wrapping it.
    return min(255, value * 2)


def rgba_from_iif(data: bytes, info: dict[str, int]) -> bytes:
    psm = info["psm"]
    pixels = data[info["pixel_offset"] :]
    rgba = bytearray()
    if psm in (0x02, 0x0A):
        for (value,) in struct.iter_unpack("<H", pixels):
            rgba.extend(
                (
                    _expand_5(value & 0x1F),
                    _expand_5((value >> 5) & 0x1F),
                    _expand_5((value >> 10) & 0x1F),
                    255,
                )
            )
    elif psm == 0x00:
        for red, green, blue, alpha in struct.iter_unpack("4B", pixels):
            rgba.extend((red, green, blue, _ps2_alpha(alpha)))
    elif psm == 0x01:
        for index in range(0, len(pixels), 3):
            rgba.extend((*pixels[index : index + 3], 255))
    else:  # guarded by parse_iif
        raise ValueError(f"unsupported IIF PSM 0x{psm:x}")
    return bytes(rgba)


def parse_bfnt(data: bytes) -> dict[str, int]:
    if len(data) < 0x120 or data[:4] != b"BFNT":
        raise ValueError("font does not contain a BFNT header")
    width, height, psm, nx, ny, cell_w, cell_h = struct.unpack_from("<7I", data, 4)
    bytes_per_pixel = {0x00: 4, 0x01: 3, 0x02: 2, 0x0A: 2}.get(psm)
    if bytes_per_pixel is None:
        raise ValueError(f"unsupported BFNT PSM 0x{psm:x}")
    expected = 0x120 + width * height * bytes_per_pixel
    if len(data) != expected:
        raise ValueError(f"BFNT size mismatch: expected {expected}, got {len(data)}")
    if nx * cell_w != width or ny * cell_h != height:
        raise ValueError("BFNT character grid does not cover the texture")
    return {
        "width": width,
        "height": height,
        "psm": psm,
        "bytes_per_pixel": bytes_per_pixel,
        "columns": nx,
        "rows": ny,
        "cell_width": cell_w,
        "cell_height": cell_h,
        "pixel_offset": 0x120,
    }


def rgba_from_bfnt(data: bytes, info: dict[str, int]) -> bytes:
    synthetic_iif = b"IIF1" + struct.pack(
        "<III", info["width"], info["height"], info["psm"]
    ) + data[info["pixel_offset"] :]
    return rgba_from_iif(synthetic_iif, parse_iif(synthetic_iif))


def decode_xor_words(data: bytes, key: int = XOR_KEY) -> bytes:
    decoded = bytearray(data)
    for offset in range(0, len(decoded) - len(decoded) % 4, 4):
        value = struct.unpack_from("<I", decoded, offset)[0] ^ key
        struct.pack_into("<I", decoded, offset, value)
    return bytes(decoded)


def _decode_fixed_text(data: bytes) -> str:
    return data.split(b"\0", 1)[0].decode("latin-1").rstrip()


def parse_protracker_mod(data: bytes) -> dict[str, object]:
    if len(data) < 1084:
        raise ValueError("MOD is shorter than a 31-sample ProTracker header")
    signature = data[1080:1084]
    channels_by_signature = {
        b"M.K.": 4,
        b"M!K!": 4,
        b"4CHN": 4,
        b"6CHN": 6,
        b"8CHN": 8,
    }
    channels = channels_by_signature.get(signature)
    if channels is None:
        raise ValueError(f"unsupported MOD signature {signature!r}")
    song_length = data[950]
    if not 1 <= song_length <= 128:
        raise ValueError(f"invalid MOD song length {song_length}")
    pattern_table = data[952:1080]
    pattern_count = max(pattern_table[:song_length]) + 1
    sample_bytes = 0
    samples: list[dict[str, object]] = []
    for index in range(31):
        offset = 20 + index * 30
        name = _decode_fixed_text(data[offset : offset + 22])
        length = int.from_bytes(data[offset + 22 : offset + 24], "big") * 2
        sample_bytes += length
        samples.append({"number": index + 1, "name": name, "bytes": length})
    pattern_bytes = pattern_count * 64 * channels * 4
    expected_size = 1084 + pattern_bytes + sample_bytes
    if len(data) != expected_size:
        raise ValueError(
            f"MOD structure derives 0x{expected_size:x} bytes, got 0x{len(data):x}"
        )
    return {
        "title": _decode_fixed_text(data[:20]),
        "signature": signature.decode("ascii"),
        "channels": channels,
        "song_length": song_length,
        "restart": data[951],
        "patterns": pattern_count,
        "pattern_bytes": pattern_bytes,
        "sample_bytes": sample_bytes,
        "derived_size": expected_size,
        "samples": samples,
    }


def parse_ps2_icon(data: bytes) -> dict[str, object]:
    if len(data) < 20:
        raise ValueError("PS2 icon header is truncated")
    file_id, shapes, texture_type, reserved, vertices = struct.unpack_from("<5I", data)
    if file_id != 0x00010000 or reserved != 0x3F800000:
        raise ValueError("PS2 icon header constants are invalid")
    if not shapes or not vertices or vertices % 3:
        raise ValueError("PS2 icon has invalid shape/vertex counts")

    offset = 20
    positions: list[tuple[float, float, float]] = []
    normals: list[tuple[float, float, float]] = []
    texcoords: list[tuple[float, float]] = []
    per_vertex_size = (shapes + 2) * 8
    geometry_end = offset + vertices * per_vertex_size
    if geometry_end + 20 > len(data):
        raise ValueError("PS2 icon geometry is truncated")
    for _ in range(vertices):
        x, y, z, _unknown = struct.unpack_from("<4h", data, offset)
        positions.append((x / 4096.0, y / 4096.0, z / 4096.0))
        offset += shapes * 8
        nx, ny, nz, _unknown = struct.unpack_from("<4h", data, offset)
        normals.append((nx / 4096.0, ny / 4096.0, nz / 4096.0))
        offset += 8
        u, v, _color = struct.unpack_from("<hhI", data, offset)
        texcoords.append((u / 4096.0, v / 4096.0))
        offset += 8

    anim_tag, frame_length, anim_speed, play_offset, frame_count = struct.unpack_from(
        "<IIfII", data, offset
    )
    offset += 20
    frames: list[dict[str, object]] = []
    for _ in range(frame_count):
        if offset + 8 > len(data):
            raise ValueError("PS2 icon animation frame is truncated")
        shape_id, key_count = struct.unpack_from("<II", data, offset)
        offset += 8
        keys: list[tuple[float, float]] = []
        for _ in range(key_count):
            if offset + 8 > len(data):
                raise ValueError("PS2 icon animation key is truncated")
            keys.append(struct.unpack_from("<ff", data, offset))
            offset += 8
        frames.append({"shape": shape_id, "keys": keys})

    if texture_type > 7:
        raise ValueError("compressed PS2 icon textures are not supported by this target extractor")
    texture_size = 128 * 128 * 2
    if offset + texture_size != len(data):
        raise ValueError(
            f"PS2 icon texture boundary mismatch: 0x{offset:x} + 0x{texture_size:x} "
            f"!= 0x{len(data):x}"
        )
    return {
        "file_id": file_id,
        "shapes": shapes,
        "texture_type": texture_type,
        "vertices": vertices,
        "triangles": vertices // 3,
        "animation_tag": anim_tag,
        "frame_length": frame_length,
        "animation_speed": anim_speed,
        "play_offset": play_offset,
        "frames": frames,
        "texture_offset": offset,
        "texture_width": 128,
        "texture_height": 128,
        "positions": positions,
        "normals": normals,
        "texcoords": texcoords,
    }


def rgba_from_ps2_icon(data: bytes, info: dict[str, object]) -> bytes:
    texture_offset = int(info["texture_offset"])
    rgba = bytearray()
    for (value,) in struct.iter_unpack("<H", data[texture_offset:]):
        rgba.extend(
            (
                _expand_5(value & 0x1F),
                _expand_5((value >> 5) & 0x1F),
                _expand_5((value >> 10) & 0x1F),
                255,
            )
        )
    return bytes(rgba)


def ps2_icon_obj(info: dict[str, object]) -> str:
    positions = info["positions"]
    normals = info["normals"]
    texcoords = info["texcoords"]
    assert isinstance(positions, list)
    assert isinstance(normals, list)
    assert isinstance(texcoords, list)
    lines = [
        "# Extracted from the SNES Station v0.23 Memory Card icon",
        "mtllib snes_emu.mtl",
        "o snes_emu_memory_card_icon",
    ]
    lines.extend(f"v {x:.8f} {y:.8f} {z:.8f}" for x, y, z in positions)
    lines.extend(f"vt {u:.8f} {v:.8f}" for u, v in texcoords)
    lines.extend(f"vn {x:.8f} {y:.8f} {z:.8f}" for x, y, z in normals)
    lines.append("usemtl snes_emu_texture")
    for index in range(0, len(positions), 3):
        one, two, three = index + 1, index + 2, index + 3
        lines.append(f"f {one}/{one}/{one} {two}/{two}/{two} {three}/{three}/{three}")
    return "\n".join(lines) + "\n"


def _png_chunk(kind: bytes, payload: bytes) -> bytes:
    return (
        struct.pack(">I", len(payload))
        + kind
        + payload
        + struct.pack(">I", zlib.crc32(kind + payload) & 0xFFFFFFFF)
    )


def png_rgba(width: int, height: int, rgba: bytes) -> bytes:
    if len(rgba) != width * height * 4:
        raise ValueError("RGBA byte count does not match image dimensions")
    stride = width * 4
    scanlines = b"".join(
        b"\0" + rgba[row * stride : (row + 1) * stride] for row in range(height)
    )
    return (
        b"\x89PNG\r\n\x1a\n"
        + _png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
        + _png_chunk(b"IDAT", zlib.compress(scanlines, 9))
        + _png_chunk(b"IEND", b"")
    )


def rgba_on_background(rgba: bytes, background: tuple[int, int, int]) -> bytes:
    composited = bytearray()
    back_r, back_g, back_b = background
    for red, green, blue, alpha in struct.iter_unpack("4B", rgba):
        inverse = 255 - alpha
        composited.extend(
            (
                (red * alpha + back_r * inverse + 127) // 255,
                (green * alpha + back_g * inverse + 127) // 255,
                (blue * alpha + back_b * inverse + 127) // 255,
                255,
            )
        )
    return bytes(composited)


def _write(output_root: Path, relative: str, data: bytes) -> Path:
    path = output_root / relative
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return path


def _json_safe(value: object) -> object:
    if isinstance(value, dict):
        return {str(key): _json_safe(item) for key, item in value.items()}
    if isinstance(value, list):
        return [_json_safe(item) for item in value]
    if isinstance(value, tuple):
        return [_json_safe(item) for item in value]
    return value


def extract_assets(image: bytes, output_root: Path) -> dict[str, object]:
    if len(image) != REFERENCE_SIZE:
        raise ValueError(f"reference size mismatch: expected {REFERENCE_SIZE}, got {len(image)}")
    image_digest = sha256(image)
    if image_digest != REFERENCE_SHA256:
        raise ValueError(
            f"reference SHA-256 mismatch: expected {REFERENCE_SHA256}, got {image_digest}"
        )

    output_root.mkdir(parents=True, exist_ok=True)
    metadata: list[dict[str, object]] = []
    written: list[Path] = []

    for spec in ASSETS:
        data = image_slice(image, spec)
        stored_size = validate_size_word(image, spec)
        item: dict[str, object] = {
            "name": spec.name,
            "kind": spec.kind,
            "va_start": f"0x{spec.va:08x}",
            "va_end": f"0x{spec.end_va:08x}",
            "size": spec.size,
            "sha256": spec.sha256,
            "size_word_va": (
                f"0x{spec.size_word_va:08x}" if spec.size_word_va is not None else None
            ),
            "stored_size": stored_size,
        }

        if spec.kind == "IRX ELF32":
            item["format"] = parse_irx_elf(data)
            written.append(_write(output_root, spec.output, data))
        elif spec.kind == "IIF1 GS texture":
            info = parse_iif(data)
            item["format"] = info
            written.append(_write(output_root, spec.output, data))
            preview = str(Path(spec.output).with_suffix(".png"))
            written.append(
                _write(
                    output_root,
                    preview,
                    png_rgba(info["width"], info["height"], rgba_from_iif(data, info)),
                )
            )
        elif spec.kind == "BFNT GS font":
            info = parse_bfnt(data)
            item["format"] = info
            written.append(_write(output_root, spec.output, data))
            preview = str(Path(spec.output).with_suffix(".png"))
            font_rgba = rgba_from_bfnt(data, info)
            written.append(
                _write(
                    output_root,
                    preview,
                    png_rgba(info["width"], info["height"], font_rgba),
                )
            )
            written.append(
                _write(
                    output_root,
                    "graphics/frontend_font_preview.png",
                    png_rgba(
                        info["width"],
                        info["height"],
                        rgba_on_background(font_rgba, (32, 40, 48)),
                    ),
                )
            )
        elif spec.kind == "XOR-obfuscated text":
            decoded = decode_xor_words(data)
            decoded_digest = sha256(decoded)
            if decoded_digest != spec.decoded_sha256:
                raise ValueError(
                    f"{spec.name}: decoded SHA-256 mismatch: expected "
                    f"{spec.decoded_sha256}, got {decoded_digest}"
                )
            decoded.decode("ascii")
            item["decoded_sha256"] = decoded_digest
            written.append(_write(output_root, spec.output, decoded))
        elif spec.kind == "ProTracker MOD":
            info = parse_protracker_mod(data)
            item["format"] = info
            item["runtime_aligned_load_size"] = (spec.size + 15) & ~15
            written.append(_write(output_root, spec.output, data))
        elif spec.kind == "PS2 Memory Card 3D icon":
            info = parse_ps2_icon(data)
            public_info = {
                key: value
                for key, value in info.items()
                if key not in {"positions", "normals", "texcoords"}
            }
            item["format"] = public_info
            written.append(_write(output_root, spec.output, data))
            texture_png = png_rgba(
                int(info["texture_width"]),
                int(info["texture_height"]),
                rgba_from_ps2_icon(data, info),
            )
            written.append(
                _write(output_root, "memory-card/snes_emu_texture.png", texture_png)
            )
            written.append(
                _write(
                    output_root,
                    "memory-card/snes_emu.obj",
                    ps2_icon_obj(info).encode("utf-8"),
                )
            )
            written.append(
                _write(
                    output_root,
                    "memory-card/snes_emu.mtl",
                    b"newmtl snes_emu_texture\nKd 1.0 1.0 1.0\nmap_Kd snes_emu_texture.png\n",
                )
            )
        else:
            raise AssertionError(f"unhandled asset kind {spec.kind}")
        metadata.append(item)

    readme = (
        "SNES Station v0.23 private extracted assets\n"
        "===========================================\n\n"
        "These files were reproduced from your SHA-256-verified SNES_EMU.ELF.\n"
        "Do not commit this directory or redistribute its binary contents.\n"
        "The repository contains only offsets, sizes, hashes and extraction code.\n\n"
        "The .ico file is a PlayStation 2 Memory Card 3D icon, not a Windows icon.\n"
        "Its texture and OBJ conversion are provided beside it for inspection.\n"
    ).encode("utf-8")
    written.append(_write(output_root, "README.txt", readme))

    sums = []
    for path in sorted(written):
        relative = path.relative_to(output_root).as_posix()
        sums.append(f"{sha256(path.read_bytes())}  {relative}")
    _write(output_root, "SHA256SUMS.txt", ("\n".join(sums) + "\n").encode("ascii"))

    manifest = {
        "schema": 1,
        "reference": {
            "base": f"0x{REFERENCE_BASE:08x}",
            "size": REFERENCE_SIZE,
            "sha256": REFERENCE_SHA256,
        },
        "assets": _json_safe(metadata),
    }
    _write(
        output_root,
        "manifest.json",
        (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode("utf-8"),
    )
    return manifest


def print_summary(assets: Iterable[dict[str, object]], output_root: Path) -> None:
    print(f"embedded assets: OK ({sum(1 for _ in assets)} validated ranges)")
    for asset in assets:
        print(
            f"  {asset['name']:<27} {asset['va_start']}..{asset['va_end']} "
            f"{int(asset['size']):>7} bytes"
        )
    print(f"private output: {output_root}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--input",
        type=Path,
        default=ROOT / "build" / "SNES_EMU.unpacked.bin",
        help="SHA-256-verified unpacked memory image",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "build" / "extracted-assets",
        help="private output directory (must remain ignored by Git)",
    )
    args = parser.parse_args()
    try:
        image = args.input.read_bytes()
        manifest = extract_assets(image, args.output)
    except (OSError, UnicodeError, ValueError) as exc:
        raise SystemExit(f"asset extraction failed: {exc}") from exc
    print_summary(manifest["assets"], args.output)


if __name__ == "__main__":
    main()
