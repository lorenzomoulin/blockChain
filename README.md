# blockChain
O código em C usa a estratégia de buscar as hashes iterando por números consecutivos, então dado um número N e um número K de threads,
o programa vai dividir N em K blocos de inteiros a serem testados. Essa estratégia é válida, porque a hash de x e x+1 é bem diferente,
então acaba sendo uma busca aleatória nas hashes.
Para rodar o código em C, que está na pasta "desafio de hoje":
  1 - entrar na pasta "desafio de hoje"
  2 - abrir o terminal e executar o comando "make":
  3 - abrir outra aba do terminal
  4 - executar "./rpcMiner_server"
  5 - na outra aba executar ./rpcMiner_server <host do server> <Numero máximo do bruteforce> <Número de threads>
  6 - escolher a opção desejada do menu
  
---------------------------------------------------------
  
Para rodar o código em python, que está na pasta "PYTHON":
  1 - entrar na pasta "PYTHON"
  2 - abrir o terminal e executar o comando "make":
  3 - abrir outra aba do terminal
  4 - executar "./rpcMiner_server"
  5 - na outra aba executar ./rpcMiner_server <host do server> <Numero máximo do bruteforce> <Número de threads>
