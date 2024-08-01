#include "libcsv.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

// constantes

#define BUFFER_SIZE 4096   // Tamanho utilizado para armazenar os dados do arquivo lido
#define COMPARATOR_SIZE 3  // Define o tamanho do comparador usado na estrutura FilterStruct
#define FILTER_FAILURE 4   // Código de erro retornado em caso de falha na filtragem
#define INITIAL_COUNT 0    // Valor inicial padrão para contadores e índices
#define MAX_CELL_SIZE 256  // Tamanho máximo de uma célula individual em uma estrutura CsvStruct
#define MAX_VALUE_SIZE 100 // Tamanho máximo do valor armazenado na estrutura FilterStruct

// estruturas

typedef struct
{
    char columns[MAX_CELL_SIZE][MAX_CELL_SIZE]; // Colunas do CSV
    char row[MAX_CELL_SIZE][MAX_CELL_SIZE];     // Linhas do CSV
    int columQty;                               // Quantidade de colunas
    int rowQty;                                 // Quantidade de linhas
} CsvStruct;

typedef struct
{
    char selected_header[256][256]; // CabeC'alhos selecionados para filtragem
    char comparators[100][3];       // Comparadores usados na filtragem (ex: ">", "<", "==")
    char values[100][256];          // Valores usados na comparaC'C#o durante a filtragem
    int filter_size;                // Tamanho do filtro aplicado
} FilterStruct;

typedef struct
{
    int values[100];
    int size;
} SelectedRowStruct;

// declaracao das funcoes

int matchFilter(char[], char *comparator, char[]);

void printSelectedData(bool boolValue, const char *headers, ...);

FilterStruct processFilter(const char[]);

// definicao das funcoes

int matchFilter(char *value, char *comparator, char *contest)
{
    switch (comparator[0])
    {
    case '<':
        if (comparator[1] == '=')
        {
            return (strcmp(value, contest) <= 0);
        }
        return (strcmp(value, contest) < 0);
    case '>':
        if (comparator[1] == '=')
        {
            return (strcmp(value, contest) >= 0);
        }
        return (strcmp(value, contest) > 0);
    case '=':
        if (comparator[1] == '=')
        {
            return 4;
        }
        return (strcmp(value, contest) == 0);
    case '!':
        if (comparator[1] == '=')
        {
            return (strcmp(value, contest) != 0);
        }
        break;
    }
    return 4;
}

void printSelectedData(bool boolValue, const char *headers, ...)
{
    va_list args;
    va_start(args, headers);

    CsvStruct data = va_arg(args, CsvStruct);
    SelectedRowStruct selc_idx = va_arg(args, SelectedRowStruct);

    const char *pad = "";
    if (boolValue)
    {
        for (int k = INITIAL_COUNT; k < data.columQty; k++)
        {
            if (strstr(headers, data.columns[k]) != NULL)
            {
                printf("%s%s", pad, data.columns[k]);
                pad = ",";
            }
        };
    }
    else
    {
        for (int h = INITIAL_COUNT; h < selc_idx.size; h++)
        {
            pad = "";
            for (int t = INITIAL_COUNT; t < data.columQty; t++)
            {
                if (strstr(headers, data.columns[t]) != NULL)
                {

                    printf("%s%s", pad, data.row[(selc_idx.values[h] * data.columQty) + t]);
                    pad = ",";
                }
            }
            putchar('\n');
        }
    }
    putchar('\n');
    va_end(args);
}

