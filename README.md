<!-- Henrique Gomes Zanin NUSP: 10441321 -->
<!-- Gabriel Guimaraes Vilas Boas Marin NUSP: 11218521 -->
# B-Tree-DBMS

Toda a discussão relativa à implementação encontra-se no pdf.

Para compilar o trabalho execute no diretório onde está o Makefile:
    $ make

Será gerado um arquivo binário com o nome "main" no diretório bin/. Para executa-lo:

    $ ./main

 ou

    $ ./main --cmd

 ou

    $./main --batch

A explicação das variações podem ser encontradas no PDF. Não é necessário carregar
a tabela para o primeiro comando, apenas para os demais.

Para gerar 10 casos de teste execute no diretório tests:
    
    $ make

    $ ./main 10 > caso.in

O número de inserções pode ser modificado para qualquer valor desejado. 
O arquivo gerado contém as instruções necessárias para inserir todos os registros. Após as inserções a 
busca pode ser feita na interface principal ou pelo cmd por meio do comando search. Segue um exemplo de
carregamento em lote:

    $ ./main --batch caso.in

Dois casos de teste foram disponibilizados na pasta bin, um com 20 inserções e outro com 20.000

Qualquer dúvida de como executar mande um email para: henrique.zanin@usp.br
