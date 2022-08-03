

main()
{
	int i=0;
	int j=0;
	int k=0;
	
	enableInterrupts();
	
	for(i=0; i<200; i++) {
		
		interrupt(0x21, 0x00, "+++++++++++++++++++++++++++++++++++++ uprog6+++++++++++++++++++++++++++++\n\r\0",0,0);
		//print("+++ uprog6 +++ \n\r\0");
		
		// Give up 5 time slices...
		j=0;
		while(j<5) {
			j++;
			yield();
		}
	}
	
	interrupt(0x21, 0x05, 0, 0, 0); 

}