FilterStruct processFilter(const char *filters)
{
    FilterStruct filters_data;
    filters_data.filter_size = 0;
    char filter_copy[strlen(filters) + 1];
    char *filter_token_temp;
    char comparator[3];
    char value[256];

    strcpy(filter_copy, filters);

    filter_token_temp = strtok(filter_copy, "\n");

    while (filter_token_temp)
    {
        sscanf(filter_token_temp, "%[^<>=#?$*&@+!]%c%s", filters_data.selected_header[filters_data.filter_size], &comparator[0], value);
        if (value[0] == '=')
        {
            comparator[1] = value[0];
            strcpy(filters_data.values[filters_data.filter_size], value + 1);
        }
        else
        {
            comparator[1] = '\0';
            strcpy(filters_data.values[filters_data.filter_size], value);
        }
        strcpy(filters_data.comparators[filters_data.filter_size], comparator);
        filters_data.filter_size++;
        filter_token_temp = strtok(NULL, "\n");
    }

    return filters_data;
}

// metodo processCsv

void processCsv(const char *csvData, const char *headers, const char *filters)
{
    // declaracao e inicializacao das variaveis

    char csv_copy[strlen(csvData) + 1];
    char csv_rows_copy[strlen(csvData) + 1];
    char allowed_columns_copy[strlen(headers) + 1];
    char *token;
    char *allowed_token;

    strcpy(csv_copy, csvData);
    strcpy(csv_rows_copy, csvData);
    strcpy(allowed_columns_copy, headers);

    CsvStruct data;                                     // estrutura que armazena os dados do CSV
    FilterStruct filters_data = processFilter(filters); // estrutura que armazena os dados do filtro
    SelectedRowStruct temp_row_values;                  // estrutura que armazena os valores das linhas filtradas temporariamente sendo 0 para que não bateram com o filtro e 1 para que bateram
    SelectedRowStruct seleted_row;                      // estrutura que armazena os indices das linhas que passaram na filtragem
    int headers_filtered[MAX_CELL_SIZE];                // array com os indices das colunas que serao filtradas

    int index_sel = INITIAL_COUNT; // inicializa um contador para os indices das colunas filtradas
    // int column_matched = INITIAL_COUNT;     // inicializa um contador referente as colunas verificas pelo filtro
    // int column_not_matched = INITIAL_COUNT; // inicializa um contador para contabilizar as colunas nao passaram na verificacao do filtro

    data.columQty = INITIAL_COUNT;        // inicializa a quantidade de colunas
    data.rowQty = INITIAL_COUNT;          // inicializa a quantidade de linhas
    temp_row_values.size = INITIAL_COUNT; // inicializa a quantidade de valores das linhas filtradas
    seleted_row.size = INITIAL_COUNT;     // inicializa a quantidade de linhas selecionadas

    // processamento dos dados do CSV

    strtok(csv_copy, "\n");

    token = strtok(csv_copy, ",");

    while (token)
    {
        strcpy(data.columns[data.columQty++], token);

        token = strtok(NULL, ",");
    }

    // verifica se as colunas selecionadas existem no CSV

    allowed_token = strtok(allowed_columns_copy, ",");

    while (allowed_token)
    {
        int hl = INITIAL_COUNT;
        for (int k = INITIAL_COUNT; k < data.columQty; k++)
        {
            if (strcmp(allowed_token, data.columns[k]) == 0)
            {
                hl = 1;
                break;
            }
        }
        if (hl == 0)
        {
            fprintf(stderr, "Header '%s' not found in CSV file/string", allowed_token);
            exit(EXIT_FAILURE);
        }

        allowed_token = strtok(NULL, ",");
    }

    free(allowed_token);

    // adicao das linhas no array de linhas

    token = strtok(csv_rows_copy, "\n");
    token = strtok(NULL, ",");

    while (token)
    {
        strcpy(data.row[data.rowQty++], token);
        token = strtok(NULL, ",\n");
    }

    free(token);

    // verifica se as colunas filtradas existem no CSV

    for (int w = INITIAL_COUNT; w < filters_data.filter_size; w++)
    {
        headers_filtered[w] = -1; // fornece um indice invalido para as colunas filtradas afim de verificar a existencia das mesmas
        for (int z = INITIAL_COUNT; z < data.columQty; z++)
        {
            if (strcmp(filters_data.selected_header[w], data.columns[z]) == 0) // verifica se a coluna filtrada existe no CSV
            {
                headers_filtered[w] = z; // armazena o indice da coluna filtrada
                break;
            }
        }
        if (headers_filtered[w] == -1)
        {
            fprintf(stderr, "Header '%s' not found in CSV file/string", filters_data.selected_header[w]);
            exit(EXIT_FAILURE);
        }
    }

    // verifica depois depois de validadas se as colunas filtradas existem no CSV e armazena os indices das colunas filtradas

    int row_limit = data.rowQty / data.columQty;
    int not_repeated_filter_size = filters_data.filter_size;
    int counter = INITIAL_COUNT;

    for (int i = INITIAL_COUNT; i < filters_data.filter_size; i++)
    {
        if (headers_filtered[i] == headers_filtered[i - 1] && i > 0)
        {
            not_repeated_filter_size--;
        }
    }

    // printf("not repeated filter size %d\n", not_repeated_filter_size);

    for (int i = INITIAL_COUNT; i < filters_data.filter_size; i++)
    {
        index_sel = INITIAL_COUNT;                         // inicializa o contador de indices das linhas filtradas
        int j = headers_filtered[i];                       // indice da coluna filtrada
        char *curr_fil_comp = filters_data.comparators[i]; // comparador atual
        if (headers_filtered[i] == headers_filtered[i - 1] && i > 0)
        {
            counter = counter - not_repeated_filter_size;
        }
        while (index_sel < row_limit)
        {
            char *curr_col_val = data.row[index_sel * data.columQty + j];                        // valor atual da coluna filtrada
            int match_result = matchFilter(curr_col_val, curr_fil_comp, filters_data.values[i]); // resultado da comparacao

            if (match_result == FILTER_FAILURE) // verifica validade do filtro
            {
                fprintf(stderr, "Invalid filter: '%s%s%s'", filters_data.selected_header[i], curr_fil_comp, filters_data.values[i]);
                exit(EXIT_FAILURE);
            }

            temp_row_values.values[temp_row_values.size++] = match_result; // adiciona o indice da linha no array de linhas selecionadas
            if (temp_row_values.values[counter] == 0 && match_result == 1)
            {
                temp_row_values.values[counter] = match_result;
            }

            index_sel++;
            counter++;
        }
    }

    int set = 0;
    int temp_counter = 0;
    while (set < row_limit)
    {
        for (int u = 0; u < not_repeated_filter_size; u++)
        {
            int selected_row_value = temp_row_values.values[u * row_limit + set];
            if (selected_row_value == 0)
            {
                break;
            }

            temp_counter += selected_row_value;
        }
        if (temp_counter == not_repeated_filter_size)
        {
            seleted_row.values[seleted_row.size++] = set;
        }

        temp_counter = 0;
        set++;
    }

    if (seleted_row.size == 0)
    {
        fprintf(stderr, "No row value matched");
        exit(EXIT_FAILURE);
    }

    printSelectedData(true, headers, data);               // imprime os cabeçalhos selecionados
    printSelectedData(false, headers, data, seleted_row); // imprime as linhas selecionadas
}

// metodo processCsvFile

void processCsvFile(const char *csvFilePath, const char *headers, const char *filters)
{
    char buffer[BUFFER_SIZE];                      // buffer de 4096 para armazenar os dados do arquivo
    FILE *csv_file_data = fopen(csvFilePath, "r"); // abre o arquivo no modo leitura

    if (csv_file_data == NULL) // verifica se o caminho do arquivo é valido
    {
        fprintf(stderr, "File not found.");
        exit(EXIT_FAILURE);
    }

    char data[BUFFER_SIZE] = ""; // buffer para armazenar a string dos dados do arquivo

    while (fgets(buffer, sizeof(buffer), csv_file_data)) // leitura dos dados do arquivo
    {
        strcat(data, buffer); // concatena os dados do arquivo no buffer
    };

    fclose(csv_file_data); // fecha o arquivo

    processCsv(data, headers, filters); // processa os dados do arquivo
}