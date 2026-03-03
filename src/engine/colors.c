// ============================================================================
//  Optimized Color Conversion Utilities (behavior-identical)
// ============================================================================

#include <PR/ultratypes.h>
#include "math_util.h"
#include "colors.h"

// ---------------------------------------------------------------------------
//  Small inline helpers to eliminate repeated macro expansion
// ---------------------------------------------------------------------------

static inline u8 extract_c16(RGBA16 src, u16 mask, u16 idx) {
    return COMPOSITE_TO_COLOR(src, mask, idx);
}

static inline u8 extract_c32(RGBA32 src, u32 mask, u32 idx) {
    return COMPOSITE_TO_COLOR(src, mask, idx);
}

static inline f32 extract_cf16(RGBA16 src, u16 mask, u16 idx) {
    return COMPOSITE_TO_COLORF(src, mask, idx);
}

static inline f32 extract_cf32(RGBA32 src, u32 mask, u32 idx) {
    return COMPOSITE_TO_COLORF(src, mask, idx);
}

// ============================================================================
//  RGBA16 → RGB / RGBA
// ============================================================================

static inline void rgba16_to_colorRGB(ColorRGB dst, RGBA16 src) {
    dst[0] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_R);
    dst[1] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_G);
    dst[2] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_B);
}

static inline void rgba16_to_colorRGBA(ColorRGBA dst, RGBA16 src) {
    dst[0] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_R);
    dst[1] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_G);
    dst[2] = extract_c16(src, MSK_RGBA16_C, IDX_RGBA16_B);
    dst[3] = extract_c16(src, MSK_RGBA16_A, IDX_RGBA16_A);
}

// ============================================================================
//  RGBA32 → RGB / RGBA
// ============================================================================

static inline void rgba32_to_colorRGB(ColorRGB dst, RGBA32 src) {
    dst[0] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_R);
    dst[1] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_G);
    dst[2] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_B);
}

static inline void rgba32_to_colorRGBA(ColorRGBA dst, RGBA32 src) {
    dst[0] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_R);
    dst[1] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_G);
    dst[2] = extract_c32(src, MSK_RGBA32_C, IDX_RGBA32_B);
    dst[3] = extract_c32(src, MSK_RGBA32_A, IDX_RGBA32_A);
}

// ============================================================================
//  RGB / RGBA → RGBA16 / RGBA32
// ============================================================================

static inline RGBA16Return32 colorRGB_to_rgba16(ColorRGB src) {
    return  COLOR_TO_COMPOSITE(src[0], MSK_RGBA16_C, IDX_RGBA16_R)
          | COLOR_TO_COMPOSITE(src[1], MSK_RGBA16_C, IDX_RGBA16_G)
          | COLOR_TO_COMPOSITE(src[2], MSK_RGBA16_C, IDX_RGBA16_B)
          | MSK_RGBA16_A;
}

static inline RGBA16Return32 colorRGBA_to_rgba16(ColorRGBA src) {
    return  COLOR_TO_COMPOSITE(src[0], MSK_RGBA16_C, IDX_RGBA16_R)
          | COLOR_TO_COMPOSITE(src[1], MSK_RGBA16_C, IDX_RGBA16_G)
          | COLOR_TO_COMPOSITE(src[2], MSK_RGBA16_C, IDX_RGBA16_B)
          | COLOR_TO_COMPOSITE(src[3], MSK_RGBA16_A, IDX_RGBA16_A);
}

static inline RGBA32 colorRGB_to_rgba32(ColorRGB src) {
    return  COLOR_TO_COMPOSITE(src[0], MSK_RGBA32_C, IDX_RGBA32_R)
          | COLOR_TO_COMPOSITE(src[1], MSK_RGBA32_C, IDX_RGBA32_G)
          | COLOR_TO_COMPOSITE(src[2], MSK_RGBA32_C, IDX_RGBA32_B)
          | MSK_RGBA32_A;
}

static inline RGBA32 colorRGBA_to_rgba32(ColorRGBA src) {
    return  COLOR_TO_COMPOSITE(src[0], MSK_RGBA32_C, IDX_RGBA32_R)
          | COLOR_TO_COMPOSITE(src[1], MSK_RGBA32_C, IDX_RGBA32_G)
          | COLOR_TO_COMPOSITE(src[2], MSK_RGBA32_C, IDX_RGBA32_B)
          | COLOR_TO_COMPOSITE(src[3], MSK_RGBA32_A, IDX_RGBA32_A);
}

// ============================================================================
//  Float versions
// ============================================================================

