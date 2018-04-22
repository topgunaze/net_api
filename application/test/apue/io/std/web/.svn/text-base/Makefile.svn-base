dest = hello.cgi time.cgi pic.cgi login.cgi

all : $(dest)
#hello.cgi:hello.c

%.cgi : %.c
	gcc $< -o $@ 

clean:
	rm *.cgi *.o -f

install:
	cp *.cgi /var/www/cgi-bin/
