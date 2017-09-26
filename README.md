# Dropbox
Trabalho prático da disciplina de Sistemas Operacionais II da UFRGS, implementa um serviço semelhante ao Dropbox. 

## Especificação
Criar um servidor responsável por gerenciar arquivos de diversos usuários (clientes). A estrutura mínima de arquivos corresponde aos descritos abaixo:
* `dropboxServer.h` e `dropboxServer.c` implementam as funções do servidor.
* `dropboxClient.h` e `dropboxClient.c` implementam as funções relacionadas ao cliente.
* `dropboxUtil.h` e `drobpoxUtil.c` implementam métodos auxiliares, armazenam constantes, etc.

Um exemolo de coneção com o servidor é `./dropboxClient user endereço porta` no qual, para cada cliente, o servidor deve garantir **múltiplas sessões**, limitadas em 2, através das quais o mesmo cliente pode acessar o serviço através de dispositivos distintos simultaneamente; **consistência nas estruturas de armazenamento**, e **sincronização no aceso de arquivos**, possibilitando que cada vez que o cliente modifique um arquivo, esse arquivo seja atualizado no servidor. 

### Projeto do sistema e estruturas de dados
Para cada cliente conectado ao servidor, uma interface com o usuário deverá ser acessível via linha de comando com as seguintes requisições:
* `upload <path/filename.ext>` envia o arquivo `filename.ext` para o servidor.
* `download <filename.ext>` faz download do arquivo `filename.ext` do servidor para o diretório local.
* `list_server` lista os arquivos salvos no servidor que sejam associados ao usuário.
* `list_client` lista os arquivos salvos no diretório `sync_dir_<nomeUsuário>`
* `get_sync_dir` cria o diretório `sync_dir_<nomeUsuário>` no `\home` do usuário.
* `exit` encerra a conexo com o servidor.

O comando `get_sync_dir` deve ser executado logo após o estabelecimento de uma conexão entre cliente e servidor. Toda vez que esse comando for executado, o servidor verificará se o diretório `sync_dir_<nomeUsuário>` existe no dispositivo do cliente. Caso contrário, o diretório devererá ser criado e a sincronização efetuada. Toda vez que alguma mudança ocorrer dentro desse diretório, essa mudança deverá ser espelhada no servidor pelo cliente.


#### Interface mínima para o cliente
```C
  int connect_server(char* host, int port);
// Conecta o cliente com o servidor.
//   host: endereço do servidor
//   port: porta aguardando conexão


  void sync_client();
// Sincroniza o diretório `sync_dir_<nomeUsuário>` com o servidor.


  void send_file(char* file);
// Envia um arquivo `file` para o servidor. Deverá ser executada quando for realizar o upload de um arquivo.
//   file: nome+extensão do arquivo a ser enviado.
  

  void get_file(char* file);
// Obtém um arquivo `file` do servidor. Deverá ser executada quando for realizar o download de um arquivo.
//   file: nome+extensão do arquivo a ser baixado.


  void delete_file(char* file);
// Exclui um arquivo `file` de `sync_dir_<nomeUsuário>`
//   file: nome+extensão do arquivo a ser excluído.


  void close_connection();
// Encerra a conexão com o servidor
```
#### Interface mínima para o servidor
```C
  void sync_server();
// Sincroniza o servidor com o diretório `sync_dir_<nomeUsuário>` do cliente.


  void receive_file(char* file);
// Recebe o arquivo `file` do cliente. 
// Deverá ser executada quando for realizar o upload de um arquivo.
//     file: caminho+nome+extensão do arquivo a ser recebido


  void send_file(char* file);
// Envia o arquivo `file` para o cliente.
// Deverá ser executada quando for realizar download de um arquivo.
//     file: caminho+nome+extensão do arquivo a ser enviado

```

#### Estruturas de dados mantidas no servidor
*Estarão dispostas no servidor na forma de uma lista encadeada.* 
```C
struct client {
  int    devices[2];            // associado aos dispositivos do usuário
  char   userid[MAXNAME];       // id do usuário no servidor, que deverá ser único. Informado pela linha de comando.
  struct file_info[MAXFILES];   // metadados de cada arquivo que o cliente possui no servidor.
  int    logged_in;             // cliente está logado ou não
  
}

struct file_info {
  char name[MAXNAME];           // nome do arquivo
  char extension[MAXNAME];      // tipo de extensão do arquivo
  char last_modified[MAXNAME];  // data da última modificação do arquivo
  int  size;                    // tamanho em bytes do arquivo
  
 }

```


