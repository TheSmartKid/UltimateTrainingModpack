#ifndef RAYGUN_PRINTER_H
#define RAYGUN_PRINTER_H

void print_char(u64 module_accessor, char to_print, int line_num, float horiz_offset, float facing_left);
void print_string(u64 module_accessor, const char* print_str);

#endif // RAYGUN_PRINTER_H
