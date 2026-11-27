#!/usr/bin/env python3
"""Genera los iconos de Teleprompter a partir de design/teleprompter.tokens.json.

Escribe en resources/app-icons/ el icono de la aplicación (teleprompter.svg, .png, .ico y .icns) y en
resources/ui/<tema>/check.svg la marca de las casillas de cada tema. Los colores salen de los tokens:
no se editan aquí.

Requiere Pillow. Uso, desde la raíz del repositorio:

    python3 design/build_icons.py
"""

import json
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parent.parent
TOKENS = json.loads((ROOT / "design" / "teleprompter.tokens.json").read_text(encoding="utf-8"))
DARK = TOKENS["themes"]["dark"]

# Lienzo del icono y factor de supermuestreo: se dibuja ampliado y se reduce para suavizar los bordes.
SIZE = 512
SCALE = 4

# Tamaños de las imágenes que contienen los archivos .ico y .icns.
ICO_SIZES = [(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)]
ICNS_SIZES = [(16, 16), (32, 32), (64, 64), (128, 128), (256, 256), (512, 512)]

# Geometría, en unidades del lienzo de 512: pantalla, línea de lectura y soporte.
CORNER = 112
SCREEN = (72, 120, 440, 350)
SCREEN_RADIUS = 22
STROKE = 22
BAND = (100, 212, 412, 258)
BAND_RADIUS = 10
BAND_ALPHA = 46
LINE_WIDTH = 20
LINES = [(170, 181, 390), (170, 235, 340), (170, 289, 370)]  # (x1, y, x2); la central es la resaltada
POINTER = [(112, 217), (140, 235), (112, 253)]
STAND = [(256, 350, 256, 392), (196, 404, 316, 404)]


def rgb(token):
    value = DARK[token].lstrip("#")

    return tuple(int(value[i : i + 2], 16) for i in (0, 2, 4))


def build_png():
    bg, frame, text, accent = rgb("bg"), rgb("fg-muted"), rgb("fg"), rgb("accent")
    s = lambda v: int(round(v * SCALE))
    image = Image.new("RGBA", (SIZE * SCALE, SIZE * SCALE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)

    def line(x1, y1, x2, y2, width, color):
        draw.line((s(x1), s(y1), s(x2), s(y2)), fill=color, width=s(width))

        # Los extremos redondeados se dibujan como círculos.
        for x, y in ((x1, y1), (x2, y2)):
            r = width / 2
            draw.ellipse((s(x - r), s(y - r), s(x + r), s(y + r)), fill=color)

    draw.rounded_rectangle((0, 0, s(SIZE) - 1, s(SIZE) - 1), radius=s(CORNER), fill=bg)
    draw.rounded_rectangle([s(v) for v in SCREEN], radius=s(SCREEN_RADIUS), outline=frame, width=s(STROKE))

    # La franja de lectura es translúcida, así que va en su propia capa.
    band = Image.new("RGBA", image.size, (0, 0, 0, 0))
    ImageDraw.Draw(band).rounded_rectangle([s(v) for v in BAND], radius=s(BAND_RADIUS), fill=accent + (BAND_ALPHA,))
    image = Image.alpha_composite(image, band)
    draw = ImageDraw.Draw(image)

    for i, (x1, y, x2) in enumerate(LINES):
        line(x1, y, x2, y, LINE_WIDTH, accent if i == 1 else text + (217,))

    draw.polygon([(s(x), s(y)) for x, y in POINTER], fill=accent)

    for x1, y1, x2, y2 in STAND:
        line(x1, y1, x2, y2, STROKE, frame)

    return image.resize((SIZE, SIZE), Image.LANCZOS)


def build_svg():
    bg, frame, text, accent = DARK["bg"], DARK["fg-muted"], DARK["fg"], DARK["accent"]
    x1, y1, x2, y2 = SCREEN
    bx1, by1, bx2, by2 = BAND
    lines = "".join(
        f'<line x1="{a}" y1="{y}" x2="{b}" y2="{y}" stroke="{accent if i == 1 else text}"' + ("" if i == 1 else ' stroke-opacity=".85"') + "/>"
        for i, (a, y, b) in enumerate(LINES)
    )
    stand = "".join(f'<line x1="{a}" y1="{b}" x2="{c}" y2="{d}"/>' for a, b, c, d in STAND)
    pointer = " ".join(f"{x} {y}" for x, y in POINTER)

    return (
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {SIZE} {SIZE}" width="{SIZE}" height="{SIZE}">'
        f'<rect width="{SIZE}" height="{SIZE}" rx="{CORNER}" fill="{bg}"/>'
        f'<rect x="{x1}" y="{y1}" width="{x2 - x1}" height="{y2 - y1}" rx="{SCREEN_RADIUS}" fill="none" stroke="{frame}" stroke-width="{STROKE}"/>'
        f'<rect x="{bx1}" y="{by1}" width="{bx2 - bx1}" height="{by2 - by1}" rx="{BAND_RADIUS}" fill="{accent}" fill-opacity=".18"/>'
        f'<g stroke-width="{LINE_WIDTH}" stroke-linecap="round">{lines}</g>'
        f'<path d="M{pointer}Z" fill="{accent}"/>'
        f'<g stroke="{frame}" stroke-width="{STROKE}" stroke-linecap="round">{stand}</g>'
        "</svg>\n"
    )


def build_check(theme):
    return (
        '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 12 12" width="12" height="12">'
        f'<path d="M2.5 6.4L5 8.9l4.5-5.3" fill="none" stroke="{TOKENS["themes"][theme]["accent-text"]}" '
        'stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round"/></svg>\n'
    )


def main():
    icons = ROOT / "resources" / "app-icons"
    png = build_png()

    png.save(icons / "teleprompter.png")
    png.save(icons / "teleprompter.ico", sizes=ICO_SIZES)
    png.save(icons / "teleprompter.icns", sizes=ICNS_SIZES)
    (icons / "teleprompter.svg").write_text(build_svg(), encoding="utf-8")

    for theme in ("dark", "light"):
        (ROOT / "resources" / "ui" / theme / "check.svg").write_text(build_check(theme), encoding="utf-8")


if __name__ == "__main__":
    main()
