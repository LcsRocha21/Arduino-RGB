Este repositório contém o código para um veículo autônomo baseado em Arduino, projetado para simular um robô seguidor de linha com detecção de obstáculos e respostas visuais. O sistema combina sensores infravermelhos, RGB e ultrassônicos, além de LEDs, para executar as seguintes funcionalidades:

Funcionalidades
1. Seguir a Faixa Branca
- O veículo utiliza sensores infravermelhos para detectar faixas brancas em uma pista preta.
- A movimentação inicia quando o sensor RGB identifica a cor verde, permitindo que o veículo siga a faixa com precisão.

2. Parar o Veículo
Os motores são desligados automaticamente em duas situações:
- Quando o sensor RGB detecta a cor vermelho.
- Quando o sensor ultrassônico identifica um obstáculo a uma distância configurada.
No caso de obstáculos detectados pelo sensor ultrassônico, um LED verde é ativado como alerta.

3. Alerta com LED Vermelho
- Sempre que o sensor RGB identifica a cor amarelo, um LED vermelho é acionado como sinal visual.
  
Este projeto combina lógica de sensores e respostas autônomas, sendo ideal para estudos em automação, veículos autônomos e sistemas embarcados.
