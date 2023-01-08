Memory maps `/dev/fb0`, then provides a very simple REPL to draw to it.

Commands are:

- `move <x> <y>`: moves the cursor to x, y
- `color <r> <g> <b>`: sets the drawing color
- `rect <width> <height>`: draws a rectangle with those dimensions, with top
  left corner at the cursor
- `exit`: exits
