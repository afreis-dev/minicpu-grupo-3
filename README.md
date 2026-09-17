# MiniCPU - Grupo 3

Projeto da atividade **CPU - Torneio de Processadores**, de Infraestrutura de Hardware.
Simulador em **C**, com memória de 256 bytes, quatro registradores e ciclo
Fetch-Decode-Execute. O desafio é calcular **5! = 120 por somas repetidas**.

## Equipe e divisão de tarefas

**Nome da equipe: Grupo 3.**

| Integrante | Responsabilidade |
|---|---|
| Arthur Reis | Implementação e explicação do código |
| Gabriel Peixoto | Testes e conferência do trace |
| Luísa Fischer | Apresentação |
| Sofia Villela | Slides |

Essa é a divisão de responsabilidades. Os testes pessoais de Gabriel ainda devem
ser executados seguindo o guia; o trace incluído é uma execução de verificação do projeto.

## Como executar

É necessário um compilador C, como o GCC. Não há bibliotecas externas.
Abra o terminal na pasta do projeto e compile:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o minicpu
```

No Windows (PowerShell):

```powershell
.\minicpu.exe
```

No Linux/macOS:

```sh
./minicpu
```

A saída mostra uma linha por instrução e termina com:

```text
Resultado: 5! = 120 (MEM[0x20])
```

## Como funciona

1. `carregar_programa` coloca as instruções numéricas e N=5 na memória.
2. `fetch` lê os três bytes da instrução e avança o PC em 3.
3. `decode_execute` identifica o opcode com `switch` e executa a operação.
4. `trace` imprime os registradores, PC, ZF e o resultado na memória.
5. O laço termina quando a instrução `HALT` é executada.

As dez instruções do enunciado estão implementadas: LOAD, STORE, ADD, SUB,
MOV, CMP, JMP, JZ, JNZ e HALT. Apenas CMP altera ZF.

O cálculo começa com resultado 1. Para multiplicar por 5, soma 1 cinco vezes;
depois soma 5 quatro vezes, 20 três vezes, 60 duas vezes e 120 uma vez.
Os resultados parciais são **1 → 5 → 20 → 60 → 120 → 120**.
O fatorial é calculado pelo programa de opcodes, sem multiplicação ou função pronta em C.

## Organização da memória

| Endereço | Uso |
|---|---|
| `0x00` a `0x02` | Instrução JMP para `0x40`; PC começa em 0 |
| `0x10` | Entrada N=5 |
| `0x20` | Resultado final |
| `0x21` | Resultado anterior, usado nas somas |
| `0x22` | Fator atual, preservado durante a multiplicação |
| `0x40` a `0x84` | Programa do fatorial, incluindo HALT |

O salto inicial impede que o código sobrescreva os dados exigidos pelo desafio.
R0 acumula o resultado; R1 conta as somas e recebe o próximo fator;
R2 guarda o valor somado; R3 recebe 0 ou 1 para comparações e decrementos.

Registradores e memória usam `uint8_t` (0 a 255). A aritmética dá a volta a cada
256 valores: por exemplo, 255 + 1 resulta em 0. Portanto, o fatorial exato cabe
somente para N de 0 a 5. Com N=6, a CPU armazena 208, que é 720 módulo 256.
O PC usa um inteiro maior para detectar acesso fora da memória antes de ler os bytes.

## Testes e material

- [Guia da equipe](GUIA_EQUIPE.md): testes para Gabriel e apoio para implementação, apresentação e slides.
- [Trace completo](trace.txt): execução padrão, com 158 ciclos e resultado 120.
- [Relatório em PDF](relatorio.pdf): material para entrega, com o link do GitHub no final.

Verificação técnica: compilação sem avisos, fatorial de 0 a 5, comportamento de
8 bits em N=6, dez opcodes, condições dos saltos e exemplo básico do enunciado.
Gabriel deve registrar a própria execução no checklist do guia.

## Referências

- Material do professor: `CPU_Atividade.pdf`, seções 3 a 6 e desafio do Grupo 3.
- [Intel - The Journey Inside: Microprocessors, lição 2](https://www.intel.com/content/www/us/en/education/k12/the-journey-inside/explore-the-curriculum/microprocessors.html): conceitos de busca, decodificação e execução.
