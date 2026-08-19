# 🛠️ Ordem de Implementação — ProcessFlow

---

## 🧱 Fase 1 — Esqueleto do programa

- [X] Criar `main.c` com `main(argc, argv)`
- [X] Verificar número de argumentos: 0 → modo interativo, 1 → modo workflow, >1 → erro e encerrar
- [X] Implementar parser de linha: ler linha, tokenizar por espaço, ignorar tokens vazios (trata múltiplos espaços)
- [X] Ignorar linha vazia no prompt silenciosamente
- [X] Implementar loop interativo: exibir `processflow> `, ler linha, despachar comando
- [X] Implementar `exit` encerrando o loop
- [X] Tratar CTRL-D (EOF em `fgets`/`getline`) encerrando normalmente

---

## 📋 Fase 2 — Cadastro de tarefas

- [X] Definir struct `Task` com nome, programa e array de argumentos
- [X] Implementar array/lista de tarefas cadastradas
- [X] Implementar comando `task <nome> <programa> [args...]` populando a struct
- [X] Implementar busca de tarefa por nome (retorna NULL se não existir)
- [X] Ao tentar executar tarefa inexistente: imprimir erro e continuar

---

## ▶️ Fase 3 — Execução simples

- [X] Implementar função `spawn(task)`: `fork()` + `execvp()` no filho
- [X] No pai: `waitpid()` aguardando o filho
- [X] Tratar falha de `execvp()` no filho: imprimir erro e `exit(1)` do filho
- [X] Implementar `run sequential <tarefa>` chamando `spawn()` para uma tarefa
- [X] Testar: `task listar /bin/ls -l` → `run sequential listar`

---

## 🔗 Fase 4 — Sequencial + Paralelo

- [X] Estender `run sequential` para N tarefas: chamar `spawn()` em loop, aguardando cada filho antes do próximo
- [ ] Implementar `run parallel`: fazer todos os `fork()` antes de qualquer `wait()`
- [ ] Coletar filhos paralelos em loop de `waitpid()` até todos terminarem (sem assumir ordem)
- [ ] Testar filhos terminando em ordens diferentes (ex.: `sleep 2` e `sleep 1` em paralelo)

---

## 🔀 Fase 5 — Pipe

- [ ] Implementar função `run_pipe(tasks[], n)` recebendo array de tarefas
- [ ] Criar N-1 pipes com `pipe()` antes de qualquer `fork()`
- [ ] Para cada filho i: `dup2(pipe[i-1][0], STDIN)` e `dup2(pipe[i][1], STDOUT)` conforme posição
- [ ] Fechar todas as pontas não usadas no filho antes de `exec()`
- [ ] No pai: fechar todas as pontas dos pipes e aguardar todos os filhos
- [ ] Testar: `run pipe listar ordenar contar` (ls | sort | wc)

---

## 📂 Fase 6 — Redirecionamento

- [ ] Criar campos `input_file`, `output_file`, `append_file` na struct `Task`
- [ ] Implementar comando `input <tarefa> <arquivo>`: salva caminho na struct
- [ ] Implementar comando `output <tarefa> <arquivo>`: salva caminho na struct (modo truncar)
- [ ] Implementar comando `append <tarefa> <arquivo>`: salva caminho na struct (modo append)
- [ ] Na função `spawn()`, antes do `exec()`: abrir arquivo e `dup2()` para STDIN/STDOUT se definido
- [ ] Erro ao abrir arquivo: imprimir mensagem no filho e `exit(1)` (pai captura via `waitpid`)

---

## 📜 Fase 7 — workflowFile + workdir

- [ ] Implementar leitura de arquivo `.pf`: abrir, ler linha a linha, imprimir cada linha antes de processar
- [ ] Reusar o mesmo dispatcher de comandos do modo interativo
- [ ] Encerrar ao ler `exit` ou ao chegar no EOF do arquivo
- [ ] Erro ao abrir arquivo `.pf`: imprimir mensagem e encerrar o programa
- [ ] Implementar `workdir <diretório>`: chamar `chdir()` e atualizar variável de diretório atual
- [ ] Diretório inexistente em `workdir`: imprimir erro com `perror()` e continuar
- [ ] Testar modo workflow com arquivo `.pf` completo incluindo `workdir`

---

## 🌑 Fase 8 — Background + jobs + wait

- [ ] Definir struct `Job` com jobId, PID, nome da tarefa e status (running/done)
- [ ] Implementar lista de jobs ativos
- [ ] Implementar `start <tarefa>`: `fork()` sem `waitpid()`, registrar job, imprimir `[jobId] PID`
- [ ] Implementar `jobs`: iterar lista e exibir jobs ainda ativos
- [ ] Implementar `wait <jobId>`: buscar job por id, chamar `waitpid(pid)`, marcar como concluído
- [ ] Job inexistente em `wait`: imprimir erro e continuar
- [ ] Implementar coleta de zumbis: instalar handler de `SIGCHLD` com `waitpid(-1, WNOHANG)` em loop, atualizando status na lista de jobs
- [ ] Testar `start`, `jobs`, `wait` e verificar ausência de processos zumbi

---

## ⚠️ Fase 9 — Revisão de erros e casos limite

- [ ] Revisar todos os `fork()`/`exec()`/`pipe()`/`open()` com tratamento de erro
- [ ] Garantir que processo filho com código de saída ≠ 0 não derruba o pai
- [ ] Testar linha vazia, espaços múltiplos, CTRL-D e workflow sem `exit`
- [ ] Testar argumentos inválidos ao iniciar (`./processflow a b c`)

---

## 🗂️ Fase 10 — Empacotamento e entrega

- [ ] Criar `Makefile` com targets `all`, `clean` e `test`
- [ ] Criar `README.md` com SO, arquivos, como compilar, executar e testar
- [ ] Gravar sessão de testes: `script -a evidencias.log` → rodar todos os casos → `exit`
- [ ] Fazer commits atômicos no GitHub (um por feature implementada)
- [ ] Escrever relatório PDF seguindo o Guia (seções 1–10)
- [ ] Montar diretório `iniciais/` com tudo, compactar: `tar -cf iniciais.tar iniciais/`
- [ ] Submeter no Google Classroom