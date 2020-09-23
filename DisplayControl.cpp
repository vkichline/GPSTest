#include "DisplayControl.h"

DisplayControl::DisplayControl(DisplayDefinition& d) : sprite(&M5.Lcd) {
  // Font metrics for free fonts are implemented, but protected. Duplicating code here:
  ascent  = 0;
  descent = 0;
  // Find the biggest above and below baseline offsets
  for (uint8_t c = 0; c < (d.font->last - d.font->first); c++) {
    GFXglyph *glyph  = &d.font->glyph[c];
    int8_t g_ascent  = -glyph->yOffset;
    if (g_ascent > ascent)   ascent = g_ascent;
    int8_t g_decent = glyph->height - g_ascent;
    if (g_decent > descent) descent = g_decent;
  }
  uint16_t height = ascent + descent + d.margin_top + d.margin_bottom + d.border_width * 2;
  outer_rect      = { d.position.x, d.position.y, d.width, height };
  inner_rect      = outer_rect;
  inner_rect.x   += (d.margin_left + d.border_width);
  inner_rect.y   += (d.margin_top  + d.border_width);
  inner_rect.w   -= (d.margin_left + d.margin_right  + d.border_width * 2);
  inner_rect.h   -= (d.margin_top  + d.margin_bottom + d.border_width * 2);
  bg_color        = d.bg_color;
  fg_color        = d.fg_color;
  font            = d.font;
  align_kind      = d.align;
  border_width    = d.border_width;
  border_color    = d.border_color;
  shorten_to_fit  = d.shorten_to_fit;
  use_elipsis     = d.use_elipsis;
  precision       = d.precision;
  int_formatter   = d.int_formatter;
  float_formatter = d.float_formatter;
  clear();
}


// Initialize values
//
void DisplayControl::clear() {
  value_kind      = NoValue;
  text_value      = "";
  int_value       = 0;
  float_value     = 0.0;
  is_unsigned     = false;
  changed         = true;
}


// Height is set automatically; this allows access to it.
//
uint16_t DisplayControl::get_height() {
  return outer_rect.h;
}


// Return the old border color
//
uint32_t DisplayControl::set_border_color(uint32_t color) {
  uint32_t temp = border_color;
  border_color  = color;
  return temp;
}


// Get the current border color
//
uint32_t DisplayControl::get_border_color(void) {
  return border_color;
}


// Return the old border width
//
uint8_t DisplayControl::set_border_width(uint8_t width) {
  uint8_t temp  = border_width;
  border_width  = width;
  return temp;
}


// Get the current border width
//
uint8_t DisplayControl::get_border_width(void) {
  return border_width;
}


// Return the old font
//
const GFXfont* DisplayControl::set_font(const GFXfont* font) {
  const GFXfont* temp = this->font;
  this->font          = font;
  return temp;
}


// Return the current font
//
const GFXfont* DisplayControl::get_font(void) {
  return font;
}


// Return the old alignment
//
AlignKind DisplayControl::set_alignment(AlignKind align) {
  AlignKind temp    = align_kind;
  this->align_kind  = align;
  return temp;
}


 // Get the current alignment
 //
AlignKind DisplayControl::get_alignment(void) {
  return align_kind;
}


// Set the formatter for integer values. Int values become TextValues
//
void DisplayControl::set_int_format(const char* format) {
  if(nullptr == format) {
    if(0 < int_formatter.length()) changed = true;
    int_formatter = "";
    return;
  }
  if(int_formatter != format) changed = true;
  int_formatter = format;
  if(IntValue == value_kind) {
    sprintf(buffer, format, int_value);
    text_value = buffer;
    value_kind = TextValue;
    changed    = true;
  }
}

// Set the formatter for double values. Float values become TextValues
//
void DisplayControl::set_float_format(const char* format) {
  if(nullptr == format) {
    if(0 < float_formatter.length()) changed = true;
    float_formatter = "";
    return;
  }
  if(float_formatter != format) changed = true;
  float_formatter = format;
  if(FloatValue == value_kind) {
    sprintf(buffer, format, float_value);
    text_value = buffer;
    value_kind = TextValue;
    changed    = true;
  }
}


// Set the value to text, make it a TextValue
//
void DisplayControl::set_value(String value) {
  if(text_value != value || TextValue != value_kind) changed = true;
  text_value = value;
  value_kind = TextValue;
}


// Set the value to text, make it a TextValue
//
void DisplayControl::set_value(const char* value) {
  if(text_value != String(value) || TextValue != value_kind) changed = true;
  text_value = value;
  value_kind = TextValue;
}


