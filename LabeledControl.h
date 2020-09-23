#pragma once

#include "DisplayControl.h"

typedef struct LabelDefinition {
  String          label_text;
  uint16_t        width;
  AlignKind       align = AlignLeft;
} LabelDefinition;


class LabeledControl {
  public:
    LabeledControl(LabelDefinition label, DisplayDefinition display);
    void            clear();                        // Initialize values
    void            draw();                         // Flicker-free drawing w/o overflow
    uint16_t        get_height();                   // Height is set automatically; this allows access to it.
    void            set_value(String value);        // Set the value to text, make it a TextValue
    void            set_value(const char* value);   // Set the value to text, make it a TextValue
    void            set_value(int32_t value);       // Set the value to int, making it an IntValue (unless int_formatter is set)
    void            set_value(uint32_t value);      // Set the value to int, making it an IntValue (unless int_formatter is set)
    void            set_value(double value);        // Set the value to double, making it an FloatValue (unless float_formatter is set)
    DisplayControl& get_label();                    // For doing more precise adjustments of the label
    DisplayControl& get_display();                  // For doing more precise adjustments of the display
  protected:
    DisplayControl* label_control;
    DisplayControl* display_control;
};
