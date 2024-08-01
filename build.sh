#!/bin/sh

# atualiza o repositorio de pacotes e instala o compilador gcc
apk update &&
    apk add gcc && apk add g++

# compilacao do arquivo csv.c para um arquivo objeto csv.o
gcc -c -Wall -Werror -fpic csv.c

# cria uma biblioteca compartilhada libcsv.so apartir do objeto csv.o
gcc -shared -o libcsv.so csv.o

# declara o caminho de busca das bibliotecas
LD_LIBRARY_PATH="$PWD"/csv:$LD_LIBRARY_PATH

# exporta a variavel LD_LIBRARY_PATH,tornando-a visivel, para outros programas
export LD_LIBRARY_PATH="$PWD"/csv:$LD_LIBRARY_PATH
