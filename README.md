hpc_ufw_cpp
===========

Micro Framework para HPC (Versão C++)



SPRINT 1 - Versão mínima funcional
==================================

A ideia é, ao final deste sprint, termos uma estrutura minima onde utiliza-se libev, faz bind, accept, recebe msg e envia resposta, utilizando-se um modelo simples de concorrência onde o master faz accept e repassa para as workers threads.


Tarefas identificadas:

* TDD/FDD
* (OK) Um app de teste
* (OK) Classe abstrata que define os modelos de concorrência
* (OK) Classe que implementa o modelo de concorrência com várias threads
* (OK) Utilizar libev para implementar o modelo reativo
* (OK) Apenas um modelo de concorrência
   * (OK) Threads
   * (OK) Cada worker tem seu loop (libev)
   * (OK) Master faz bind, listen e accept, e repassa tarefas para as workers pelo loop
   * (OK) Define porta (pode ser fixa, dentro do modelo carregado)
* Makefile
* Capturar sinais de erro



BACKLOG
=======

* Suporte a protocolos de tamanho variável, podendo inclusive receber pacotes incompletos e, se necessário, parser reentrante
* Implementar outros modelos de concorrência 
   * Accept nas threads
   * Fork ao invés de threads
* Cada worker processar várias requisições
* Parser/tarefas poderem relatar erros
