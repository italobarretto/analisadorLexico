# analisadorLexico
Um analisador léxico elaborado com automato de pilha

## funcionamento
  1. Para operação do analisador, faz-se necessário adicionar o arquivo em txt "ListaSimbolos.txt" na mesma pasta do programa. Bem como os arquivos a serem analisados;
  2. Ao abrir o programa insira o nome do arquivo txt a ser analisado, conforme o padrão do exemplo apresentado. É necessário digitar a extensão do arquivo;
  3. Após adicionar o nome e pressionar a tecla enter, aguarde o programa realizar a sua análise e retornar com os resultados obtidos.

Obs.: O programa não funcionou corretamente com o Code::Blocks. No entanto, funcionou perfeitamente com o DevC++. Por conta disso estou incluindo também o executável do programa já compilado.

## elaboração
  1. O programa foi elaborado utilizando um automato com pilha para realizar a análise dos operadores de fechamento {}();
  2. Uma lista simplesmente encadeada foi utilizada para armazenar a lista de símbolos para utilização do programa. Essa mesma lista também foi utilizada para adicionar novos elementos a lista de símbolos, quando o programa chamava as funções num e string;
  3. As principais funcionalidades e variáveis do programa encontram-se com comentários para facilitar a interpretação do código.
