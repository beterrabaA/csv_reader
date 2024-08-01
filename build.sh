#!/bin/sh

# compilacao do arquivo csv.c para um arquivo objeto csv.o
gcc -c -Wall -Werror -fpic csv.c

# cria uma biblioteca compartilhada libcsv.so apartir do objeto csv.o
gcc -shared -o libcsv.so csv.o

# compilacao do arquivo main.c e integracao da biblioteca compartilhada libcsv
gcc -L. -Wall -o main main.c -lcsv

# declara o caminho de busca das bibliotecas
LD_LIBRARY_PATH="$PWD"/csv:$LD_LIBRARY_PATH

# exporta a variavel LD_LIBRARY_PATH,tornando-a visivel, para outros programas
export LD_LIBRARY_PATH="$PWD"/csv:$LD_LIBRARY_PATH

# executa o programa main
./main