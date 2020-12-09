# IOT Aplicada - Aviário

Esse projeto compreende em demonstrar front de uma aplicação para controle de temperatura e umidade para a disciplina de IOT Aplicada em um ambiente de viário.

## Como rodar

Para rodar esse projeto é necessário ter o Node instalado na máquina.

##### Para instalar as dependências `npm install`
##### Para rodar o projeto `npm run start`

## Informações gerais

O front se alimenta do firebase para a manipulação dos dados.   
Após receber os dados, é feito um mapeamento para identificar os aviários e obter os dados de cada aviário.   
Com os aviários identificados e os dados de cada, é feito uma consulta de qual último valor.   
Utilizando o último valor, é feito uma convergência com os dados ideais e é mostrado para o usuário pela barra de notificação e display de retângulo.   
A aplicações compreende validação de múltiplos usuários.   

## Necessidades

É necessários informar o servidor firebase para o funcionamento do projeto, a configuração deve ser colocado em: `src/App.js` na variável `firebaseConfig`;


## Libs utilizadas

##### `firebase` para consulta dos dados
##### `moment` para manipulação de datas
##### `react-toastify` para mostrar notificações
##### `react-loading` para loading inicial
