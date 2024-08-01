set width 0
set height 0
set verbose off
set print pretty on
set print address off
set loggin file ./debug/logfile.log
set logging on
shell date > ./debug/logfile.log

target remote localhost:3333
b main
continue
b 75
continue
print "Registers after init functions:"
print "GPIOC"
print /t *GPIOC
print "USART 3"
print /t *USART3
print "RCC"
print /t *RCC

monitor shutdown
quit
y
    
