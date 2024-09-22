set width 0
set height 0
set verbose off
set print pretty on
set print address off
set logging file ./build/debug/gdb.log
set logging on
shell date > ./build/debug/gdb.log

target remote localhost:3333
b Test
continue

print /x *SPI1
print /x *GPIOB

monitor shutdown
quit
y
