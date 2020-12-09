# IOT Aplicada - Aviário

Esse projeto compreende em demonstrar front de uma aplicação para controle de temperatura e umidade para a disciplina de IOT Aplicada em um ambiente de viário.

## Funcionalidade

Para a construção deste projeto, foi utilizado a plataforma do esp32 para controlar a temperatura e umidade do aviário.

O processo funciona da seguinte maneira:

1) Deve ser definido em qual estágio de idade os filhotes se encontram, conforme os dias de idade há algumas diferenças na temperatura ideal que deve ser mantido.

2) Os dados são enviados via request para o servidor, que faz a leitura dos dados e grava no banco de dados para manter o histórico da temperatura.

3) A plataforma mostra os dados de temperatura e umidade na página. Conforme vai surgindo mais aviários, a página se encaminha de deixá-los lado a lado para melhor visualização. Há também alertas para informar se a temperatura ou umidade estão fora do ideal.


## Instalação

1) Abra este projeto pelo Platform.IO

2) Altere o Usuario e senha do Wi-FI e as portas utilizadas dentro do arquivo defines.h

3) Faça o upload do projeto ao ESP32
