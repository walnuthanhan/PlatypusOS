

main()
{
	int i=0;
	int j=0;
	int k=0;
	
	enableInterrupts();
	
	for(i=0; i<1000; i++) {
		
		interrupt(0x21, 0x00, "### uprog7 ###\n\r\0",0,0);
		yield();
	}
	
	interrupt(0x21, 0x05, 0, 0, 0); 

}
