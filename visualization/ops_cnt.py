def cnt_adds(src_width, src_height, block_size, overlap, out_size) -> int:
    w = src_width
    h = src_height
    b = block_size
    o = overlap
    n = out_size

    # corner
    cnt_fist = 9 * (block_size ** 2) + 2 * block_size + 1

    # left
    cnt_left = src_width - block_size + 1
    cnt_left += (src_width - block_size) * (src_height - block_size + 1)
    cnt_left += (src_width - block_size) * (src_height - block_size) * (15 * block_size * overlap + 2 * overlap + 1)
    cnt_left += 12 * (block_size ** 2) + 26 * block_size * overlap + 10 * block_size + 9 * overlap + 3
    cnt_left += 3 * (src_width - block_size) * (src_height - block_size) + 4

    cnt_left *= (n - 1)

    # above
    cnt_above = (w - b + 1) \
                + (w - b) * (h - b + 1) \
                + (w - b) * (h - b) * (15 * b * o + 2 * o + 1) \
                + 12 * b ** 2 \
                + 12 * b * o \
                + 15 * b \
                + 2 * o ** 2 \
                + 9 * o \
                + 4 \
                + 3 * (w - b) * (h - b) \
                + 4

    cnt_above *= (n - 1)

    # corner
    cnt_corner = w - b + 1 \
                 + (w - b) * (h - b + 1) \
                 + (w - b) * (h - b) * (30 * b * o + 2 * b + o - 15 * o ** 2 + 2) \
                 + 25 * b ** 2 \
                 + 38 * b * o \
                 + 25 * b \
                 + 2 * o ** 2 \
                 + 18 * o \
                 + 8 \
                 + 3 * (w - b) * (h - b) \
                 + 4

    cnt_corner *= (n - 1) ** 2

    return cnt_corner + cnt_left + cnt_above + cnt_fist


def cnt_mults(src_width, src_height, block_size, overlap, out_size) -> int:
    w = src_width
    h = src_height
    b = block_size
    o = overlap
    n = out_size

    # corner
    cnt_fist = 3 * b ** 2 + 2 * b + 1

    # left
    cnt_left = (w - b) * (h - b) * 5 * b * o \
               + 3 * b ** 2 \
               + 11 * b * o \
               + 5 * b \
               - 1 \
               + 1

    cnt_left *= (n - 1)

    # above
    cnt_above = (w - b) * (h - b) * 5 * b * o \
                + 3 * b ** 2 \
                + 10 * b * o \
                + 4 * b \
                + o \
                - 1 \
                + 1

    cnt_above *= (n - 1)

    # corner
    cnt_corner = (w - b) * (h - b) * (10 * b * o - 2 * o ** 2) \
                 + 9 * b ** 2 \
                 + 21 * b * o \
                 + 9 * b \
                 + o \
                 - 2 \
                 + 1

    cnt_corner *= (n - 1) ** 2

    return cnt_corner + cnt_left + cnt_above + cnt_fist


def cnt_divs(src_width, src_height, block_size, overlap, out_size) -> int:
    w = src_width
    h = src_height
    b = block_size
    o = overlap
    n = out_size

    # corner
    cnt_corner = 3 * b ** 2
    cnt_corner *= (n - 1) ** 2

    return cnt_corner


def cnt_cmps(src_width, src_height, block_size, overlap, out_size) -> int:
    w = src_width
    h = src_height
    b = block_size
    o = overlap
    n = out_size

    # corner
    cnt_fist = 3 * b ** 2 + 2 * b + 1

    # left
    cnt_left = w - b + 1 \
               + (w - b) * (h - b + 1) \
               + (w - b) * (h - b) * (b * o + 2 * b + 1) \
               + 4 * b ** 2 \
               + 7 * b * o \
               + 18 * b \
               + 6 \
               + 3 * (w - b) * (h - b) \
               + 2

    cnt_left *= (n - 1)

    # above
    cnt_above = w - b + 1 \
                + (w - b) * (h - b + 1) \
                + (w - b) * (h - b) * (b * o + 2 * o + 1) \
                + 3 * b ** 2 \
                + 6 * b * o \
                + 9 * b \
                + o ** 2 \
                + 8 * o \
                + 5 \
                + 3 * (w - b) * (h - b) \
                + 2

    cnt_above *= (n - 1)

    # corner
    cnt_corner = w - b + 1 \
                 + (w - b) * (h - b + 1) \
                 + (w - b) * (h - b) * (b * o + 2 * b - o - o ** 2 + 2) \
                 + 8 * b ** 2 \
                 + 13 * b * o \
                 + 29 * b \
                 + o ** 2 \
                 + 8 * o \
                 + 12 \
                 + 3 * (w - b) * (h - b) \
                 + 2

    cnt_corner *= (n - 1) ** 2

    return cnt_corner + cnt_left + cnt_above + cnt_fist


def cnt_all(src_width, src_height, block_size, overlap, out_size) -> int:
    cnt = cnt_adds(src_width, src_height, block_size, overlap, out_size)
    cnt += cnt_mults(src_width, src_height, block_size, overlap, out_size)
    cnt += cnt_divs(src_width, src_height, block_size, overlap, out_size)
    cnt += cnt_cmps(src_width, src_height, block_size, overlap, out_size)

    return cnt
