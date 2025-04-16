gcc `pkg-config --cflags gtk+-3.0` -o Calculator calculator.c `pkg-config --libs gtk+-3.0`
./Calculator