#pragma once
#include <M5Stack.h>

// This control is intended to display flicker-free values with zero over-drawing
// using higher quality fonts.
// String, int and double values can be accommodated, as well as custom formatting.
//
// By Van Kichline
// In the year of the plague

// What If I make it so you specify the upper-left corner and width, and it selects the height
// (bumping up if we hit the bottom.) A function acn return the height.
// Make margin exclude border, so a border of 3 and a margin of 4 adds 14 to width and height.

typedef struct DispPoint {
  uint16_t  x;
  uint16_t  y;
} DispPoint;

typedef struct DispRect {
  uint16_t  x;
  uint16_t  y;
  uint16_t  w;
  uint16_t  h;
} DispRect;

typedef enum AlignKind { AlignLeft, AlignCenter, AlignRight } AlignKind;

typedef struct DisplayDefinition {
  DispPoint       position;
  uint16_t        width;
  const GFXfont*  font;
  uint16_t        margin_left     = 4;
  uint16_t        margin_top      = 2;
  uint16_t        margin_right    = 4;
  uint16_t        margin_bottom   = 2;
  AlignKind       align           = AlignLeft;
  uint32_t        fg_color        = WHITE;
  uint32_t        bg_color        = BLACK;
  uint8_t         border_width    = 0;
  uint32_t        border_color    = BLACK;
  bool            shorten_to_fit  = true;
  bool            use_elipsis     = true;
  uint8_t         precision       = 2;
  String          int_formatter   = "";
  String          float_formatter = "";
} DisplayDefinition;

typedef enum ValueKind { NoValue, TextValue, IntValue, FloatValue } ValueKind;


class DisplayControl {
  public:
    DisplayControl(DisplayDefinition& definition);
    void                  clear();                                  // Initialize values
    void                  draw();                                   // Flicker-free drawing w/o overflow
    uint16_t              get_height();                             // Height is set automatically; this allows access to it.
    uint32_t              set_border_color(uint32_t color);         // Return the old border color
    uint32_t              get_border_color(void);                   // Get the current border color
    uint8_t               set_border_width(uint8_t width);          // Return the old border width
    uint8_t               get_border_width(void);                   // Get the current border width
    AlignKind             set_alignment(AlignKind align);           // Return the old alignment
    AlignKind             get_alignment(void);                      // Get the current alignment
    const GFXfont*        set_font(const GFXfont* font);            // Return the old font
    const GFXfont*        get_font(void);                           // Return the current font
    void                  set_int_format(const char* format);       // Set the formatter for integer values. Int values become TextValues
    void                  set_float_format(const char* format);     // Set the formatter for double values. Float values become TextValues
    void                  set_value(String value);                  // Set the value to text, make it a TextValue
    void                  set_value(const char* value);             // Set the value to text, make it a TextValue
    void                  set_value(int32_t value);                 // Set the value to int, making it an IntValue (unless int_formatter is set)
    void                  set_value(uint32_t value);                // Set the value to unsigned int, making it an IntValue (unless int_formatter is set)
    void                  set_value(double value);                  // Set the value to double, making it an FloatValue (unless float_formatter is set)
  protected:
    bool                  inset_rect(DispRect& r, uint16_t amount); // Return true if the rectangle was changed. Do not go negative.
    TFT_eSprite           sprite;                                   // (Unallocated) sprite for flicker-free drawing
    bool                  shorten_to_fit;                           // If true, truncate the text to fit neatly in the control
    bool                  use_elipsis;                              // If true, alsway include ... at end when shortening, unless ... does not fit
    bool                  is_unsigned;                              // Set to true when an uint32_t value is set
    char                  buffer[64];                               // General purpose utility buffer
    ValueKind             value_kind;                               // What kind of value is currently set (none, text, int or float)
    AlignKind             align_kind;                               // Alignment: left, center or right
    DispRect              outer_rect;                               // Location and extent of the control
    DispRect              inner_rect;                               // Calculated Rect that the value belongs inside of
    uint32_t              bg_color;
    uint32_t              fg_color;
    uint8_t               border_width;
    uint32_t              border_color;
    uint8_t               precision;                                // Number of digits after the decimal in floating-point number
    const GFXfont*        font;
    uint8_t               ascent;                                   // Number of pixels the font extends above the baseline
    uint8_t               descent;                                  // Number of pixels the font extends below the baseline

    bool                  changed;                                  // Set when value is changed, cleared when drawn
    String                int_formatter;                            // printf format string to format integer values
    String                float_formatter;                          // printf format string to format floating point values
    String                text_value;                               // text value if value_kind == TextValue
    int32_t               int_value;                                // integral value if value_kind == IntValue
    double                float_value;                              // floating point value if value_kind == FloatValue
};
