hpc_ufw_cpp
===========

Micro Framework para HPC (Versão C++)



VERSÃO 0.1 - Versão mínima funcional
====================================

* TDD/FDD
* Um app de teste
* Classe abstrata que define os modelos de concorrência
* Classe que implementa o modelo de concorrência com várias threads
* Apenas um modelo de concorrência
    * Threads
    * Cada worker tem seu epoll (ou similar)
    * Master faz bind, listen e accept, e repassa tarefas para as workers pelo epoll
    * Define porta (pode ser fixa, dentro do modelo carregado)
    * Faz bind/listen (dentro do modelo carregado)
* Makefile
* Monitora eventos adicionando os sockets no epoll
* Captura sinais de erro



MILESTONES:
===========

* (OK) estrutura minima onde utiliza libev, faz bind, accept, recebe msg e envia resposta
* (OK) client que envia msg (protocolo texto) e recebe resposta
* (OK) Client envia tarefa (protocolo binario)
* (OK) Server decodifica protocolo, executa, responde
* Multithread
* Capturar sinais de erro



BACKLOG
=======

* Suporte a protocolos de tamanho variável, podendo inclusive receber pacotes incompletos e, se necessário, parser reentrante
* Implementar outros modelos de concorrência 
   * Accept nas threads
   * Fork ao invés de threads
* Cada worker processar várias requisições
