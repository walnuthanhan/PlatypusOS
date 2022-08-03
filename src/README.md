Question 4: Add restoreFile(char *fname) that will restore deleted file. However, because the first character is 0x00, there's no way to check it, thus the program will assume that users put the correct into, and only check for the 5 last characters of filename
Shell command: restore <fname>
