HtcpcpStandard_build :
	gcc -Wall -ansi -pedantic-errors -c "HtcpcpStandard.c"

test1_build : HtcpcpStandard_build
	gcc -Wall -ansi -pedantic-errors -o test1.out test1.c HtcpcpStandard.o

test1_run : test1_build
	./test1.out

test1_memcheck : test1_build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./test1.out

client_build : HtcpcpStandard_build
	gcc -Wall -ansi -pedantic-errors -o htcpcp_client.out htcpcp_client.c HtcpcpStandard.o

client_run : client_build
	./htcpcp_client.out

client_memcheck : client_build
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./htcpcp_client.out

server_build : HtcpcpStandard_build
	gcc -Wall -ansi -pedantic-errors -o htcpcp_server.out htcpcp_server.c HtcpcpStandard.o

server_run : server_build
	./htcpcp_server.out

server_memcheck : server_run
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./htcpcp_server.out