#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#ifndef _PAINT_H
#define _PAINT_H

#include "../rct2/addresses.h"
#include "../common.h"
#include "../world/map.h"
#include "../interface/colour.h"
#include "../drawing/drawing.h"

// Global for paint clipping height.
extern uint8 gClipHeight;


typedef struct attached_paint_struct attached_paint_struct;
typedef struct paint_struct paint_struct;
typedef union paint_entry paint_entry;

#pragma pack(push, 1)
/* size 0x12 */
struct attached_paint_struct {
    uint32 image_id;        // 0x00
    union {
        uint32 tertiary_colour;
        // If masked image_id is masked_id
        uint32 colour_image_id;
    };
    uint16 x;       // 0x08
    uint16 y;       // 0x0A
    uint8 flags;    // 0x0C
    uint8 pad_0D;
    attached_paint_struct* next;    //0x0E
};
#ifdef PLATFORM_32BIT
// TODO: drop packing from this when all rendering is done.
assert_struct_size(attached_paint_struct, 0x12);
#endif

enum PAINT_QUADRANT_FLAGS {
    PAINT_QUADRANT_FLAG_IDENTICAL = (1 << 0),
    PAINT_QUADRANT_FLAG_BIGGER = (1 << 7),
    PAINT_QUADRANT_FLAG_NEXT = (1 << 1),
};

/* size 0x34 */
struct paint_struct {
    uint32 image_id;        // 0x00
    union {
        uint32 tertiary_colour; // 0x04
        // If masked image_id is masked_id
        uint32 colour_image_id; // 0x04
    };
    uint16 bound_box_x;     // 0x08
    uint16 bound_box_y;     // 0x0A
    uint16 bound_box_z; // 0x0C
    uint16 bound_box_z_end; // 0x0E
    uint16 bound_box_x_end; // 0x10
    uint16 bound_box_y_end; // 0x12
    uint16 x;               // 0x14
    uint16 y;               // 0x16
    uint16 quadrant_index;
    uint8 flags;
    uint8 quadrant_flags;
    attached_paint_struct* attached_ps; //0x1C
    paint_struct* var_20;
    paint_struct* next_quadrant_ps; // 0x24
    uint8 sprite_type;      //0x28
    uint8 var_29;
    uint16 pad_2A;
    uint16 map_x;           // 0x2C
    uint16 map_y;           // 0x2E
    rct_map_element *mapElement; // 0x30 (or sprite pointer)
};
#ifdef PLATFORM_32BIT
// TODO: drop packing from this when all rendering is done.
assert_struct_size(paint_struct, 0x34);
#endif

typedef struct paint_string_struct paint_string_struct;

/* size 0x1E */
struct paint_string_struct {
    rct_string_id string_id;        // 0x00
    paint_string_struct *next;      // 0x02
    uint16 x;                       // 0x06
    uint16 y;                       // 0x08
    uint32 args[4];                 // 0x0A
    uint8 *y_offsets;               // 0x1A
};
#ifdef PLATFORM_32BIT
assert_struct_size(paint_string_struct, 0x1e);
#endif
#pragma pack(pop)

union paint_entry{
    paint_struct basic;
    attached_paint_struct attached;
    paint_string_struct string;
};

typedef struct sprite_bb {
    uint32 sprite_id;
    rct_xyz16 offset;
    rct_xyz16 bb_offset;
    rct_xyz16 bb_size;
} sprite_bb;

enum PAINT_STRUCT_FLAGS {
    PAINT_STRUCT_FLAG_IS_MASKED = (1 << 0)
};

typedef struct support_height {
    uint16 height;
    uint8 slope;
    uint8 pad;
} support_height;

typedef struct tunnel_entry {
    uint8 height;
    uint8 type;
} tunnel_entry;

#define MAX_PAINT_QUADRANTS 512
#define TUNNEL_MAX_COUNT    65

typedef struct paint_session
{
    rct_drawpixelinfo *     Unk140E9A8;
    paint_entry             PaintStructs[4000];
    paint_struct *          Quadrants[MAX_PAINT_QUADRANTS];
    uint32                  QuadrantBackIndex;
    uint32                  QuadrantFrontIndex;
    void *                  CurrentlyDrawnItem;
    paint_entry *           EndOfPaintStructArray;
    paint_entry *           NextFreePaintStruct;
    rct_xy16                SpritePosition;
    paint_struct            UnkF1A4CC;
    paint_struct *          UnkF1AD28;
    attached_paint_struct * UnkF1AD2C;
    uint8                   InteractionType;
    support_height          SupportSegments[9];
    support_height          Support;
    paint_string_struct *   PSStringHead;
    paint_string_struct *   LastPSString;
    paint_struct *          WoodenSupportsPrependTo;
    rct_xy16                MapPosition;
    tunnel_entry            LeftTunnels[TUNNEL_MAX_COUNT];
    uint8                   LeftTunnelCount;
    tunnel_entry            RightTunnels[TUNNEL_MAX_COUNT];
    uint8                   RightTunnelCount;
    uint8                   VerticalTunnelHeight;
    rct_map_element *       SurfaceElement;
    bool                    DidPassSurface;
    uint8                   Unk141E9DB;
    uint16                  Unk141E9DC;
    uint32                  TrackColours[4];
} paint_session;