// Set the value to int, making it an IntValue (unless int_formatter is set)
//
void DisplayControl::set_value(int32_t value) {
  if(0 == int_formatter.length()) {
    if(int_value != value|| IntValue != value_kind) changed = true;
    int_value   = value;
    value_kind  = IntValue;
  }
  else {
    sprintf(buffer, int_formatter.c_str(), value);
    if(text_value != String(value) || TextValue != value_kind) changed = true;
    text_value     = buffer;
    value_kind     = TextValue;
    is_unsigned    = false;
  }
}


// Set the value to unsigned int, making it an IntValue (unless int_formatter is set)
//
void DisplayControl::set_value(uint32_t value) {
  if(0 == int_formatter.length()) {
    if(int_value != value|| IntValue != value_kind) changed = true;
    int_value     = value;
    value_kind    = IntValue;
    is_unsigned   = true;
  }
  else {
    sprintf(buffer, int_formatter.c_str(), value);
    if(text_value != String(value) || TextValue != value_kind) changed = true;
    text_value = buffer;
    value_kind = TextValue;
  }
}


// Set the value to double, making it an FloatValue (unless float_formatter is set)
//
void DisplayControl::set_value(double value) {
  if(0 == float_formatter.length()) {
    if(float_value != value || FloatValue != value_kind) changed = true;
    float_value = value;
    value_kind  = FloatValue;
  }
  else {
    sprintf(buffer, float_formatter.c_str(), value);
    if(text_value != String(value) || TextValue != value_kind) changed = true;
    text_value = buffer;
    value_kind = TextValue;
  }
}


// Make a rectangle smaller in all directions by the amount provided.
// Reduction is limited so that the rect does not become negative.
// Return true if any reduction was done.
//
bool DisplayControl::inset_rect(DispRect& r, uint16_t amount) {
  if(amount * 2 > r.w) amount = r.w / 2;
  if(amount * 2 > r.h) amount = r.h / 2;
  if(0 == amount) return false;
  r.x += amount;
  r.y += amount;
  r.w -= (amount * 2);
  r.h -= (amount * 2);
  return true;
}


// Draw the control, flicker free, without overdrawing the boundaries
// TBD:
// Flicker free
//
void DisplayControl::draw() {
  if(changed) {
    String value;
    changed = false;
    sprite.createSprite(outer_rect.w, outer_rect.h);
    DispRect display_rect(outer_rect);
    display_rect.x = display_rect.y = 0;

    // draw the background
    sprite.fillRect(display_rect.x, display_rect.y, display_rect.w, display_rect.h, bg_color);
    // draw the content
    sprite.setFreeFont(font);
    sprite.setTextColor(fg_color, bg_color);
    switch(value_kind) {
      case NoValue: break;
      case TextValue:
        value = text_value;
        break;
      case IntValue:
        value = String(is_unsigned ? (uint32_t)int_value : int_value);
        break;
      case FloatValue:
        sprintf(buffer, "%.*f", precision, float_value);
        value = String(buffer);
        break;
    }

    // Trim the string to fit if required.
    if(shorten_to_fit) {
      uint16_t elip_width = sprite.textWidth("...");
      if(use_elipsis && elip_width > inner_rect.w) {
        value = "";
      }
      else while(inner_rect.w < sprite.textWidth(value)) {
        if(use_elipsis) {
          if(3 < value.length() && value.lastIndexOf("...") == (value.length() - 3)) {
             value.remove(value.length() - 4);
           }
          else {
           value.remove(value.length() - 1);
          }
          value += "...";
        }
        else {
          value.remove(value.length() - 1);
        }
      }
    }

    // Apply horizontal adjustment for alignment and draw the text
    uint16_t x    = inner_rect.x - outer_rect.x;
    uint16_t wid  = sprite.textWidth(value);
    switch(align_kind) {
      case AlignLeft: break;
      case AlignCenter:
        x = (inner_rect.w - wid) / 2;
        break;
      case AlignRight:
        x = inner_rect.w - wid;
        break;
    }
    sprite.setCursor(x, (inner_rect.y - outer_rect.y) + ascent);
    sprite.print(value);

    // If there's a border, draw it (overdrawing text overflow)
    for(uint8_t i = 0; i < border_width; i++) {
      sprite.drawRect(display_rect.x, display_rect.y, display_rect.w, display_rect.h, border_color);
      inset_rect(display_rect, 1);
    }

    // Render to the screen and free the memory
    sprite.pushSprite(outer_rect.x, outer_rect.y);
    sprite.deleteSprite();
  }
}
