#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <emmintrin.h>
#include <tmmintrin.h>


typedef struct Pixel
{
  uint8_t a;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} __attribute__((packed)) pixel_t;

typedef struct BitMapFileHeader
{
  char type[2];
  uint32_t size;
  uint32_t reserved;
  uint32_t offset_bits;
} __attribute__((packed)) bmfh_t;

typedef struct BitMapInfoHeader
{
  uint32_t size;
  int32_t width;
  int32_t height;
  uint16_t planes;
  uint16_t bit_count;
  uint32_t compression;
  uint32_t image_size;
  uint32_t xppm;
  uint32_t yppm;
  uint32_t colors_used;
  uint32_t colors_important;
} __attribute__((packed)) bmih_t;

typedef struct BitMapPicture
{
  bmfh_t bmp_header;
  bmih_t dib_header;
  pixel_t *content;
} bmp_t;


extern void
parse_bmp_file (int fd, bmp_t *bmp)
{
  read(fd, &bmp->bmp_header, sizeof(bmp->bmp_header));
  read(fd, &bmp->dib_header, sizeof(bmp->dib_header));
  bmp->content = (pixel_t *) malloc(bmp->dib_header.image_size);
  lseek(fd, (__off_t) bmp->bmp_header.offset_bits, SEEK_SET);
  read(fd, bmp->content, bmp->dib_header.image_size);
}

extern void
overlay_bmp (bmp_t *font_bmp, bmp_t *layer_bmp, bmp_t *result_bmp)
{
  result_bmp->bmp_header = font_bmp->bmp_header;
  result_bmp->dib_header = font_bmp->dib_header;
  result_bmp->content = font_bmp->content;
  result_bmp->content = (pixel_t *) malloc(result_bmp->dib_header.image_size);

  __m128i_u mask_lo = _mm_setzero_si128();
  mask_lo[0] = 0x00;
  mask_lo[1] = 0x80;
  mask_lo[2] = 0x01;
  mask_lo[3] = 0x81;
  mask_lo[4] = 0x02;
  mask_lo[5] = 0x82;
  mask_lo[6] = 0x03;
  mask_lo[7] = 0x83;
  mask_lo[8] = 0x04;
  mask_lo[9] = 0x84;
  mask_lo[10] = 0x05;
  mask_lo[11] = 0x85;
  mask_lo[12] = 0x06;
  mask_lo[13] = 0x86;
  mask_lo[14] = 0x07;
  mask_lo[15] = 0x87;
  __m128i_u mask_hi = _mm_setzero_si128();
  mask_hi[0] = 0x08;
  mask_hi[1] = 0x80;
  mask_hi[2] = 0x09;
  mask_hi[3] = 0x81;
  mask_hi[4] = 0x0A;
  mask_hi[5] = 0x82;
  mask_hi[6] = 0x0B;
  mask_hi[7] = 0x83;
  mask_hi[8] = 0x0C;
  mask_hi[9] = 0x84;
  mask_hi[10] = 0x0D;
  mask_hi[11] = 0x85;
  mask_hi[12] = 0x0E;
  mask_hi[13] = 0x86;
  mask_hi[14] = 0x0F;
  mask_hi[15] = 0x87;
  __m128i_u zero_quad = _mm_setzero_si128();
  __m128i_u quad256 = _mm_setzero_si128();
  quad256[0] = quad256[1] = quad256[2] = quad256[3] = 256;

  int i = 0;
  /*
    _mm_setzero_si128   - Return vector of type __m128i with all elements set to zero.
    _mm_set1_epi16      - Broadcast 16-bit integer a to all all elements of dst. This intrinsic may generate vpbroadcastw.
    _mm_set_epi8        - Set packed 8-bit integers in dst with the supplied values in reverse order.
    _mm_loadu_si128     - Load 128-bits of integer data from memory into dst. mem_addr does not need to be aligned on any particular boundary.
    _mm_unpacklo_epi8   - Unpack and interleave 8-bit integers from the low half of a and b, and store the results in dst.
    _mm_unpackhi_epi8   - Unpack and interleave 8-bit integers from the high half of a and b, and store the results in dst.
    _mm_shuffle_epi8    - Shuffle packed 8-bit integers in a according to shuffle control mask in the corresponding 8-bit element of b, and store the results in dst.
    _mm_mulhi_epu16     - Multiply the packed unsigned 16-bit integers in a and b, producing intermediate 32-bit integers, and store the high 16 bits of the intermediate integers in dst.
    _mm_packus_epi16    - Convert packed 16-bit integers from a and b to packed 8-bit integers using unsigned saturation, and store the results in dst.
    _mm_sub_epi16       - Subtract packed 16-bit integers in b from packed 16-bit integers in a, and store the results in dst.
    _mm_adds_epu8       - Add packed unsigned 8-bit integers in a and b using saturation, and store the results in dst.
    _mm_storeu_si128    - Store 128-bits of integer data from a into memory. mem_addr does not need to be aligned on any particular boundary.
   */
  /*
  for (; (i + 4) <= result_bmp->dib_header.image_size / sizeof(pixel_t); i += 4)
  {
    __m128i_u font_quad = _mm_loadu_si128((const __m128i_u *) (font_bmp->content + i));
    __m128i_u layer_quad = _mm_loadu_si128((const __m128i_u *) (layer_bmp->content + i));
    __m128i_u lo_font_quad = _mm_unpacklo_epi8(font_quad, zero_quad);
    __m128i_u hi_font_quad = _mm_unpacklo_epi8(font_quad, zero_quad);
    __m128i_u lo_layer_quad = _mm_unpacklo_epi8(layer_quad, zero_quad);
    __m128i_u hi_layer_quad = _mm_unpacklo_epi8(layer_quad, zero_quad);
    lo_font_quad = _mm_shuffle_epi8(lo_font_quad, mask_lo);
    hi_font_quad = _mm_shuffle_epi8(hi_font_quad, mask_hi);
    lo_layer_quad = _mm_shuffle_epi8(lo_layer_quad, mask_lo);
    hi_layer_quad = _mm_shuffle_epi8(hi_layer_quad, mask_hi);

    __m128i_u result_quad = _mm_sub_epi16(layer_quad, font_quad);

    _mm_storeu_si128((__m128i_u *) (result_bmp->content + i), result_quad);
  }
   */
  for (; i < result_bmp->dib_header.image_size / sizeof(pixel_t); i++)
  {
    pixel_t pixel_font = font_bmp->content[i];
    pixel_t pixel_layer = layer_bmp->content[i];
    pixel_t pixel_result = {
        255,
        (uint8_t) (pixel_font.r + ((int) (pixel_layer.r - pixel_font.r) * pixel_layer.a >> 8)),
        (uint8_t) (pixel_font.g + ((int) (pixel_layer.g - pixel_font.g) * pixel_layer.a >> 8)),
        (uint8_t) (pixel_font.b + ((int) (pixel_layer.b - pixel_font.b) * pixel_layer.a >> 8))
    };
    result_bmp->content[i] = pixel_result;
  }
}

