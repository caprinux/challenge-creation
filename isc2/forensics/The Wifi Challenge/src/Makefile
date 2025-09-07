all:
	gcc -s client.c -o client
	upx client
	perl -pi -e 's/UPX/APT/g' client
	gcc server.c -o server
