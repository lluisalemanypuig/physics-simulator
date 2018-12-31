#pragma once

void __psv_exit_func();
void __psv_initGL(int argc, char *argv[]);
void __psv_refresh();
void __psv_timed_refresh(int value);
void __psv_reshape(int w, int h);
void __psv_mouse_click_event(int button, int state, int x, int y);
void __psv_mouse_movement(int x, int y);
void __psv_mouse_drag_event(int x, int y);
void __psv_special_keys(int key, int x, int y);
void __psv_keyboard_event(unsigned char c, int x, int y);

void __psv_interface(int argc, char *argv[]);
