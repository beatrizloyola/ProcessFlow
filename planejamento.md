# DIA 1

## Hora 1 - Conceitos mínimos

- `fork()` — cria um processo filho
- `execvp()` — substitui o filho por outro programa
- `waitpid()` — o pai espera o filho terminar

## Hora 2 - Esqueleto

- Loop de entrada com suporte aos dois modos:
  - **Interativo:** lê do terminal e exibe o prompt `processflow>`
  - **Workflow:** lê do arquivo `.pf` e imprime cada linha antes de processá-la
- Parsing da linha (separar comandos dos argumentos, lidar com múltiplos espaços)
- Struct `task` com campos para: nome, programa, argumentos, arquivo de entrada, arquivo de saída, modo append

## Hora 3 - Implementar `task` e `run sequential`

- `task listar /bin/ls -l` → salva na lista de tarefas
- `run sequential t1 t2` → executa uma por vez com fork+exec+wait
- Testar com comandos reais (`/bin/ls`, `/bin/echo`)

---

# DIA 2

## Hora 1 - `run parallel` e `start`/`jobs`/`wait`

- `run parallel` → fork todos antes de chamar `waitpid`
- `start` → fork sem esperar, guardar PID e job ID numa lista
- `jobs` → checar com `waitpid(-1, WNOHANG)` quais já terminaram, depois listar os ativos
- `wait <jobId>` → chamar `waitpid` no PID certo

## Hora 2 - Redirecionamento (`dup2`)

- `input`, `output`, `append` → configuram campos na struct da tarefa (não executam ainda)
- Na hora do `run`, o filho abre o arquivo e usa `dup2()` para redirecionar stdin/stdout antes do `exec`

## Hora 3 - `run pipe` e `workdir`

- `pipe()` + `dup2()` para encadear saída de uma tarefa na entrada da próxima
- `workdir` → `chdir()` antes de executar as tarefas seguintes

---

# DIA 3

## Hora 1 - Tratamento de erros

- Tarefa não cadastrada
- Programa não encontrado ou não pode ser executado
- Arquivo de entrada/saída não pode ser aberto
- Diretório inválido em `workdir`
- Job informado não existe
- Número incorreto de argumentos ao iniciar o ProcessFlow
- Arquivo workflow não existe ou não pode ser aberto (encerrar)

## Hora 2 - Casos especiais

- Linha vazia no prompt
- Múltiplos espaços entre tokens
- Arquivo `.pf` sem `exit` (tratar EOF)
- Ctrl-D no modo interativo
- Processos filhos terminando com código ≠ 0
- Background: coletar processos filhos que terminaram para evitar zumbis (`waitpid(-1, WNOHANG)`)
- Processos em paralelo terminando em ordens diferentes

## Hora 3 - Testes integrados

- Criar um arquivo `.pf` que exercite tudo junto, rodar e ajustar