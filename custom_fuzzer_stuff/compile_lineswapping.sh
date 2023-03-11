

#gcc -c -Wall -O3 Line_Swapping.c -ggdb -g -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/utils/custom_mutators -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/examples/custom_mutators -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Line_Swapping.o
gcc -c -Wall -O3 Line_Swapping.c -ggdb -g -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/utils/custom_mutators -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/examples/custom_mutators -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Line_Swapping.o

gcc -c -Wall -O3 Bruteforce_tests.c -ggdb -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/utils/custom_mutators -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/examples/custom_mutators -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Bruteforce_tests.o
gcc -c -Wall -O3 Mutators_aux.c -ggdb -g -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/utils/custom_mutators -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/examples/custom_mutators -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Mutators_aux.o
gcc -shared -Wall -g -ggdb -O3 Mutators_aux.o Bruteforce_tests.o Line_Swapping.o -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/utils/custom_mutators -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/examples/custom_mutators -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o lineswapping.so


