# IOT Aplicada - Aviário

Esse projeto compreende em demonstrar front de uma aplicação para controle de temperatura e umidade para a disciplina de IOT Aplicada em um ambiente de viário.

## Como rodar

Para rodar esse projeto é necessário ter o Node instalado na máquina.

##### Para instalar as dependências `npm install`
##### Para rodar o projeto `npm run dev`

## Informações gerais

O backend está estruturado para receber uma requisição `POST`, onde irá salvar as informações provenientes do ESP32 no banco FIREBASE.
A ideia é ser um servidor ponte onde só receberá e já gravar no firebase.

## Necessidades

É necessário informar o servidor firebase para o funcionamento do projeto, a configuração deve ser colocado em: `src/api/aviario/firebase.js`

## Libs utilizadas

##### `express` para levantar servidor
##### `firebase` para gravar os dados