extern paint_session gPaintSession;

#ifdef NO_RCT2
#define gTrackColours               gPaintSession.TrackColours
#else
#define gPaintStructs               RCT2_ADDRESS(0x00EE788C, paint_entry)
#define g_currently_drawn_item      RCT2_GLOBAL(0x009DE578, void*)
#define gEndOfPaintStructArray      RCT2_GLOBAL(0x00EE7880, paint_entry *)
#define gPaintSpritePosition        RCT2_GLOBAL(0x009DE568, rct_xy16)
#define gPaintInteractionType       RCT2_GLOBAL(RCT2_ADDRESS_PAINT_SETUP_CURRENT_TYPE, uint8)
#define gSupportSegments            RCT2_ADDRESS(RCT2_ADDRESS_CURRENT_SUPPORT_SEGMENTS, support_height)
#define gSupport                    RCT2_GLOBAL(RCT2_ADDRESS_CURRENT_PAINT_TILE_MAX_HEIGHT, support_height)
#define gWoodenSupportsPrependTo    RCT2_GLOBAL(0x009DEA58, paint_struct *)
#define gPaintMapPosition           RCT2_GLOBAL(0x009DE574, rct_xy16)
#define gLeftTunnels                RCT2_ADDRESS(0x009E3138, tunnel_entry)
#define gLeftTunnelCount            RCT2_GLOBAL(0x0141F56A, uint8)
#define gRightTunnels               RCT2_ADDRESS(0x009E30B6, tunnel_entry)
#define gRightTunnelCount           RCT2_GLOBAL(0x0141F56B, uint8)
#define gVerticalTunnelHeight       RCT2_GLOBAL(0x009E323C, uint8)
#define gSurfaceElement             RCT2_GLOBAL(0x009E3250, rct_map_element *)
#define gDidPassSurface             RCT2_GLOBAL(0x009DE57C, bool)
#define g141E9DB                    RCT2_GLOBAL(0x0141E9DB, uint8)
#define gUnk141E9DC                 RCT2_GLOBAL(0x0141E9DC, uint16)
#define gTrackColours   RCT2_ADDRESS(0x00F44198, uint32)
#endif

/** rct2: 0x00993CC4 */
extern const uint32 construction_markers[];
extern bool gShowDirtyVisuals;
extern bool gPaintBoundingBoxes;

paint_struct * sub_98196C(uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, uint32 rotation);
paint_struct * sub_98197C(uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z, uint32 rotation);
paint_struct * sub_98198C(uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z, uint32 rotation);
paint_struct * sub_98199C(uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z, uint32 rotation);

paint_struct * sub_98196C_rotated(uint8 direction, uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset);
paint_struct * sub_98197C_rotated(uint8 direction, uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z);
paint_struct * sub_98199C_rotated(uint8 direction, uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z);

void paint_util_push_tunnel_rotated(uint8 direction, uint16 height, uint8 type);

bool paint_attach_to_previous_attach(uint32 image_id, uint16 x, uint16 y);
bool paint_attach_to_previous_ps(uint32 image_id, uint16 x, uint16 y);
void paint_floating_money_effect(money32 amount, rct_string_id string_id, sint16 y, sint16 z, sint8 y_offsets[], sint16 offset_x, uint32 rotation);

void paint_init(rct_drawpixelinfo * dpi);
void paint_generate_structs(rct_drawpixelinfo * dpi);
paint_struct paint_arrange_structs();
void paint_draw_structs(rct_drawpixelinfo * dpi, paint_struct * ps, uint32 viewFlags);
void paint_draw_money_structs(rct_drawpixelinfo * dpi, paint_string_struct * ps);

// TESTING
#ifdef __TESTPAINT__
    void testpaint_clear_ignore();
    void testpaint_ignore(uint8 direction, uint8 trackSequence);
    void testpaint_ignore_all();
    bool testpaint_is_ignored(uint8 direction, uint8 trackSequence);

    #define TESTPAINT_IGNORE(direction, trackSequence) testpaint_ignore(direction, trackSequence)
    #define TESTPAINT_IGNORE_ALL() testpaint_ignore_all()
#else
    #define TESTPAINT_IGNORE(direction, trackSequence)
    #define TESTPAINT_IGNORE_ALL()
#endif

#endif
