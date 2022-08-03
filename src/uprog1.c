main()
{
	enableInterrupts();
	interrupt(0x21, 0, "WooHoo! I'm a user program!\r\n\0", 0, 0);
	while(1);
}
