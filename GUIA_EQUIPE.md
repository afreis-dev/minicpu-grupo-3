# Guia da equipe - Grupo 3

## Arthur: entender a implementação

Leia `main.c` nesta ordem: `main`, `fetch`, `decode_execute`, `carregar_programa`
e `trace`. Os comentários ao lado dos bytes mostram a instrução e seu endereço.

- O C simula a CPU. A lista de bytes é o programa que essa CPU executa.
- O `switch` identifica cada opcode e aplica a operação.
- O laço interno (`0x58` a `0x6A`) faz as somas de uma multiplicação.
- O laço externo volta para `0x4F`, usando o próximo fator.
- `0x21` preserva o resultado anterior; `0x22` preserva o fator.
- O salto inicial para `0x40` mantém os dados em `0x10` e `0x20` fora do código.

Exemplo: quando o resultado é 5 e o fator é 4, R0 começa em 0 e recebe
5 quatro vezes: 5, 10, 15, 20. Depois o próximo fator é 3.

## Gabriel: roteiro de testes

Execute estes passos e registre seus próprios resultados. O checklist começa
em branco porque a verificação técnica do projeto não substitui seu teste.

### 1. Compilar

Na pasta do projeto:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o minicpu
.\minicpu.exe
```

Se `gcc` não for reconhecido, é necessário instalar ou configurar um compilador C
no computador. O código também pode ser compilado em um ambiente C da faculdade.

- [ ] Compilou sem erros e sem avisos.
- [ ] A primeira instrução foi `JMP 0x40`, deixando PC=64.
- [ ] A execução terminou sozinha, com `HALT`.
- [ ] O último ciclo foi 158, com R0=120, PC=133, ZF=1 e MEM[0x20]=120.
- [ ] A mensagem final foi `Resultado: 5! = 120 (MEM[0x20])`.

O trace mostra o estado **depois** da instrução. Por isso o PC final é 133:
o HALT começa em 130 e a busca já avançou três posições.

### 2. Conferir o trace

```powershell
.\minicpu.exe > trace-gabriel.txt
Get-Content trace-gabriel.txt -Tail 6
Select-String -Path trace-gabriel.txt -Pattern 'STORE R0, 0x21'
```

Nos cinco `STORE R0, 0x21`, R0 deve mostrar, em ordem: **1, 5, 20, 60, 120**.
O resultado só vai para `0x20` no ciclo 157. Antes disso, ele fica em R0.
No Linux/macOS, use `./minicpu > trace-gabriel.txt` e abra o arquivo no editor.

- [ ] Conferiu os resultados parciais.
- [ ] Conferiu que CMP atualiza ZF e que JNZ repete o laço quando ZF=0.
- [ ] Salvou o trace da própria execução e uma captura do resultado.

### 3. Testar valores menores

No começo de `main.c`, altere apenas `#define N 5`, compile de novo e execute.

| N | Resultado esperado em MEM[0x20] | Resultado observado |
|---|---|---|
| 0 | 1 | |
| 1 | 1 | |
| 2 | 2 | |
| 3 | 6 | |
| 4 | 24 | |
| 5 | 120 | |

Teste opcional: N=6 produz 208 por causa do limite de 8 bits; não representa
o fatorial exato. Não é um erro da soma.

- [ ] Testou de 0 a 5 e preencheu a tabela.
- [ ] Restaurou `#define N 5`, recompilou e confirmou 120 novamente.

Registro de Gabriel: data: ______ / compilador: ______ / observações: ______.

## Luísa: pontos para apresentar em 1 a 3 minutos

1. **Desafio:** calcular 5! por somas repetidas e guardar 120 no endereço 0x20.
2. **Estrutura:** memória, quatro registradores, PC e ZF. Mostrar as funções de busca e execução.
3. **Lógica:** explicar 5 vezes 4 como 5 + 5 + 5 + 5. A CPU não tem MUL.
4. **Demonstração:** executar o programa, mostrar algumas linhas do trace e a última linha.
5. **Fechamento:** comentar o teste de 0! e o limite de 255.

Perguntas prováveis:

- **O que é PC?** É o endereço da próxima instrução; a busca soma 3, e saltos podem mudar esse valor.
- **O que é ZF?** Vale 1 quando CMP encontra valores iguais. Controla JZ e JNZ.
- **Por que começar com 1?** É o valor inicial do produto e também o resultado de 0!.
- **Por que o salto inicial?** Para não misturar os bytes do programa com os endereços dos dados.
- **O trace é tempo real do processador?** Não. Cada linha corresponde a uma instrução simulada.

## Sofia: sugestão de quatro slides

1. **Grupo 3 e desafio:** integrantes, funções e objetivo 5! = 120.
2. **MiniCPU:** quatro registradores, memória de 256 bytes e fluxo Fetch → Decode → Execute.
3. **Somas repetidas:** exemplo 5 + 5 + 5 + 5 = 20 e sequência 1 → 5 → 20 → 60 → 120.
4. **Resultado:** captura da execução, MEM[0x20]=120 e link do repositório.

Use a captura do teste de Gabriel quando ele concluir. Não precisa colocar todo
o código ou as 158 linhas nos slides. Deixe a demonstração no terminal.