extern void
free_bmp (bmp_t *bmp)
{
  free(bmp->content);
}

extern void
store_bmp_file (int origin_fd, int fd, bmp_t *bmp)
{
  lseek(origin_fd, 0, SEEK_SET);
  void *buffer = malloc((size_t) bmp->bmp_header.offset_bits);
  read(origin_fd, buffer, (size_t) bmp->bmp_header.offset_bits);
  write(fd, buffer, (size_t) bmp->bmp_header.offset_bits);
  free(buffer);
  write(fd, bmp->content, bmp->dib_header.image_size);
}


int
main (int argc, char *argv[])
{
  int font_fd = open(argv[1], O_RDONLY);
  int layer_fd = open(argv[2], O_RDONLY);
  int result_fd = open(argv[3], O_WRONLY | O_CREAT, 0644);

  bmp_t font_bmp, layer_bmp, result_bmp;

  parse_bmp_file(font_fd, &font_bmp);
  parse_bmp_file(layer_fd, &layer_bmp);

  overlay_bmp(&font_bmp, &layer_bmp, &result_bmp);

  store_bmp_file(font_fd, result_fd, &result_bmp);

  free_bmp(&font_bmp);
  free_bmp(&layer_bmp);
  free_bmp(&result_bmp);

  close(font_fd);
  close(layer_fd);
  close(result_fd);
  return 0;
}

/*
    __m128i_u lhs_font_quad, rhs_font_quad, lhs_layer_quad, rhs_layer_quad;
    __m128i_u mask_lhs = _mm_setzero_si128();
    mask_lhs[0] = 0x00;
    mask_lhs[1] = 0x80;
    mask_lhs[2] = 0x01;
    mask_lhs[3] = 0x81;
    mask_lhs[4] = 0x02;
    mask_lhs[5] = 0x82;
    mask_lhs[6] = 0x03;
    mask_lhs[7] = 0x83;
    mask_lhs[8] = 0x04;
    mask_lhs[9] = 0x84;
    mask_lhs[10] = 0x05;
    mask_lhs[11] = 0x85;
    mask_lhs[12] = 0x06;
    mask_lhs[13] = 0x86;
    mask_lhs[14] = 0x07;
    mask_lhs[15] = 0x87;
    __m128i_u mask_rhs = _mm_setzero_si128();
    mask_rhs[0] = 0x08;
    mask_rhs[1] = 0x80;
    mask_rhs[2] = 0x09;
    mask_rhs[3] = 0x81;
    mask_rhs[4] = 0x0A;
    mask_rhs[5] = 0x82;
    mask_rhs[6] = 0x0B;
    mask_rhs[7] = 0x83;
    mask_rhs[8] = 0x0C;
    mask_rhs[9] = 0x84;
    mask_rhs[10] = 0x0D;
    mask_rhs[11] = 0x85;
    mask_rhs[12] = 0x0E;
    mask_rhs[13] = 0x86;
    mask_rhs[14] = 0x0F;
    mask_rhs[15] = 0x87;
 */