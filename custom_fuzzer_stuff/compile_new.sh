rm Mutators_aux.o
rm Other\ tests.o
rm Bruteforce_tests.o
mv bruteforcenew.so backupthing.so

gcc -c -Wall -O3 Other\ tests.c -ggdb -g -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Other\ tests.o
gcc -c -Wall -O3 Bruteforce_tests.c -ggdb -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Bruteforce_tests.o
gcc -c -Wall -O3 Mutators_aux.c -ggdb -g -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -fpic -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/include -o Mutators_aux.o
gcc -shared -Wall -g -ggdb -O3 Mutators_aux.o Other\ tests.o Bruteforce_tests.o -I/home/cyberhacker/Asioita/newaflfuzz/AFLplusplus/custom_mutators/examples -fpic -I/home/cyberhacker/Asioita/Hakkerointi/Counterstrikestuff/aflpp/shit/AFLplusplus/include -o bruteforcenew.so


