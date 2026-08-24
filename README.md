# ProcessFlow

Orquestrador de processos em C que gerencia "tasks" (comandos nomeados) e as executa de forma sequencial, paralela ou encadeada por pipe, com suporte a redirecionamento de entrada/saída, jobs em background e mudança de diretório.

## Sistema operacional

Implementado e testado em **Linux** (usa `fork`, `execvp`, `waitpid`, `dup2`, `pipe`, `chdir`). Não é portável para Windows sem uma camada de compatibilidade (WSL/Cygwin).

## Arquivos

- `main.c`: loop principal (modo interativo e modo workflow) e parser de comandos
- `task.c` / `task.h`: struct `Task` e cadastro/busca de tarefas (`criar_task`, `encontrar_task`)
- `exec.c` / `exec.h`: execução das tarefas: `spawn` (síncrono), `spawn_async` (background), `run_pipe` (encadeamento via pipe)
- `job.c` / `job.h`: controle de jobs em background: `criar_job`, `encontrar_job`, `listar_jobs_ativos`, `marcar_job_concluido`, `sigchld_handler`
- `Makefile`: regras de compilação

## Compilar

```bash
make
```

Gera o binário `processflow` (usa `gcc -Wall -Wextra -g`).

Para limpar:

```bash
make clean
```

## Executar

Modo interativo:

```bash
./processflow
```

Modo workflow (lê comandos de um arquivo `.pf`, imprimindo cada linha antes de executá-la):

```bash
./processflow arquivo.pf
```

### Comandos suportados

- `task <nome> <programa> [args...]`: cadastra uma tarefa
- `run [sequential|parallel|pipe] <task1> <task2> ...`: executa tarefas (padrão: sequencial)
- `start <task>`: executa em background, imprime `[job_id] pid`
- `jobs`: lista jobs em background ativos
- `wait <job_id>`: aguarda o término de um job
- `input <task> <arquivo>` / `output <task> <arquivo>` / `append <task> <arquivo>`: configura redirecionamento
- `workdir <dir>`: muda o diretório de trabalho
- `exit`: encerra o programa

## Testar

```bash
make test
```

Builda e roda os casos em `Testes/` (cada `testeN-entrada.txt` é comparado contra `testeN-saida.txt` esperado).

Validação adicional feita manualmente:

- Execução interativa dos comandos acima com programas reais (`/bin/ls`, `/bin/echo`, etc.)
- Arquivos `.pf` de workflow exercitando os cenários (sequencial, paralelo, pipe, redirecionamento, jobs, erros)
- Evidências dos testes registradas em `evidencias.log`
