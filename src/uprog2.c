main(){
	interrupt(0x21, 0x00, "Interrupt Program\n\r\0", 0, 0);
	interrupt(0x21, 0x05, 0, 0, 0);
}