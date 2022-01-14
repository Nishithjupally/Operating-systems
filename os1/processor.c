 #include <unistd.h>
 #include <sys/syscall.h>
 #include <sys/types.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <sys/utsname.h>

 int main()
 {
	
	FILE *pf1,*pf2;
	int i;
    struct utsname info;
    i = uname (&info);
    if(!i) 
      {
  	    printf ("Operating System : %s\n",info.sysname);
  	    printf ("Kernel Version: %s\n",info.release);
  	    printf ("OS Version: %s\n",info.version);
      }

	pf1 = popen ("inxi -C","r"); //using inix command
	char outputcpu[1024];
	fgets (outputcpu,1024,pf1);
	printf ("%s\n",outputcpu);
	
	return 0;


 } 