static inline void rgba16_to_colorRGBf(ColorRGBf dst, RGBA16 src) {
    dst[0] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_R);
    dst[1] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_G);
    dst[2] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_B);
}

static inline void rgba16_to_colorRGBAf(ColorRGBAf dst, RGBA16 src) {
    dst[0] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_R);
    dst[1] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_G);
    dst[2] = extract_cf16(src, MSK_RGBA16_C, IDX_RGBA16_B);
    dst[3] = extract_cf16(src, MSK_RGBA16_A, IDX_RGBA16_A);
}

static inline void rgba32_to_colorRGBf(ColorRGBf dst, RGBA32 src) {
    dst[0] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_R);
    dst[1] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_G);
    dst[2] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_B);
}

static inline void rgba32_to_colorRGBAf(ColorRGBAf dst, RGBA32 src) {
    dst[0] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_R);
    dst[1] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_G);
    dst[2] = extract_cf32(src, MSK_RGBA32_C, IDX_RGBA32_B);
    dst[3] = extract_cf32(src, MSK_RGBA32_A, IDX_RGBA32_A);
}

static inline RGBA16Return32 colorRGBf_to_rgba16(ColorRGBf src) {
    return  COLORF_TO_COMPOSITE(src[0], MSK_RGBA16_C, IDX_RGBA16_R)
          | COLORF_TO_COMPOSITE(src[1], MSK_RGBA16_C, IDX_RGBA16_G)
          | COLORF_TO_COMPOSITE(src[2], MSK_RGBA16_C, IDX_RGBA16_B)
          | MSK_RGBA16_A;
}

static inline RGBA16Return32 colorRGBAf_to_rgba16(ColorRGBAf src) {
    return  COLORF_TO_COMPOSITE(src[0], MSK_RGBA16_C, IDX_RGBA16_R)
          | COLORF_TO_COMPOSITE(src[1], MSK_RGBA16_C, IDX_RGBA16_G)
          | COLORF_TO_COMPOSITE(src[2], MSK_RGBA16_C, IDX_RGBA16_B)
          | COLORF_TO_COMPOSITE(src[3], MSK_RGBA16_A, IDX_RGBA16_A);
}

static inline RGBA32 colorRGBf_to_rgba32(ColorRGBf src) {
    return  COLORF_TO_COMPOSITE(src[0], MSK_RGBA32_C, IDX_RGBA32_R)
          | COLORF_TO_COMPOSITE(src[1], MSK_RGBA32_C, IDX_RGBA32_G)
          | COLORF_TO_COMPOSITE(src[2], MSK_RGBA32_C, IDX_RGBA32_B)
          | MSK_RGBA32_A;
}

static inline RGBA32 colorRGBAf_to_rgba32(ColorRGBAf src) {
    return  COLORF_TO_COMPOSITE(src[0], MSK_RGBA32_C, IDX_RGBA32_R)
          | COLORF_TO_COMPOSITE(src[1], MSK_RGBA32_C, IDX_RGBA32_G)
          | COLORF_TO_COMPOSITE(src[2], MSK_RGBA32_C, IDX_RGBA32_B)
          | COLORF_TO_COMPOSITE(src[3], MSK_RGBA32_A, IDX_RGBA32_A);
}

// ============================================================================
//  Averaging (unrolled, branchless inside the loop)
// ============================================================================
Bool32 colorRGBA_average_2(ColorRGBA dst, ColorRGBA c1, ColorRGBA c2) {
    u32 a = c1[3] + c2[3];
    if (!a) return FALSE;

    dst[3] = a;
    dst[0] = (c1[0] * c1[3] + c2[0] * c2[3]) / a;
    dst[1] = (c1[1] * c1[3] + c2[1] * c2[3]) / a;
    dst[2] = (c1[2] * c1[3] + c2[2] * c2[3]) / a;
    return TRUE;
}

Bool32 colorRGBA_average_3(ColorRGBA dst, ColorRGBA c1, ColorRGBA c2, ColorRGBA c3) {
    u32 a = c1[3] + c2[3] + c3[3];
    if (!a) return FALSE;

    dst[3] = a;
    dst[0] = (c1[0] * c1[3] + c2[0] * c2[3] + c3[0] * c3[3]) / a;
    dst[1] = (c1[1] * c1[3] + c2[1] * c2[3] + c3[1] * c3[3]) / a;
    dst[2] = (c1[2] * c1[3] + c2[2] * c2[3] + c3[2] * c3[3]) / a;
    return TRUE;
}
