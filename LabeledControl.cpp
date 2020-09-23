#include "LabeledControl.h"

LabeledControl::LabeledControl(LabelDefinition label, DisplayDefinition display) {
  DisplayDefinition label_def = display;
  DisplayDefinition disp_def  = display;
  label_def.width             = label.width;
  label_def.align             = label.align;
  disp_def.position.x        += label.width;
  label_control               = new DisplayControl(label_def);
  display_control             = new DisplayControl(disp_def);
  label_control->set_value(label.label_text);
}


// Initialize values of the display. The label is not changed.
//
void LabeledControl::clear() {
  display_control->clear();
}


// Flicker-free drawing w/o overflow
//
void LabeledControl::draw() {
  label_control->draw();
  display_control->draw();
}


// Height is set automatically; this allows access to it.
//
uint16_t LabeledControl::get_height() {
  return display_control->get_height();
}


// Set the value to text, make it a TextValue
//
void LabeledControl::set_value(String value) {
  display_control->set_value(value);
}


// Set the value to text, make it a TextValue
//
void LabeledControl::set_value(const char* value) {
  display_control->set_value(value);
}


// Set the value to int, making it an IntValue (unless int_formatter is set)
//
void LabeledControl::set_value(int32_t value) {
  display_control->set_value(value);
}


// Set the value to int, making it an IntValue (unless int_formatter is set)
//
void LabeledControl::set_value(uint32_t value) {
  display_control->set_value(value);
}


// Set the value to double, making it an FloatValue (unless float_formatter is set)
//
void LabeledControl::set_value(double value) {
  display_control->set_value(value);
}


// For doing more precise adjustments of the label
//
DisplayControl& LabeledControl::get_label() {
  return *label_control;
}


// For doing more precise adjustments of the display
//
DisplayControl& LabeledControl::get_display() {
  return *display_control;
}
